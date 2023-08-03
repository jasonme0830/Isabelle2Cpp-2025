#pragma once

#include "../parser/token.hpp"
#include "../utility/error.hpp"
#include "funcentity.hpp"

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <vector>

namespace hol2cpp {

class Code;
class Datatype;
class FuncEntity;

template<typename T>
using Ptr = std::shared_ptr<T>;

/**
 * base class for all types
 *  types generate concrete type as 'std::uint64_t'
 */
struct Type
{
  virtual ~Type() = 0;
  /**
   * receive the related function func and return the type infomation
   * @func: generate type infomation related with the given func
   */
  virtual TypeInfo gen_typeinfo(FuncEntity& func) const = 0;
  // todo @xubo removes this method, not required after supporting type
  // inference
  virtual TypeInfo apply(
    std::function<TypeInfo(const std::string&)>& trans) const = 0;

  virtual std::string gen_datatype(Datatype&) const = 0;
  virtual std::string main_name() const;

  virtual Ptr<Type> clone() const;
  virtual void print_type() const = 0;

  //code by myk
  virtual std::string get_datatype() const = 0;
};

/**
 * class for normal type
 *  normal type is neither 't nor list in 't list
 *  it is a concrete type like nat
 */
struct NormalType final : Type
{
  std::string name;

  /**
   * @name: name of concrete type
   */
  NormalType(std::string name);

public:
  TypeInfo gen_typeinfo(FuncEntity& func) const override;
  TypeInfo apply(
    std::function<TypeInfo(const std::string&)>& trans) const override;

  std::string gen_datatype(Datatype&) const override;
  std::string main_name() const override;

  Ptr<Type> clone() const override;
  void print_type() const override;

  //code by myk
  std::string get_datatype() const override;
};

/**
 * argument type is the type variable in isabelle
 *  such as 't in 't list
 */
struct ArgumentType final : Type
{
  std::string name;

  /**
   * @name: name of the argument type but without '
   *  for example, t is the name of 't
   */
  ArgumentType(std::string name);

public:
  TypeInfo gen_typeinfo(FuncEntity& func) const override;
  TypeInfo apply(
    std::function<TypeInfo(const std::string&)>& trans) const override;

  std::string gen_datatype(Datatype&) const override;

  Ptr<Type> clone() const override;
  void print_type() const override;

  //code by myk
  std::string get_datatype() const override;
};

/**
 * template type is the type which has type variable
 *  such as list in 't list
 */
struct TemplateType final : Type
{
  std::string name;
  std::vector<Ptr<Type>> args;

  TemplateType() = default;
  /**
   * @name: name of template type
   * @args: type arguments for this template type
   *  such 'a and 'b in ('a * 'b)
   */
  TemplateType(std::string name);
  TemplateType(std::string name, Ptr<Type>&& arg);
  TemplateType(std::string name, std::vector<Ptr<Type>>&& args);

public:
  TypeInfo gen_typeinfo(FuncEntity& func) const override;
  TypeInfo apply(
    std::function<TypeInfo(const std::string&)>& trans) const override;

  std::string gen_datatype(Datatype&) const override;
  std::string main_name() const override;

  Ptr<Type> clone() const override;
  void print_type() const override;

  //code by myk
  std::string get_datatype() const override;
};

// unused now
struct ProductType;

/**
 * function type is defined with symbol =>
 *  such as 'a => 'b => ('a * 'b) will be translated to
 *  std::pair<T0, T1>(T0, T1)
 */
struct FuncType final : Type
{
  std::vector<Ptr<Type>> types;

  Type* result_type() const;
  FuncType() = default;

public:
  void gen_funcentity(FuncEntity& func) const;

  TypeInfo gen_typeinfo(FuncEntity& func) const override;
  TypeInfo apply(
    std::function<TypeInfo(const std::string&)>& trans) const override;

  std::string gen_datatype(Datatype&) const override;

  Ptr<Type> clone() const override;
  void print_type() const override;

  //code by myk
  std::string get_datatype() const override;
};

/**
 * base class for all exprs
 */
struct Expr
{
  Ptr<Type> expr_type;

  virtual ~Expr() = 0;

  /**
   * method to generate code when expr occurs as pattern
   * not return value, just generate statements in func
   */
  virtual void gen_pattern(FuncEntity& func, const std::string& prev) const;

  // todo @xubo records the typeinfo of the expr
  std::string gen_expr(FuncEntity& func) const
  {
    if (expr_type == nullptr) {
      return gen_expr_impl(func, TypeInfo());
    } else {
      return gen_expr_impl(func, expr_type->gen_typeinfo(func));
    }
  }
  /**
   * method to generate code when expr occurs as the expression to return
   * return the expression and generate statements when needed
   */
  virtual std::string gen_expr_impl(FuncEntity& func,
                                    const TypeInfo& typeinfo) const = 0;

  virtual void analyze_var_movable(
    std::set<std::string>& movables); // for move-list

  virtual void print_expr() const = 0;
  virtual void print_expr_type() const;
};

struct IntegralExpr final : Expr
{
  std::string value;

  IntegralExpr(std::string value);

public:
  void gen_pattern(FuncEntity& func, const std::string& prev) const override;
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;

  void print_expr() const override;
};

/**
 * single identifier or numeral literal
 *  without arguments
 */
struct VarExpr final : Expr
{
  std::string name;
  bool movable; // for move-list

  /**
   * @name: value of VarExpr
   *  single identifier or numeral literal
   */
  VarExpr(std::string name);

public:
  void gen_pattern(FuncEntity& func, const std::string& prev) const override;
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;
  void analyze_var_movable(std::set<std::string>& movables) override;

  void print_expr() const override;
};

/**
 * constructor with arguments
 *  for example, Cons is the constructor
 *   and x and xs are arguments in Cons x xs
 */
struct ConsExpr final : Expr
{
  std::string constructor;
  std::vector<Ptr<Expr>> args;

  /**
   * @constructor: name of constructor
   * @args: arguments for the constructor
   */
  ConsExpr(std::string constructor);
  ConsExpr(std::string constructor, Ptr<Expr> lhs, Ptr<Expr> rhs);

public:
  void gen_pattern(FuncEntity& func, const std::string& prev) const override;
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;
  void analyze_var_movable(std::set<std::string>& movables) override;

  void print_expr() const override;
};

/**
 * ListExpr describes expression like [...]
 *  exprs are splited by comma in []
 */
struct ListExpr final : Expr
{
  std::vector<Ptr<Expr>> exprs;

public:
  void gen_pattern(FuncEntity& func, const std::string& prev) const override;
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;
  void analyze_var_movable(std::set<std::string>& movables) override;

  void print_expr() const override;
};

/**
 * SetExpr describes expression like {...}
 *  exprs are splited by comma in {}
 */
struct SetExpr final : Expr
{
  std::vector<Ptr<Expr>> exprs;

  /**
   * @exprs: exprs in {}
   */
  SetExpr();
  SetExpr(std::vector<Ptr<Expr>>&& exprs);

public:
  void gen_pattern(FuncEntity& func, const std::string& prev) const override;
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;
  void analyze_var_movable(std::set<std::string>& movables) override;

  void print_expr() const override;
};

struct BinaryOpExpr final : Expr
{
  Token op;
  mutable Ptr<Expr> lhs, rhs;

  BinaryOpExpr(Token op, Ptr<Expr>&& lhs, Ptr<Expr>&& rhs);

public:
  void gen_pattern(FuncEntity& func, const std::string& prev) const override;
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;
  void analyze_var_movable(std::set<std::string>& movables) override;

  void print_expr() const override;
};

struct Equation final
{
  Ptr<Expr> pattern;
  Ptr<Expr> expr;
  // for helpful comment
  std::string raw_str;

public:
  void gen_funcentity(FuncEntity& func) const;
};

struct LetinExpr final : Expr
{
  Equation equation;
  Ptr<Expr> expr;

  LetinExpr(Equation&& equation);

public:
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;
  void analyze_var_movable(std::set<std::string>& movables) override;

  void print_expr() const override;
};

struct CaseExpr final : Expr
{
  Ptr<Expr> expr;
  std::vector<Equation> equations;

  CaseExpr(Ptr<Expr>&& expr);

public:
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;
  void analyze_var_movable(std::set<std::string>& movables) override;

  void print_expr() const override;
};

struct LambdaExpr final : Expr
{
  std::vector<std::string> parameters;
  Ptr<Expr> expr;

  std::vector<Ptr<Type>> lambda_storage;
  std::multimap<std::string, std::reference_wrapper<Ptr<Type>>>
    lambda_types_map;

public:
  std::string gen_expr_impl(FuncEntity& func,
                            const TypeInfo& typeinfo) const override;

  void print_expr() const override;
};

struct Definition
{
  virtual ~Definition() = 0;

  virtual std::string def_name() const { return ""; }

  virtual bool is_error() const { return false; }

  // check the definition is pre-def of not
  virtual bool is_predef() const { return false; }
  /// check the def has been pre-defined or not
  virtual bool is_predefined() const { return false; }
  virtual bool is_datatype_decl() const { return false; }
  virtual bool is_function_decl() const { return false; }

  // These func is code by myk.
  virtual bool is_isomorphism() const { return false; }

  virtual void gen_code(Code&) const;
};

struct ErrorDefinition : Definition
{
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
struct DatatypeDef : Definition
{
  /**
   * Component likes ConsExpr
   *  but arguments are types but not expressions
   */
  struct Component
  {
    std::string constructor;
    std::vector<Ptr<Type>> arguments;
    Component clone() const;
  };

  Ptr<Type> decl_type;
  // mutable for gen_code
  mutable std::vector<Component> components;

  std::string def_name() const override;

  bool is_predefined() const override;
  bool is_datatype_decl() const override;
  DatatypeDef clone() const;

  //These func is code by myk.
  bool is_isomorphism() const override;
  //compare struct component, same return true.
  bool compare_component(DatatypeDef::Component, DatatypeDef::Component) const;
  bool compare_components(std::vector<DatatypeDef::Component>&) const;
  bool compare_type(Ptr<Type>, Ptr<Type>) const;

public:
  void gen_code(Code&) const override;
};

struct FunctionDef : Definition
{
  std::string name;
  Ptr<FuncType> type;
  std::vector<Equation> equations;

  bool nonexhaustive = false;
  bool memoize = false;

  std::string def_name() const override;

  bool is_predefined() const override;
  bool is_function_decl() const override;

public:
  void gen_code(Code&) const override;
  bool compare_type(Ptr<FuncType>);
};

// as a special DatatypeDef
struct PreDatatypeDef final : DatatypeDef
{
  virtual bool is_predef() const override { return true; }
  virtual bool is_predefined() const override { return false; }

};

// as a special FunctionDef
struct PreFunctionDef final : FunctionDef
{
  virtual bool is_predef() const override { return true; }
  virtual bool is_predefined() const override { return false; }

};

struct ShortDef final : Definition
{
  std::string name;
  mutable std::vector<std::string> parameters;
  mutable Ptr<Expr> expr;

  ShortDef();
  ShortDef(std::string name, std::vector<std::string> params, Ptr<Expr> expr);

  std::string def_name() const override;

public:
  void gen_code(Code&) const override;
};

struct Theory final
{
  std::string name;
  std::vector<std::string> imports;
  std::vector<Ptr<Definition>> definitions;

public:
  Code gen_code() const;
};
} // namespace hol2cpp
