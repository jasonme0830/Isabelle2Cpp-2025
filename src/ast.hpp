#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <cassert>

namespace hol2cpp
{
class Code;
class FuncEntity;

template<typename T>
using Ptr = std::unique_ptr<T>;

struct AST
{
    virtual ~AST() = 0;
};

struct Type : AST
{
    virtual ~Type() = 0;
    virtual std::string gen_typeinfo(FuncEntity &entity) const = 0;
};

struct NormalType final : Type
{
    std::string name;

    NormalType(std::string name)
      : name(std::move(name)) {}

    std::string gen_typeinfo(FuncEntity &entity) const override;
};

struct ArgumentType final : Type
{
    std::string name;

    ArgumentType(std::string name)
      : name(std::move(name)) {}

    std::string gen_typeinfo(FuncEntity &entity) const override;
};

struct TemplateType final : Type
{
    std::string name;
    Ptr<Type> arg;

    TemplateType(std::string name, Ptr<Type> &&arg)
      : name(std::move(name)), arg(std::move(arg)) {}

    std::string gen_typeinfo(FuncEntity &entity) const override;
};

struct FuncType final : Type
{
    std::vector<Ptr<Type>> types;

    FuncType(std::vector<Ptr<Type>> &&types)
      : types(std::move(types)) {}

    Type* result_type() const
    {
        assert(!types.empty());
        return types.back().get();
    }

    void build_entity(FuncEntity &entity) const;
    std::string gen_typeinfo(FuncEntity &entity) const override;
};

struct Expr : AST
{
    virtual ~Expr() = 0;

    virtual void
    gen_pattern(FuncEntity &entity, const std::string &prev)
    const = 0;

    virtual std::string
    gen_expr(FuncEntity &entity, const std::string &type)
    const = 0;
};

struct VarExpr final : Expr
{
    std::string name;

    VarExpr(std::string name)
      : name(std::move(name)) {}

    void
    gen_pattern(FuncEntity &entity, const std::string &prev)
    const override;

    std::string
    gen_expr(FuncEntity &entity, const std::string &type)
    const override;
};

struct ConsExpr final : Expr
{
    std::string constructor;
    std::vector<Ptr<Expr>> args;

    ConsExpr(std::string constructor,
        std::vector<Ptr<Expr>> &&args)
      : constructor(std::move(constructor))
      , args(std::move(args)) {}

    void
    gen_pattern(FuncEntity &entity, const std::string &prev)
    const override;

    std::string
    gen_expr(FuncEntity &entity, const std::string &type)
    const override;
};

struct ListExpr final : Expr
{
    std::vector<Ptr<Expr>> exprs;

    ListExpr(std::vector<Ptr<Expr>> &&exprs)
      : exprs(std::move(exprs)) {}

    void
    gen_pattern(FuncEntity &entity, const std::string &prev)
    const override;

    std::string
    gen_expr(FuncEntity &entity, const std::string &type)
    const override;
};

enum class BOp
{
    LogicAnd,   // \<and>
    LogicOr,    // \<or>
    LogicEq,    // =
    LogicNoteq, // \<noteq>

    OrderLe, // \<le>
    OrderLt, // <
    OrderGe, // \<ge>
    OrderGt, // >

    SetInter,    // \<inter>
    SetUnion,    // \<union>
    SetSubseteq, // \<subseteq>
    SetSubset,   // \<subset>
    SetSupseteq, // \<supseteq>
    SetSupset,   // \<supset>
    SetIn,       // \<in>
    SetNotin,    // \<notin>

    NumAdd, // +
    NumSub, // -
    NumMul, // *
    NumDiv, // / div
    NumMod, // mod
    NumPow, // ^

    ListApp,  // @
};

struct BinaryOpExpr final : Expr
{
    BOp op;
    Ptr<Expr> lhs, rhs;

    BinaryOpExpr(BOp op, Ptr<Expr> &&lhs, Ptr<Expr> &&rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    void
    gen_pattern(FuncEntity &entity, const std::string &prev)
    const override;

    std::string
    gen_expr(FuncEntity &entity, const std::string &type)
    const override;
};

struct BinaryOpTailExpr final
{
    BOp op;
    Ptr<Expr> expr;
    Ptr<BinaryOpTailExpr> tail;

    BinaryOpTailExpr(BOp op, Ptr<Expr> &&expr,
        Ptr<BinaryOpTailExpr> &&tail)
      : op(op), expr(std::move(expr))
      , tail(std::move(tail)) {}
};

struct Equation final : AST
{
    Ptr<ConsExpr> pattern;
    Ptr<Expr> expr;

    Equation(Ptr<ConsExpr> &&pattern, Ptr<Expr> &&expr)
      : pattern(std::move(pattern)), expr(std::move(expr)) {}

    void build_entity(FuncEntity &entity) const;
};

struct FuncDecl final : AST
{
    std::string name;
    Ptr<FuncType> type;
    std::vector<Ptr<Equation>> equations;

    FuncDecl(std::string name,
        Ptr<FuncType> &&type,
        std::vector<Ptr<Equation>> &&equations)
      : name(std::move(name)), type(std::move(type))
      , equations(std::move(equations)) {}

    void build_entity(FuncEntity &entity) const;
};
} // namespace hol2cpp
