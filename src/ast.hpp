#pragma once

#include <string>
#include <vector>
#include <memory>

namespace hol2cpp {
struct AST {
    virtual ~AST() = 0;
};

struct FunctionDecl : AST {
    virtual ~FunctionDecl() = default;
};

struct Type : AST {
    virtual ~Type() = default;
};

struct NormalType : Type {
    std::string name;
    NormalType(std::string);
    virtual ~NormalType() = default;
};

struct ArgumentType : Type {
    std::string name;
    ArgumentType(std::string name);
    virtual ~ArgumentType() = default;
};

struct TemplateType : Type {
    std::string name;
    std::unique_ptr<Type> arg;
    TemplateType(std::string name,
      std::unique_ptr<Type> arg);
    virtual ~TemplateType() = default;
};

struct FunctionType : Type {
    std::vector<std::unique_ptr<Type>> types;
    FunctionType(std::vector<std::unique_ptr<Type>> types);
    virtual ~FunctionType() = default;
};

struct Expr : AST {
    virtual ~Expr() = default;
};

struct VarExpr : Expr {
    std::string name;

    VarExpr(std::string name) : name(std::move(name)) {}
    virtual ~VarExpr() = default;
};

struct ConsExpr : Expr {
    ConsExpr(std::string constructor,
      std::vector<std::unique_ptr<Expr>> patterns);
    virtual ~ConsExpr() = default;
};

struct Equation : AST {
    Equation(std::unique_ptr<ConsExpr> pattern,
      std::unique_ptr<Expr> expr);
    virtual ~Equation() = default;
};
}
