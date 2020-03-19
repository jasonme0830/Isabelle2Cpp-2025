#pragma once

#include <string>
#include <vector>

namespace hol2cpp
{
struct AST
{
    virtual ~AST() = 0;
};

struct FunDecl : AST
{
    virtual ~FunDecl() = default;

};
}
