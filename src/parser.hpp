#pragma once

#include <memory>
#include <string>
#include <vector>
#include <parsec.hpp>

#include "ast.hpp"

namespace hol2cpp
{
class Parser
{
  public:
    Parser();

    std::vector<Ptr<FuncDecl>>
    pas_func_decls(const std::string &str);

    Ptr<FuncDecl>
    pas_func_decl(const std::string &str);

#ifndef _DEBUG
  private:
#endif
    parsec::Parsec<char> digit_, alpha_, blank_, blanks_;

    parsec::Parsec<std::string> identifier_;
    parsec::Parsec<std::string> func_decl_name_;

    parsec::Parsec<Ptr<Type>>               type_term_;
    parsec::Parsec<std::vector<Ptr<Type>>>  func_type_tail_;
    parsec::Parsec<Ptr<FuncType>>           func_type_;
    parsec::Parsec<Ptr<Type>>               type_;
    parsec::Parsec<Ptr<FuncType>>           func_decl_type_;

    parsec::Parsec<Ptr<Expr>> miniterm_;
    parsec::Parsec<std::vector<Ptr<Expr>>>  miniterms_;
    parsec::Parsec<Ptr<ConsExpr>>           cons_term_;
    parsec::Parsec<Ptr<VarExpr>>            var_term_;
    parsec::Parsec<Ptr<Expr>>               term_;
    parsec::Parsec<Ptr<Expr>>               expr_;

    parsec::Parsec<Ptr<Equation>>               func_decl_equation_;
    parsec::Parsec<std::vector<Ptr<Equation>>>  func_decl_equations_;

    parsec::Parsec<Ptr<FuncDecl>>               func_decl_;
    parsec::Parsec<std::vector<Ptr<FuncDecl>>>  func_decls_;
};
} // namespace hol2cpp
