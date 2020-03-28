#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

namespace hol2cpp {
class Code;
class FuncEntity;

struct AST {
    virtual ~AST() = 0;
};

struct Type : AST {
    virtual ~Type() = 0;
    virtual void gen_typeinfo(FuncEntity &entity) const = 0;
};

struct NormalType final : Type {
    std::string name;

    NormalType(std::string name)
      : name(std::move(name)) {}

    void gen_typeinfo(FuncEntity &entity) const override;
};

struct ArgumentType final : Type {
    std::string name;

    ArgumentType(std::string name)
      : name(std::move(name)) {}

    void gen_typeinfo(FuncEntity &entity) const override;
};

struct TemplateType final : Type {
    std::string name;
    std::unique_ptr<Type> arg;

    TemplateType(std::string name,
      std::unique_ptr<Type> arg)
      : name(std::move(name)),
        arg(std::move(arg)) {}

    void gen_typeinfo(FuncEntity &entity) const override;
};

struct FuncType final : Type {
    std::vector<std::unique_ptr<Type>> types;

    FuncType(std::vector<std::unique_ptr<Type>> types)
      : types(std::move(types)) {}

    Type *result_type() const {
      assert(!types.empty());
      return types.back().get();
    }

    void gen_typeinfo(FuncEntity &entity) const;
};

struct Expr : AST {
    virtual ~Expr() = 0;
    virtual void codegen(Code &code) const = 0;
};

struct VarExpr final : Expr {
    std::string name;

    VarExpr(std::string name)
      : name(std::move(name)) {}

    void codegen(Code &code) const override;
};

struct ConsExpr final : Expr {
    std::string constructor;
    std::vector<std::unique_ptr<Expr>> args;

    ConsExpr(std::string constructor,
      std::vector<std::unique_ptr<Expr>> args)
      : constructor(std::move(constructor)),
        args(std::move(args)) {}

    void codegen(Code &code) const override;
};

struct Equation final : AST {
    std::unique_ptr<ConsExpr> pattern;
    std::unique_ptr<Expr> expr;

    Equation(std::unique_ptr<ConsExpr> pattern,
      std::unique_ptr<Expr> expr)
      : pattern(std::move(pattern)),
        expr(std::move(expr)) {}

    void codegen(Code &code) const;
};

struct FuncDecl final : AST {
    std::string name;
    std::unique_ptr<FuncType> type;
    std::vector<std::unique_ptr<Equation>> equations;

    FuncDecl(std::string name,
      std::unique_ptr<FuncType> type,
      std::vector<std::unique_ptr<Equation>> equations)
      : name(std::move(name)),
        type(std::move(type)),
        equations(std::move(equations)) {}

    void codegen(Code &code) const;
};
}
