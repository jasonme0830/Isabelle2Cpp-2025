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

struct Expr : AST
{
    virtual ~Expr() = default;
};

struct Pattern : AST
{
    virtual ~Pattern() = default;
};

struct Equation : AST
{
    Equation(std::string ident,
        std::unique_ptr<Pattern> pattern,
        std::unique_ptr<Expr> expr);
    virtual ~Equation() = default;
};
}
