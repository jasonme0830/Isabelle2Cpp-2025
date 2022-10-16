#pragma once

#include "token.hpp"
#include "tokenizer.hpp"
#include "../ir/ast.hpp"
#include "../utility/error.hpp"

#include <fstream>

namespace hol2cpp {
class Parser {
  public:
    static void add_infix_op(Token::Type, std::size_t,
        const std::string &, bool is_infixl
    );

  public:
    Parser(std::ifstream &input, std::string name) noexcept;

    Theory gen_predef();
    Theory gen_theory(Theory predef);

  private:
    ParseError error(const std::string &message) const;

    template<Token::Type type, Token::Type ...types>
    void check(const std::string &err_info) {
        if (current_token_.type != type) {
            if constexpr (sizeof...(types) > 0) {
                check<types...>(err_info);
            } else {
                throw error(err_info);
            }
        }
    }

    template<Token::Type ...types>
    void eat(const std::string &err_info) {
        check<types...>(err_info);
        get_next_token();
    }

    template<Token::Type type, Token::Type ...types>
    bool try_eat() {
        if (current_token_.type == type) {
            get_next_token();
            return true;
        }

        if constexpr (sizeof...(types) > 0) {
            return try_eat<types...>();
        } else {
            return false;
        }
    }

    template<Token::Type type, Token::Type ...types>
    bool meet() {
        if (current_token_.type == type) {
            return true;
        }

        if constexpr (sizeof...(types) > 0) {
            return meet<types...>();
        } else {
            return false;
        }
    }

    template<Token::Type ...types>
    void eat_until() {
        while (!meet<Token::Type::EndOfFile, types...>()) {
            get_next_token();
        }
    }

    void get_next_token();
    Token &next_token();
    std::string next_raw_str();

    Ptr<Definition> gen_predef_definition();
    Ptr<Definition> gen_predef_datatype_definition();
    Ptr<Definition> gen_predef_function_definition();

    Ptr<Definition> gen_declaration();
    Ptr<DatatypeDef> gen_datatype_definition();
        DatatypeDef::Component gen_component();
    Ptr<Definition> gen_function_definition();
        Equation gen_equation();

    Ptr<Type> gen_type();
    Ptr<FuncType> gen_func_type();
    Ptr<Type> gen_complex_type();
    Ptr<Type> gen_pair_type();
    Ptr<Type> gen_template_type();
    Ptr<Type> gen_type_term();
    Ptr<ArgumentType> gen_argument_type();
    Ptr<NormalType> gen_normal_type();

    std::vector<Ptr<Expr>> gen_exprs();
    Ptr<Expr> gen_expr(std::size_t layer = 0);
    Ptr<Expr> gen_term();
    Ptr<Expr> gen_construction();
    Ptr<Expr> gen_factor();
    Ptr<Expr> gen_letin();
        Equation gen_letin_equation();
    Ptr<Expr> gen_case();
        Equation gen_case_equation();
    Ptr<Expr> gen_lambda();
        std::string gen_ident_str();
    Ptr<Expr> gen_var();
    Ptr<Expr> gen_ifelse();
    Ptr<Expr> gen_list();
    Ptr<Expr> gen_set();
    Ptr<Expr> gen_pair();
        Ptr<Expr> gen_pair_helper();
    Ptr<Expr> gen_integral();

  private:
    Tokenizer tokenizer_;
    Token current_token_;
};
} // namespace hol2cpp
