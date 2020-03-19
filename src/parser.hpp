#pragma once

#include <memory>
#include <parsec.hpp>
#include "ast.hpp"

namespace hol2cpp
{
class Parser
{
  public:
    Parser();

    std::unique_ptr<FunDecl>
    pas_fun_decl();

  private:
    parsec::Parsec<std::unique_ptr<FunDecl>>
    FunDeclParsec;
};
}
