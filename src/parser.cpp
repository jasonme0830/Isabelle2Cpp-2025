#include "parser.hpp"

#include <set>
#include <map>
#include <vector>

using namespace std;

namespace hol2cpp {
namespace operators {
// set<Token::Type> uops;

vector<pair<size_t, set<Token::Type>>> bop_precedences {
    {
        30, {
            Token::Type::Or
        }
    },
    {
        35, {
            Token::Type::And
        }
    },
    {
        50, {
            Token::Type::Equiv, Token::Type::NotEq,
            Token::Type::Le, Token::Type::Lt, Token::Type::Ge, Token::Type::Gt,
            Token::Type::Subset, Token::Type::SubsetEq, Token::Type::SupsetEq, Token::Type::Supset,
            Token::Type::In, Token::Type::NotIn
        }
    },
    {
        65, {
            Token::Type::Sharp, Token::Type::At, Token::Type::Union,
            Token::Type::Add, Token::Type::Sub
        }
    },
    {
        70, {
            Token::Type::Inter,
            Token::Type::Mul, Token::Type::Div, Token::Type::Mod
        }
    },
    {
        80, {
            Token::Type::Pow
        }
    }
};

set<Token::Type> bop_right_associativity {
    Token::Type::Or, Token::Type::And,
    Token::Type::Pow,
    Token::Type::Sharp, Token::Type::At
};

set<Token::Type> &bops_at_layer(size_t layer) {
    return bop_precedences[layer].second;
}
} // namespace operators

Parser::Parser(ifstream &input) noexcept
  : tokenizer_(input), current_token_(tokenizer_.next_token()) {
    // ...
}

void Parser::get_next_token() {
    current_token_ = tokenizer_.next_token();
}

Token &Parser::next_token() {
    return current_token_ = tokenizer_.next_token();
}

Theory Parser::gen_theory() {
    Theory theory;

    eat_until<Token::Type::Theory>();
    get_next_token();

    check<Token::Type::Identifier>("expected an identifier");
    theory.name = current_token_.value;
    get_next_token();

    eat<Token::Type::Imports>("expected keyword imports");
    while (!meet<Token::Type::Begin>()) {
        check<Token::Type::Identifier>("expected an identifier");
        theory.imports.push_back(current_token_.value);
        get_next_token();
    }
    get_next_token(); // eat begin

    while (!meet<Token::Type::EndOfFile>()) {
        try {
            if (auto decl = gen_declaration()) {
                theory.declarations.push_back(move(decl));
            }
        } catch (const exception &e) {
            theory.declarations.push_back(nullptr);
            "\033[1;34mparse error\033[0m at $nd declaration:\n    $\n"_fs.outf(cerr,
                theory.declarations.size(), e.what()
            );
            tokenizer_.get_next_input();
        }
    }

    return theory;
}

Ptr<Declaration> Parser::gen_declaration() {
    eat_until<Token::Type::Datatype, Token::Type::Function>();

    if (meet<Token::Type::EndOfFile>()) {
        return nullptr;
    } else if (meet<Token::Type::Datatype>()) {
        return gen_datatype_declaration();
    } else {
        return gen_function_declaration();
    }
}

Ptr<DataTypeDecl> Parser::gen_datatype_declaration() {
    auto decl = make_unique<DataTypeDecl>();

    eat<Token::Type::Datatype>("expected token Datatype");
    if (meet<Token::Type::Identifier>()) {
        decl->decl_type = gen_normal_type();
    } else {
        decl->decl_type = gen_template_type();
    }

    eat<Token::Type::Equiv>("expected token Equiv");
    decl->components.push_back(gen_component());
    while (meet<Token::Type::Pipe>()) {
        get_next_token();
        decl->components.push_back(gen_component());
    }

    return decl;
}

DataTypeDecl::Component Parser::gen_component() {
    DataTypeDecl::Component componment;

    check<Token::Type::Identifier>("expected an identifier");
    componment.constructor = current_token_.value;
    get_next_token();

    while (meet<Token::Type::TypeVariable, Token::Type::Identifier, Token::Type::Quotation>()) {
        componment.arguments.push_back(gen_type_term());
    }

    return componment;
}

Ptr<FuncDecl> Parser::gen_function_declaration() {
    auto decl = make_unique<FuncDecl>();

    eat<Token::Type::Function>("expected token Function");

    check<Token::Type::Identifier>("expected token Identifier");
    decl->name = current_token_.value;
    get_next_token();

    eat<Token::Type::Colonn>("expected ::");

    eat<Token::Type::Quotation>("expected token Quotation");
    decl->type = gen_func_type();
    eat<Token::Type::Quotation>("expected token Quotation");

    eat<Token::Type::Where>("expected token Where");
    decl->equations.push_back(gen_equation());
    while (meet<Token::Type::Pipe>()) {
        get_next_token();
        decl->equations.push_back(gen_equation());
    }

    return decl;
}

Equation Parser::gen_equation() {
    Equation equation;

    eat<Token::Type::Quotation>("expected token Quotation");
    /**
     * TODO: dispatch different term for default-func or infixl/r
     *  for infix, gen_spicial_expr
    */
    equation.pattern = gen_construction();
    eat<Token::Type::Equiv>("expected token Equiv");
    equation.expr = gen_expr();
    eat<Token::Type::Quotation>("expected token Quotation");

    return equation;
}

Ptr<Type> Parser::gen_type() {
    auto type = gen_func_type();
    if (type->types.size() == 1) {
        return move(type->types.front());
    } else {
        return type;
    }
}

Ptr<FuncType> Parser::gen_func_type() {
    auto type = make_unique<FuncType>();
    type->types.push_back(gen_pair_type());
    while (meet<Token::Type::Rightarrow>()) {
        get_next_token();
        type->types.push_back(gen_pair_type());
    }
    return type;
}

Ptr<Type> Parser::gen_pair_type() {
    auto type = gen_template_type();
    if (meet<Token::Type::Star>()) {
        get_next_token();

        auto template_type = make_unique<TemplateType>("pair"s);
        template_type->args.push_back(move(type));
        template_type->args.push_back(gen_pair_type());

        return template_type;
    }
    return type;
}

/**
 * TODO: support multi type variables
 *  e.g. ('a, 'b, 'c) Triple
 *  !note, (x, y, z) is not a valid type
 *  and (,,) is not equivalent to (**)
*/
Ptr<Type> Parser::gen_template_type() {
    auto type = gen_type_term();
    while (meet<Token::Type::Identifier>()) {
        type = make_unique<TemplateType>(current_token_.value, move(type));
        get_next_token();
    }

    /**
     * TODO: chech the type is types or not
    */

    return type;
}

Ptr<Type> Parser::gen_type_term() {
    switch (current_token_.type) {
        case Token::Type::TypeVariable:
            return gen_argument_type();
        case Token::Type::Identifier:
            return gen_normal_type();
        case Token::Type::LParen: {
            get_next_token();
            auto type = gen_type();
            eat<Token::Type::RParen>("expected token RParen");
            return type;
        }
        case Token::Type::Quotation: {
            get_next_token();
            auto type = gen_type();
            eat<Token::Type::Quotation>("expected token Quotation");
            return type;
        }
        default:
            throw std::runtime_error("gen_type_term error");
    }
}

Ptr<ArgumentType> Parser::gen_argument_type() {
    check<Token::Type::TypeVariable>("expected token TypeVariable");
    auto type = make_unique<ArgumentType>(current_token_.value);
    get_next_token();
    return type;
}

Ptr<NormalType> Parser::gen_normal_type() {
    check<Token::Type::Identifier>("expected token Identifier");
    auto type = make_unique<NormalType>(current_token_.value);
    get_next_token();
    return type;
}

vector<Ptr<Expr>> Parser::gen_exprs() {
    vector<Ptr<Expr>> exprs;
    exprs.push_back(gen_expr());
    while (meet<Token::Type::Comma>()) {
        get_next_token();
        exprs.push_back(gen_expr());
    }
    return exprs;
}

Ptr<Expr> Parser::gen_expr(size_t layer) {
    if (layer == operators::bop_precedences.size()) {
        return gen_term();
    }

    auto lhs = gen_expr(layer + 1);
    auto op = current_token_;
    while (operators::bops_at_layer(layer).count(op.type)) {
        get_next_token();
        if (operators::bop_right_associativity.count(op.type)) {
            return make_unique<BinaryOpExpr>(op, move(lhs), gen_expr(layer));
        }

        lhs = make_unique<BinaryOpExpr>(op, move(lhs), gen_expr(layer + 1));
        op = current_token_;
    }
    return lhs;
}

Ptr<Expr> Parser::gen_term() {
    if (meet<Token::Type::Identifier>()) {
        return gen_construction();
    } else {
        return gen_factor();
    }
}

Ptr<Expr> Parser::gen_construction() {
    check<Token::Type::Identifier>("expected token Identifier");
    auto name = current_token_.value;
    get_next_token();

    function has_arg = [this] () mutable {
        return meet<Token::Type::Identifier,
            Token::Type::If,
            Token::Type::LBracket,
            Token::Type::LBrace,
            Token::Type::LParen,
            Token::Type::Integer
        >();
    };

    if (!has_arg()) {
        return make_unique<VarExpr>(move(name));
    }

    auto expr = make_unique<ConsExpr>(move(name));
    while (has_arg()) {
        expr->args.push_back(gen_factor());
    }

    return expr;
}

Ptr<Expr> Parser::gen_factor() {
    switch (current_token_.type)
    {
        case Token::Type::Identifier:
            return gen_var();
        case Token::Type::If:
            return gen_ifelse();
        case Token::Type::LBracket:
            return gen_list();
        case Token::Type::LBrace:
            return gen_set();
        case Token::Type::LParen:
            return gen_pair();
        case Token::Type::Integer:
            return gen_integral();

        default:
            throw std::runtime_error("gen_factor error");
    }
}

Ptr<Expr> Parser::gen_var() {
    check<Token::Type::Identifier>("expected token Identifier");
    auto expr = make_unique<VarExpr>(current_token_.value);
    get_next_token();
    return expr;
}

Ptr<Expr> Parser::gen_ifelse() {
    auto expr = make_unique<ConsExpr>("If"s);

    eat<Token::Type::If>("expected token If");
    expr->args.push_back(gen_expr());
    eat<Token::Type::Then>("expected token Then");
    expr->args.push_back(gen_expr());
    eat<Token::Type::Else>("expected token Else");
    expr->args.push_back(gen_expr());

    return expr;
}

Ptr<Expr> Parser::gen_list() {
    eat<Token::Type::LBracket>("expected token LBracket");
    if (meet<Token::Type::RBracket>()) {
        get_next_token();
        return make_unique<ListExpr>();
    }
    auto expr = make_unique<ListExpr>(gen_exprs());
    eat<Token::Type::RBracket>("expected token RBracket");
    return expr;
}

Ptr<Expr> Parser::gen_set() {
    eat<Token::Type::LBrace>("expected token LBrace");
    if (meet<Token::Type::RBrace>()) {
        get_next_token();
        return make_unique<SetExpr>();
    }
    auto expr = make_unique<ListExpr>(gen_exprs());
    eat<Token::Type::RBrace>("expected token RBrace");
    return expr;
}

Ptr<Expr> Parser::gen_pair() {
    eat<Token::Type::LParen>("expected token LParen");
    auto expr = gen_pair_helper();
    eat<Token::Type::RParen>("expected token RParen");
    return expr;
}

Ptr<Expr> Parser::gen_pair_helper() {
    auto expr = gen_expr();
    if (meet<Token::Type::Comma>()) {
        get_next_token();

        auto cons_expr = make_unique<ConsExpr>("Pair"s);
        cons_expr->args.push_back(move(expr));
        cons_expr->args.push_back(gen_pair_helper());

        return cons_expr;
    }
    return expr;
}

Ptr<Expr> Parser::gen_integral() {
    auto expr = make_unique<IntegralExpr>(current_token_.value);
    get_next_token();
    return expr;
}
} // namespace hol2cpp
