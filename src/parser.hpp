#pragma once

#include <memory>
#include <parsec.hpp>
#include <string>
#include <vector>

#include "ast.hpp"

namespace hol2cpp
{
class Parser
{
  public:
    Parser();

    std::vector<std::unique_ptr<FuncDecl>>
    pas_func_decls(const std::string &str);

    std::unique_ptr<FuncDecl>
    pas_func_decl(const std::string &str);

#ifndef _DEBUG
  private:
#endif
    parsec::Parsec<char> digit_, alpha_, blank_, blanks_;

    parsec::Parsec<std::string> identifier_;
    parsec::Parsec<std::string> func_decl_name_;

    parsec::Parsec<std::unique_ptr<Type>>               type_term_;
    parsec::Parsec<std::vector<std::unique_ptr<Type>>>  func_type_tail_;
    parsec::Parsec<std::unique_ptr<FuncType>>           func_type_;
    parsec::Parsec<std::unique_ptr<Type>>               type_;
    parsec::Parsec<std::unique_ptr<FuncType>>           func_decl_type_;

    parsec::Parsec<std::unique_ptr<Expr>> miniterm_;
    parsec::Parsec<std::vector<std::unique_ptr<Expr>>>  miniterms_;
    parsec::Parsec<std::unique_ptr<ConsExpr>>           cons_term_;
    parsec::Parsec<std::unique_ptr<VarExpr>>            var_term_;
    parsec::Parsec<std::unique_ptr<Expr>>               term_;
    parsec::Parsec<std::unique_ptr<Expr>>               expr_;

    parsec::Parsec<std::unique_ptr<Equation>>               func_decl_equation_;
    parsec::Parsec<std::vector<std::unique_ptr<Equation>>>  func_decl_equations_;

    parsec::Parsec<std::unique_ptr<FuncDecl>>               func_decl_;
    parsec::Parsec<std::vector<std::unique_ptr<FuncDecl>>>  func_decls_;
};
} // namespace hol2cpp
