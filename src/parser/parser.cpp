#include "parser.hpp"
#include "../utility/error.hpp"

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
    },
    {
        100, {
            Token::Type::Excl
        }
    }
};

set<Token::Type> bop_right_associativity {
    Token::Type::Or, Token::Type::And,
    Token::Type::Pow,
    Token::Type::Sharp, Token::Type::At
};

map<Token::Type, string> bop_func_mapping {
    { Token::Type::Sharp, "Cons" },
    { Token::Type::At, "append" },
    { Token::Type::Excl, "nth" },
};

set<Token::Type> temp_ignored_ops;

set<Token::Type> &bops_at_layer(size_t layer) {
    return bop_precedences[layer].second;
}
} // namespace operators

void Parser::add_infix_op(Token::Type type, size_t precedence, const string &func, bool is_infixl) {
    auto lower = lower_bound(operators::bop_precedences.begin(),
        operators::bop_precedences.end(), std::make_pair(precedence, std::set<Token::Type>()),
        [] (const std::pair<size_t, std::set<Token::Type>> &lhs, const std::pair<size_t, std::set<Token::Type>> &rhs)
        {
            return lhs.first < rhs.first;
        }
    );
    operators::bop_precedences.insert(lower, std::make_pair(precedence, std::set<Token::Type>{ type }));
    operators::bop_func_mapping.emplace(type, func);
    if (!is_infixl) {
        operators::bop_right_associativity.insert(type);
    }
}

Parser::Parser(ifstream &input, string name) noexcept
  : tokenizer_(input, move(name)), current_token_(tokenizer_.next_token()) {
    // ...
}

void Parser::get_next_token() {
    current_token_ = tokenizer_.next_token();
}

Token &Parser::next_token() {
    return current_token_ = tokenizer_.next_token();
}

std::string Parser::next_raw_str() {
    /** the position after `check`
     * "..."
     *  ^
    */
    check<Token::Type::Quotation>("expected token Quotation");
    /** the position after `next_raw_str`
     * "..."
     *     ^
    */
    auto res = tokenizer_.next_raw_str();
    get_next_token();
    eat<Token::Type::Quotation>("expected token Quotation");
    return res;
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
                theory.definitions.push_back(move(decl));
            }
        } catch (const TokenizeError &e) {
            "$ after No.$ definition:\n$\n"_fs.outf(cerr,
                info::light_blue("tokenize error"), theory.definitions.size(), e.what()
            );
            tokenizer_.get_next_input();
        } catch (const ParseError &e) {
            theory.definitions.push_back(make_unique<ErrorDefinition>(e.error_type()));
            "$ in No.$ definition:\n$\n"_fs.outf(cerr,
                info::light_blue("parse error"), theory.definitions.size(), e.what()
            );
        }
    }

    return theory;
}

Ptr<Definition> Parser::gen_declaration() {
    eat_until<Token::Type::Datatype, Token::Type::Function>();

    if (meet<Token::Type::EndOfFile>()) {
        return nullptr;
    } else if (meet<Token::Type::Datatype>()) {
        try {
            return gen_datatype_definition();
        } catch (const ParseError &e) {
            throw ParseError(e, PEType::Datatype);
        }
    } else {
        try {
            return gen_function_definition();
        } catch (const ParseError &e) {
            throw ParseError(e, PEType::Function);
        }
    }
}

Ptr<DatatypeDef> Parser::gen_datatype_definition() {
    auto decl = make_unique<DatatypeDef>();

    eat<Token::Type::Datatype>("expected token Datatype");
    if (meet<Token::Type::Identifier>()) {
        decl->decl_type = gen_normal_type();
    } else {
        decl->decl_type = gen_template_type();
    }

    if (decl->is_predefined()) {
        return decl;
    }

    eat<Token::Type::Equiv>("expected token Equiv");
    do {
        decl->components.push_back(gen_component());
    } while (try_eat<Token::Type::Pipe>());

    return decl;
}

DatatypeDef::Component Parser::gen_component() {
    DatatypeDef::Component componment;

    check<Token::Type::Identifier>("expected an identifier");
    componment.constructor = current_token_.value;
    get_next_token();

    while (meet<Token::Type::TypeVariable, Token::Type::Identifier, Token::Type::Quotation>()) {
        componment.arguments.push_back(gen_type_term());
    }

    return componment;
}

Ptr<Definition> Parser::gen_function_definition() {
    auto decl = make_unique<FunctionDef>();

    eat<Token::Type::Function>("expected token Function");

    if (try_eat<Token::Type::LParen>()) {
        eat<Token::Type::Identifier>("expected token Identifier");
        eat<Token::Type::RParen>("expected token RParen");
    }

    if (try_eat<Token::Type::Quotation>()) {
        auto name = gen_ident_str();
        if (try_eat<Token::Type::Quotation>()) {
            decl->name = move(name);
        } else {
            auto short_def = make_unique<ShortDef>();
            short_def->name = name;
            while (meet<Token::Type::Identifier>()) {
                short_def->parameters.push_back(gen_ident_str());
            }
            eat<Token::Type::Equiv>("expected token Equiv");
            short_def->expr = gen_expr();
            eat<Token::Type::Quotation>("expected token Quotation");
            return short_def;
        }
    } else {
        check<Token::Type::Identifier>("expected token Identifier");
        decl->name = current_token_.value;
        get_next_token();
    }

    if (decl->is_predefined()) {
        return decl;
    }

    eat<Token::Type::Colonn>("expected ::");

    eat<Token::Type::Quotation>("expected token Quotation");
    decl->type = gen_func_type();
    eat<Token::Type::Quotation>("expected token Quotation");

    if (try_eat<Token::Type::LParen>()) {
        bool is_infixl = try_eat<Token::Type::Infixl>();
        if (!is_infixl) {
            eat<Token::Type::Infixr>("expected token Infixr");
        }

        auto op_str = next_raw_str();
        auto type = Tokenizer::add_token(op_str);
        if (!type.has_value()) {
            throw error("already used operator " + op_str);
        }

        check<Token::Type::Integer>("expected token Integer");
        auto precedence = stoull(current_token_.value);
        add_infix_op(type.value(), precedence, decl->name, is_infixl);

        get_next_token();
        eat<Token::Type::RParen>("expected token RParen");
    }

    eat<Token::Type::Where>("expected token Where");
    do {
        decl->equations.push_back(gen_equation());
    } while (try_eat<Token::Type::Pipe>());

    return decl;
}

Equation Parser::gen_equation() {
    Equation equation;

    if (try_eat<Token::Type::LBracket>()) {
        eat<Token::Type::Identifier>("expected token Identifier");
        eat<Token::Type::RBracket>("expected token RBracket");
        eat<Token::Type::Colon>("expected token Colon");
    } else if (try_eat<Token::Type::Identifier>()) {
        if (try_eat<Token::Type::LBracket>()) {
            eat<Token::Type::Identifier>("expected token Identifier");
            eat<Token::Type::RBracket>("expected token RBracket");
        }
        eat<Token::Type::Colon>("expected token Colon");
    }

    eat<Token::Type::Quotation>("expected token Quotation");
    operators::temp_ignored_ops.insert(Token::Type::Equiv);

    auto start = tokenizer_.last_location();
    equation.pattern = gen_expr();
    operators::temp_ignored_ops.erase(Token::Type::Equiv);
    eat<Token::Type::Equiv>("expected token Equiv");
    equation.expr = gen_expr();
    equation.raw_str = tokenizer_.file_content(start, tokenizer_.last_location());

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
    do {
        type->types.push_back(gen_pair_type());
    } while (try_eat<Token::Type::Rightarrow>());
    return type;
}

Ptr<Type> Parser::gen_pair_type() {
    auto type = gen_template_type();
    if (try_eat<Token::Type::Mul>()) {
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
     * TODO: check the type is types or not
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
            throw error("expected token type in { TypeVariable, Identifier, LParen, Quotation }");
    }
}

Ptr<ArgumentType> Parser::gen_argument_type() {
    check<Token::Type::TypeVariable>("expected token TypeVariable");
    auto type = make_unique<ArgumentType>(current_token_.value);
    get_next_token();

    // pass the type requirement
    if (try_eat<Token::Type::Colonn>()) {
        eat<Token::Type::Identifier>("expected token Identifier");
    }

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
    do {
        exprs.push_back(gen_expr());
    } while (try_eat<Token::Type::Comma>());

    return exprs;
}

Ptr<Expr> Parser::gen_expr(size_t layer) {
    if (layer == operators::bop_precedences.size()) {
        return gen_term();
    }

    auto lhs = gen_expr(layer + 1);
    auto op = current_token_;
    while (!operators::temp_ignored_ops.count(op.type)
        && operators::bops_at_layer(layer).count(op.type)) {
        get_next_token();

        auto is_right_associative = operators::bop_right_associativity.count(op.type);
        auto rhs = is_right_associative
            ? gen_expr(layer)
            : gen_expr(layer + 1)
        ;

        if (operators::bop_func_mapping.count(op.type)) {
            lhs = make_unique<ConsExpr>(operators::bop_func_mapping[op.type], move(lhs), move(rhs));
        } else {
            lhs = make_unique<BinaryOpExpr>(op, move(lhs), move(rhs));
        }

        if (is_right_associative) {
            return lhs;
        }
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
        case Token::Type::Let:
            return gen_letin();
        case Token::Type::Case:
            return gen_case();
        case Token::Type::Lambda:
            return gen_lambda();
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
            throw error("expected token in { Identifier, If, LBracket, LBrace, LParen, Integer }");
    }
}

Equation Parser::gen_letin_equation() {
    Equation equation;

    equation.pattern = gen_factor();
    eat<Token::Type::Equiv>("expected token Equiv");
    equation.expr = gen_expr();

    return equation;
}

Ptr<Expr> Parser::gen_letin() {
    eat<Token::Type::Let>("expected token Let");
    auto letin_expr = make_unique<LetinExpr>(gen_letin_equation());

    eat<Token::Type::KeyIn>("expected token KeyIn");
    letin_expr->expr = gen_expr();

    return letin_expr;
}

Equation Parser::gen_case_equation() {
    Equation equation;

    auto start = tokenizer_.last_location();
    equation.pattern = gen_expr();
    eat<Token::Type::Rightarrow>("expected token Rightarrow");
    equation.expr = gen_expr();
    equation.raw_str = tokenizer_.file_content(start, tokenizer_.last_location());

    return equation;
}

Ptr<Expr> Parser::gen_case() {
    eat<Token::Type::Case>("expected token Case");
    auto case_expr = make_unique<CaseExpr>(gen_expr());

    eat<Token::Type::Of>("expected token Of");
    do {
        case_expr->equations.push_back(gen_case_equation());
    } while (try_eat<Token::Type::Pipe>());

    return case_expr;
}

std::string Parser::gen_ident_str() {
    check<Token::Type::Identifier>("expected token Identifier");
    auto res = current_token_.value;
    get_next_token();
    return res;
}

Ptr<Expr> Parser::gen_lambda() {
    eat<Token::Type::Lambda>("expected token Lambda");
    auto lambda_expr = make_unique<LambdaExpr>();

    if (try_eat<Token::Type::LParen>()) {
        do {
            lambda_expr->parameters.push_back(gen_ident_str());
        } while (try_eat<Token::Type::Comma>());
        eat<Token::Type::RParen>("expected token RParen");
    } else {
        do {
            lambda_expr->parameters.push_back(gen_ident_str());
        } while (meet<Token::Type::Identifier>());
    }

    eat<Token::Type::Dot>("expected token Dot");
    lambda_expr->expr = gen_expr();

    return lambda_expr;
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
    auto list_expr = make_unique<ListExpr>();

    if (try_eat<Token::Type::RBracket>()) {
        return make_unique<VarExpr>("Nil"s);
    }

    auto expr = gen_expr();
    if (meet<Token::Type::Doot, Token::Type::DootLt>()) {
        auto op = current_token_.type;
        get_next_token();
        auto res = make_unique<ConsExpr>(op == Token::Type::Doot ? "upto"s : "upt"s,
            move(expr), gen_expr()
        );
        eat<Token::Type::RBracket>("expected token RBracket");
        return res;
    }

    list_expr->exprs.push_back(move(expr));
    while (try_eat<Token::Type::Comma>()) {
        list_expr->exprs.push_back(gen_expr());
    }
    eat<Token::Type::RBracket>("expected token RBracket");
    return list_expr;
}

Ptr<Expr> Parser::gen_set() {
    eat<Token::Type::LBrace>("expected token LBrace");
    if (try_eat<Token::Type::RBrace>()) {
        return make_unique<SetExpr>();
    }
    auto expr = make_unique<SetExpr>(gen_exprs());
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
    if (try_eat<Token::Type::Comma>()) {
        auto cons_expr = make_unique<ConsExpr>("Pair"s);
        cons_expr->args.push_back(move(expr));
        cons_expr->args.push_back(gen_pair_helper());

        return cons_expr;
    }
    return expr;
}

Ptr<Expr> Parser::gen_integral() {
    check<Token::Type::Integer>("expected token Integer");
    auto expr = make_unique<IntegralExpr>(current_token_.value);
    get_next_token();
    return expr;
}

ParseError Parser::error(const string &message) const {
    return ParseError(tokenizer_.get_err_info(message));
}
} // namespace hol2cpp
