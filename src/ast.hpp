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

struct Type : AST
{
    virtual ~Type() = default;
};

struct Pattern : AST
{
    virtual ~Pattern() = default;
};
}
