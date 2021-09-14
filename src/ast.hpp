#pragma once

#include "token.hpp"
#include "error.hpp"
#include "funcentity.hpp"

#include <map>
#include <memory>
#include <vector>
#include <functional>

namespace hol2cpp
{
class Code;
class DataType;
class FuncEntity;

template<typename T>
using Ptr = std::unique_ptr<T>;

/**
 * base class for all types
 *  types generate concrete type as 'std::uint64_t'
*/
struct Type {
    virtual ~Type() = 0;

    /**
     * receive the related function entity and return the type infomation
     * @entity: generate type infomation related with the given entity
    */
    virtual TypeInfo gen_typeinfo(FuncEntity &entity) const = 0;
    virtual std::string build_data_type(DataType &) const = 0;
    virtual std::string main_name() const;
    virtual TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const = 0;
};

/**
 * class for normal type
 *  normal type is neither 't nor list in 't list
 *  it is a concrete type like nat
*/
struct NormalType final : Type {
    std::string name;

    /**
     * @name: name of concrete type
    */
    NormalType(std::string name) : name(std::move(name)) {}

    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    std::string main_name() const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;
};

/**
 * argument type is the type variable in isabelle
 *  such as 't in 't list
*/
struct ArgumentType final : Type {
    std::string name;

    /**
     * @name: name of the argument type but without '
     *  for example, t is the name of 't
    */
    ArgumentType(std::string name) : name(std::move(name)) {}

    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;
};

/**
 * template type is the type which has type variable
 *  such as list in 't list
*/
struct TemplateType final : Type {
    std::string name;
    std::vector<Ptr<Type>> args;

    /**
     * @name: name of template type
     * @args: type arguments for this template type
     *  such 'a and 'b in ('a * 'b)
    */
    TemplateType(std::string name) : name(std::move(name)) {}
    TemplateType(std::string name, Ptr<Type> &&arg) : name(std::move(name)) {
        args.push_back(std::move(arg));
    }

    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    std::string main_name() const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;
};

struct ProductType;

/**
 * function type is defined with symbol =>
 *  such as 'a => 'b => ('a * 'b) will be translated to
 *  std::pair<T0, T1>(T0, T1)
*/
struct FuncType final : Type {
    std::vector<Ptr<Type>> types;

    /**
     * return the raw pointer of result type
    */
    Type *result_type() const;

    /**
     * build the binded func entity
    */
    void build_func_entity(FuncEntity &entity) const;

    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;
};

/**
 * base class for all exprs
*/
struct Expr {
    virtual ~Expr() = 0;

    /**
     * method to generate code when expr occurs as pattern
     * not return value, just generate statements in entity
    */
    virtual void gen_pattern(FuncEntity &entity, const std::string &prev) const;

    /**
     * method to generate code when expr occurs as the expression to return
     * return the expression and generate statements when needed
    */
    virtual std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const = 0;
};

struct IntegralExpr final : Expr {
    std::string value;

    IntegralExpr(std::string value) : value(std::move(value)) {}

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

/**
 * single identifier or numeral literal
 *  without arguments
*/
struct VarExpr final : Expr {
    std::string name;

    /**
     * @name: value of VarExpr
     *  single identifier or numeral literal
    */
    VarExpr(std::string name) : name(std::move(name)) {}

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

/**
 * constructor with arguments
 *  for example, Cons is the constructor
 *   and x and xs are arguments in Cons x xs
*/
struct ConsExpr final : Expr {
    std::string constructor;
    std::vector<Ptr<Expr>> args;

    /**
     * @constructor: name of constructor
     * @args: arguments for the constructor
    */
    ConsExpr(std::string constructor) : constructor(std::move(constructor)) {}

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

/**
 * ListExpr describes expression like [...]
 *  exprs are splited by comma in []
*/
struct ListExpr final : Expr {
    std::vector<Ptr<Expr>> exprs;

    /**
     * @exprs: exprs in []
    */
    ListExpr() = default;
    ListExpr(std::vector<Ptr<Expr>> &&exprs) : exprs(std::move(exprs)) {}

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

/**
 * SetExpr describes expression like {...}
 *  exprs are splited by comma in {}
*/
struct SetExpr final : Expr {
    std::vector<Ptr<Expr>> exprs;

    /**
     * @exprs: exprs in {}
    */
    SetExpr() = default;
    SetExpr(std::vector<Ptr<Expr>> &&exprs) : exprs(std::move(exprs)) {}

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

struct BinaryOpExpr final : Expr {
    Token op;
    mutable Ptr<Expr> lhs, rhs;

    BinaryOpExpr(Token op, Ptr<Expr> &&lhs, Ptr<Expr> &&rhs)
      : op(std::move(op)) , lhs(std::move(lhs)), rhs(std::move(rhs)) {
        // ...
    }

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

struct Equation final {
    Ptr<Expr> pattern;
    Ptr<Expr> expr;

    void build_func_entity(FuncEntity &entity) const;
};

struct Declaration {
    virtual ~Declaration() = 0;
    virtual bool is_datatype_decl() const = 0;
    virtual void codegen(Code &) const = 0;
};

/**
 * struct for the declaration of new datatype
 * datatype 'a option = None | Some 'a
 * datatype 'a list = Nil | Cons 'a "'a list"
 *
 * use union to contain the type components
 *  constructor without arguments won't be declared
 *  constructor with argument(s) will be declared by its position
 *
 * use enum type to determine which Constructor
*/
struct DataTypeDecl : Declaration {
    /**
     * Component likes ConsExpr
     *  but arguments are types but not expressions
    */
    struct Component {
        std::string constructor;
        std::vector<Ptr<Type>> arguments;
    };

    Ptr<Type> decl_type;
    // mutable for codegen
    mutable std::vector<Component> components;

    virtual bool is_datatype_decl() const override { return true; }
    void codegen(Code &) const override;
};

struct FuncDecl final : Declaration {
    std::string name;
    Ptr<FuncType> type;
    std::vector<Equation> equations;

    virtual bool is_datatype_decl() const override { return false; }
    void codegen(Code &) const override;
};

struct Theory final {
    std::string name;
    std::vector<std::string> imports;
    std::vector<Ptr<Declaration>> declarations;

    void codegen(Code &) const;
};
} // namespace hol2cpp
