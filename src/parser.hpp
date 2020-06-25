#pragma once

#include <memory>
#include <string>
#include <vector>
#include <parsec.hpp>
#include "ast.hpp"

namespace hol2cpp
{
using parsec::Parsec;

/**
 * parser receive source code
 *  then return the ast node(s)
*/
class Parser
{
  public:
    /**
     * default ctor
    */
    Parser();

    /**
     * @str: source code contains multi declarations
     * return FuncDecl nodes
    */
    std::vector<Ptr<Declaration>>
    pas_decls(const std::string &str);

    Ptr<DataTypeDecl>
    pas_data_type_decl(const std::string &str);

    /**
     * @str: source code contains only one declaration
     * return FuncDecl node
    */
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

    Parsec<Ptr<Expr>>                   miniterm_;
    Parsec<std::vector<Ptr<Expr>>>      miniterms_;
    Parsec<Ptr<Expr>>                   ifelse_term_;
    Parsec<Ptr<ConsExpr>>               cons_term_;
    Parsec<Ptr<Expr>>                   var_term_;
    Parsec<Ptr<Expr>>                   list_term_;
    Parsec<Ptr<Expr>>                   set_term_;
    Parsec<Ptr<Expr>>                   pair_term_;

    Parsec<Ptr<Expr>>                   term_;
    Parsec<Ptr<Expr>>                   expr9_;
    Parsec<Ptr<BinaryOpTailExpr>>       expr8_tail_;
    Parsec<Ptr<Expr>>                   expr8_;
    Parsec<Ptr<BinaryOpTailExpr>>       expr7_tail_;
    Parsec<Ptr<Expr>>                   expr7_;
    Parsec<Ptr<BinaryOpTailExpr>>       expr6_tail_;
    Parsec<Ptr<Expr>>                   expr6_;
    Parsec<Ptr<BinaryOpTailExpr>>       expr5_tail_;
    Parsec<Ptr<Expr>>                   expr5_;
    Parsec<Ptr<Expr>>                   expr4_;
    Parsec<Ptr<Expr>>                   expr3_;
    Parsec<Ptr<BinaryOpTailExpr>>       expr2_tail_;
    Parsec<Ptr<Expr>>                   expr2_;
    Parsec<Ptr<Expr>>                   expr1_;
    Parsec<Ptr<Expr>>                   expr_;
    Parsec<std::vector<Ptr<Expr>>>      exprs_;

    Parsec<Ptr<Equation>>               func_decl_equation_;
    Parsec<std::vector<Ptr<Equation>>>  func_decl_equations_;

    Parsec<Ptr<FuncDecl>>               func_decl_;

    Parsec<std::vector<Ptr<Type>>>      types_;
    Parsec<DataTypeDecl::Component>     dtdecl_component_;
    Parsec<std::vector<DataTypeDecl::Component>> dtdecl_components_;

    Parsec<Ptr<DataTypeDecl>>           datatype_decl_;

    Parsec<std::vector<Ptr<Declaration>>> decls_;
};
} // namespace hol2cpp
