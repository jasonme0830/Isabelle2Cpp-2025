#pragma once

#include <string>
#include <vector>
#include <memory>

namespace hol2cpp {
struct AST {
    virtual ~AST() = 0;
};

struct Type : AST {
    virtual ~Type() = 0;
};

struct NormalType final : Type {
    std::string name;

    NormalType(std::string)
      : name(std::move(name)) {}
};

struct ArgumentType final : Type {
    std::string name;

    ArgumentType(std::string name)
      : name(std::move(name)) {}
};

struct TemplateType final : Type {
    std::string name;
    std::unique_ptr<Type> arg;

    TemplateType(std::string name,
      std::unique_ptr<Type> arg)
      : name(std::move(name)),
        arg(std::move(arg)) {}
};

struct FuncType final : Type {
    std::vector<std::unique_ptr<Type>> types;

    FuncType(std::vector<std::unique_ptr<Type>> types)
      : types(std::move(types)) {}
};

struct Expr : AST {
    virtual ~Expr() = 0;
};

struct VarExpr final : Expr {
    std::string name;

    VarExpr(std::string name)
      : name(std::move(name)) {}
};

struct ConsExpr final : Expr {
    std::string constructor;
    std::vector<std::unique_ptr<Expr>> args;

    ConsExpr(std::string constructor,
      std::vector<std::unique_ptr<Expr>> args)
      : constructor(std::move(constructor)),
        args(std::move(args)) {}
};

struct Equation final : AST {
    std::unique_ptr<ConsExpr> pattern;
    std::unique_ptr<Expr> expr;

    Equation(std::unique_ptr<ConsExpr> pattern,
      std::unique_ptr<Expr> expr)
      : pattern(std::move(pattern)),
        expr(std::move(expr)) {}
};

struct FuncDecl final : AST {
    std::unique_ptr<FuncType> type;
    std::vector<std::unique_ptr<Equation>> equations;

    FuncDecl(std::unique_ptr<FuncType> type,
      std::vector<std::unique_ptr<Equation>> equations)
      : type(std::move(type)),
        equations(std::move(equations)) {}
};
}
