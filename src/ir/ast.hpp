#pragma once

#include "funcentity.hpp"
#include "../parser/token.hpp"
#include "../utility/error.hpp"

#include <map>
#include <memory>
#include <vector>
#include <functional>

namespace hol2cpp {

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
    virtual TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const = 0;

    virtual std::string gen_datatype(DataType &) const = 0;
    virtual std::string main_name() const;
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
    NormalType(std::string name);

public:
    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;

    std::string gen_datatype(DataType &) const override;
    std::string main_name() const override;
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
    ArgumentType(std::string name);

public:
    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;

    std::string gen_datatype(DataType &) const override;
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
    TemplateType(std::string name);
    TemplateType(std::string name, Ptr<Type> &&arg);

public:
    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;

    std::string gen_datatype(DataType &) const override;
    std::string main_name() const override;
};

struct ProductType;

/**
 * function type is defined with symbol =>
 *  such as 'a => 'b => ('a * 'b) will be translated to
 *  std::pair<T0, T1>(T0, T1)
*/
struct FuncType final : Type {
    std::vector<Ptr<Type>> types;

    Type *result_type() const;

public:
    void gen_funcentity(FuncEntity &entity) const;

    TypeInfo gen_typeinfo(FuncEntity &entity) const override;
    TypeInfo apply(std::function<TypeInfo(const std::string &)> &trans) const override;

    std::string gen_datatype(DataType &) const override;
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

    IntegralExpr(std::string value);

public:
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
    VarExpr(std::string name);

public:
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
    ConsExpr(std::string constructor);
    ConsExpr(std::string constructor, Ptr<Expr> lhs, Ptr<Expr> rhs);

public:
    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

/**
 * ListExpr describes expression like [...]
 *  exprs are splited by comma in []
*/
struct ListExpr final : Expr {
    std::vector<Ptr<Expr>> exprs;

public:
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
    SetExpr();
    SetExpr(std::vector<Ptr<Expr>> &&exprs);

public:
    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

struct BinaryOpExpr final : Expr {
    Token op;
    mutable Ptr<Expr> lhs, rhs;

    BinaryOpExpr(Token op, Ptr<Expr> &&lhs, Ptr<Expr> &&rhs);

public:
    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

struct Equation final {
    Ptr<Expr> pattern;
    Ptr<Expr> expr;
    // for helpful comment
    std::string raw_str;

public:
    void gen_funcentity(FuncEntity &entity) const;
};

struct LetinExpr final : Expr {
    Equation equation;
    Ptr<Expr> expr;

    LetinExpr(Equation &&equation);

public:
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

struct CaseExpr final : Expr {
    Ptr<Expr> expr;
    std::vector<Equation> equations;

    CaseExpr(Ptr<Expr> &&expr);

public:
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

struct LambdaExpr final : Expr {
    std::vector<std::string> parameters;
    Ptr<Expr> expr;

public:
    std::string gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const override;
};

struct Definition {
    virtual ~Definition() = 0;

    virtual std::string def_name() const;

    virtual bool is_error() const;

    virtual bool is_predefined() const;

    virtual bool is_datatype_decl() const;
    virtual bool is_function_decl() const;

    virtual void gen_code(Code &) const;
};

struct ErrorDefinition : Definition {
    PEType type;

    ErrorDefinition(PEType type);

    bool is_error() const override;
    bool is_datatype_decl() const override;
    bool is_function_decl() const override;
};

/**
 * struct for the definition of new datatype
 * datatype 'a option = None | Some 'a
 * datatype 'a list = Nil | Cons 'a "'a list"
 *
 * use union to contain the type components
 *  constructor without arguments won't be declared
 *  constructor with argument(s) will be declared by its position
 *
 * use enum type to determine which Constructor
*/
struct DataTypeDef : Definition {
    /**
     * Component likes ConsExpr
     *  but arguments are types but not expressions
    */
    struct Component {
        std::string constructor;
        std::vector<Ptr<Type>> arguments;
    };

    Ptr<Type> decl_type;
    // mutable for gen_code
    mutable std::vector<Component> components;

    std::string def_name() const override;

    bool is_predefined() const override;
    bool is_datatype_decl() const override;

public:
    void gen_code(Code &) const override;
};

struct FunctionDef final : Definition {
    std::string name;
    Ptr<FuncType> type;
    std::vector<Equation> equations;

    std::string def_name() const override;

    bool is_predefined() const override;
    bool is_function_decl() const override;

public:
    void gen_code(Code &) const override;
};

struct ShortDef final : Definition {
    std::string name;
    mutable std::vector<std::string> parameters;
    mutable Ptr<Expr> expr;

    ShortDef() = default;
    ShortDef(std::string name, std::vector<std::string> params, Ptr<Expr> expr);

    std::string def_name() const override;

public:
    void gen_code(Code &) const override;
};

struct Theory final {
    std::string name;
    std::vector<std::string> imports;
    std::vector<Ptr<Definition>> definitions;

public:
    void gen_code(Code &) const;
};
} // namespace hol2cpp
