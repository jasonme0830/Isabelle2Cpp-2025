#pragma once

#include <memory>
#include <string>
#include <vector>
#include <parsec.hpp>

#include "ast.hpp"

namespace hol2cpp
{
using parsec::Parsec;

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
    Parsec<char>                        digit_,
                                        alpha_,
                                        blank_,
                                        blanks_;

    Parsec<std::string>                 identifier_;
    Parsec<std::string>                 func_decl_name_;

    Parsec<Ptr<Type>>                   type_term_;
    Parsec<std::vector<Ptr<Type>>>      func_type_tail_;
    Parsec<Ptr<FuncType>>               func_type_;
    Parsec<Ptr<Type>>                   type_;
    Parsec<Ptr<FuncType>>               func_decl_type_;

    Parsec<Ptr<Expr>> miniterm_;
    Parsec<std::vector<Ptr<Expr>>>      miniterms_;
    Parsec<Ptr<ConsExpr>>               cons_term_;
    Parsec<Ptr<VarExpr>>                var_term_;
    Parsec<Ptr<Expr>>                   term_;
    Parsec<Ptr<Expr>>                   expr_;

    Parsec<Ptr<Equation>>               func_decl_equation_;
    Parsec<std::vector<Ptr<Equation>>>  func_decl_equations_;

    Parsec<Ptr<FuncDecl>>               func_decl_;
    Parsec<std::vector<Ptr<FuncDecl>>>  func_decls_;
};
} // namespace hol2cpp
