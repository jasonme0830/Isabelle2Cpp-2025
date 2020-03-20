#pragma once

#include <memory>
#include <string>
#include <parsec.hpp>
#include "ast.hpp"

namespace hol2cpp
{
class Parser
{
  public:
    Parser();

    std::unique_ptr<FunDecl>
    pas_fun_decl(const std::string &str);

  private:
    Parsec<char> digit_, alpha_, blank_;
    Parsec<std::string> blanks_;
    Parsec<std::string> identifier_;
    parsec::Parsec<std::unique_ptr<FunDecl>> fun_decl_;
};
}
