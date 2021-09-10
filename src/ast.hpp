#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <cassert>
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
 * types generate concrete type as 'std::uint64_t'
*/
struct Type {
    virtual ~Type() = 0;

    /**
     * receive the related function entity and return the type infomation
     * @entity: generate type infomation related with the given entity
     *
    */
    virtual std::string gen_typeinfo(FuncEntity &entity) const = 0;
    virtual std::string build_data_type(DataType &) const = 0;
    virtual std::string main_name() const;
    virtual std::string apply(std::function<std::string(std::string)> &trans) const = 0;
};

/**
 * class for normal type
 * normal type is neither 't nor list in 't list
 * it is a concrete type like nat
*/
struct NormalType final : Type {
    std::string name;

    /**
     * @name: name of concrete type
    */
    NormalType(std::string name) : name(std::move(name)) {}

    std::string gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    std::string main_name() const override;
    std::string apply(std::function<std::string(std::string)> &trans) const override;
};

/**
 * argument type is the type variable in isabelle
 * such as 't in 't list
*/
struct ArgumentType final : Type {
    std::string name;

    /**
     * @name: name of the argument type but without '
     *  for example, t is the name of 't
    */
    ArgumentType(std::string name) : name(std::move(name)) {}

    std::string gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    std::string apply(std::function<std::string(std::string)> &trans) const override;
};

/**
 * template type is the type which has type variable
 * such as list in 't list
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

    std::string gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    std::string main_name() const override;
    std::string apply(std::function<std::string(std::string)> &trans) const override;
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
    Type *result_type() const {
        assert(!types.empty());
        return types.back().get();
    }

    /**
     * build the binded entity
    */
    void build_func_entity(FuncEntity &entity) const;

    std::string gen_typeinfo(FuncEntity &entity) const override;
    std::string build_data_type(DataType &) const override;
    std::string apply(std::function<std::string(std::string)> &trans) const override;
};

/**
 * base class for all exprs
 * exprs can be captured as pattern or expr
 *  exprs as pattern will generate code by calling gen_pattern
 *  exprs as the return expression, such (x, y) in "func x y = (x, y)",
 *   will generate code by calling gen_expr
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
    virtual std::string gen_expr(FuncEntity &entity, const std::string &type) const = 0;
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
    std::string gen_expr(FuncEntity &entity, const std::string &type) const override;
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
    std::string gen_expr(FuncEntity &entity, const std::string &type) const override;
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
    ListExpr(std::vector<Ptr<Expr>> &&exprs) : exprs(std::move(exprs)) {}

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const std::string &type) const override;
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
    SetExpr(std::vector<Ptr<Expr>> &&exprs) : exprs(std::move(exprs)) {}

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const std::string &type) const override;
};

/**
 * enum class to indicate different binary operators
*/
enum class BOp {
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

/**
 * contains binary operator,
 *  lhs expression and rhs expression
*/
struct BinaryOpExpr final : Expr {
    BOp op;
    Ptr<Expr> lhs, rhs;

    /**
     * @op: enum which indicates the binary operator
     * @lhs: the lhs expression
     * @rhs: the rhs expression
    */
    BinaryOpExpr(BOp op, Ptr<Expr> &&lhs, Ptr<Expr> &&rhs)
      : op(op) , lhs(std::move(lhs)), rhs(std::move(rhs)) {
        // ...
    }

    void gen_pattern(FuncEntity &entity, const std::string &prev) const override;
    std::string gen_expr(FuncEntity &entity, const std::string &type) const override;
};

/**
 * helpful class for parsing left-associative binary operation
 *  contains its binded binary operator,
 *   current expression and the tail expression,
 *    such * b * c in a * b * c,
 *     * is the binded operator
 *      b is the current expression
 *       and * c is the tail expression
*/
struct BinaryOpTailExpr final {
    BOp op;
    Ptr<Expr> expr;
    Ptr<BinaryOpTailExpr> tail;

    /**
     * @op: binded binary operator
     * @expr: current expression
     * @tail: tail expression, maybe nullptr
    */
    BinaryOpTailExpr(BOp op, Ptr<Expr> &&expr, Ptr<BinaryOpTailExpr> &&tail)
      : op(op), expr(std::move(expr)), tail(std::move(tail)) {
        // ...
    }
};

/**
 * equation means each line in function declaration
 * for example, "fun x y = (x, y)" is an equation
*/
struct Equation final {
    Ptr<Expr> pattern;
    Ptr<Expr> expr;

    void build_func_entity(FuncEntity &entity) const;
};

struct Declaration {
    virtual ~Declaration() = 0;

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
 * use enum type to determine which Constructor
 *
 * literal identity of the decl_type will be stored after codegen
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
    mutable std::vector<Component> components; // mutable for codegen

    void codegen(Code &) const override;
};

/**
 * function declaration contains its name,
 *  the function type and its equations
*/
struct FuncDecl final : Declaration {
    std::string name;
    Ptr<FuncType> type;
    std::vector<Equation> equations;

    void codegen(Code &) const override;
};

struct Theory final {
    std::string name;
    std::vector<std::string> imports;
    std::vector<Ptr<Declaration>> declarations;
};
} // namespace hol2cpp
