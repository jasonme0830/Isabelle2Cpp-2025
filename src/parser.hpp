#pragma once

#include <memory>
#include <vector>
#include <string>
#include <parsec.hpp>
#include "ast.hpp"

namespace hol2cpp {
class Parser {
  public:
    Parser();

    std::vector<std::unique_ptr<FunDecl>>
    pas_fun_decls(const std::string &str);

    std::unique_ptr<FunDecl>
    pas_fun_decl(const std::string &str);

  private:
    parsec::Parsec<char> digit_, alpha_, blank_, blanks_;

    parsec::Parsec<std::string> identifier_;
    parsec::Parsec<std::string> fun_decl_name_;

    parsec::Parsec<std::unique_ptr<Type>> type_;
    parsec::Parsec<std::unique_ptr<Type>> fun_decl_type_;

    parsec::Parsec<std::unique_ptr<Expr>> miniterm_;
    parsec::Parsec<std::vector<std::unique_ptr<Expr>>> miniterms_;
    parsec::Parsec<std::unique_ptr<ConsExpr>> cons_term_;
    parsec::Parsec<std::unique_ptr<VarExpr>> var_term_;
    parsec::Parsec<std::unique_ptr<Expr>> term_;
    parsec::Parsec<std::unique_ptr<Expr>> expr_;

    parsec::Parsec<std::unique_ptr<Equation>> fun_decl_equation_;
    parsec::Parsec<std::vector<std::unique_ptr<Equation>>> fun_decl_equations_;

    parsec::Parsec<std::unique_ptr<FunDecl>> fun_decl_;
    parsec::Parsec<std::vector<std::unique_ptr<FunDecl>>> fun_decls_;
};
}
