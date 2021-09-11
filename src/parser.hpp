#pragma once

#include "ast.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

#include <fstream>
#include <exception>

namespace hol2cpp {
class Parser {
  public:
    Parser(std::ifstream &input) noexcept;

    Theory gen_theory();

  private:
    template<Token::Type type, Token::Type ...types>
    void check(const std::string &err_info = "") {
        if (current_token_.type != type) {
            if constexpr (sizeof...(types) > 0) {
                check<types...>(err_info);
            } else {
                throw std::runtime_error(err_info);
            }
        }
    }

    template<Token::Type ...types>
    void eat(const std::string &err_info = "") {
        check<types...>(err_info);
        get_next_token();
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


    Ptr<Declaration> gen_declaration();
    Ptr<DataTypeDecl> gen_datatype_declaration();
        DataTypeDecl::Component gen_component();
    Ptr<FuncDecl> gen_function_declaration();
        Equation gen_equation();

    Ptr<Type> gen_type();
    Ptr<FuncType> gen_func_type();
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
