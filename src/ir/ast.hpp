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
#include <queue>

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
  virtual std::vector<Ptr<Type>> depth_traversal() = 0;
  virtual std::vector<Ptr<Type>> width_traversal() = 0;
  virtual int args_num() = 0;
  virtual bool replace_self_iso_type(std::set<std::string>&) = 0;
  virtual bool replace_self_iso_type() = 0;
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
  std::vector<Ptr<Type>> depth_traversal() override;
  std::vector<Ptr<Type>> width_traversal() override;
  int args_num() override;
  bool replace_self_iso_type(std::set<std::string>&) override;
  bool replace_self_iso_type() override;
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
  std::vector<Ptr<Type>> depth_traversal() override;
  std::string main_name() const override;
  std::vector<Ptr<Type>> width_traversal() override;
  int args_num() override;
  bool replace_self_iso_type(std::set<std::string>&) override;
  bool replace_self_iso_type() override;
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
  std::vector<Ptr<Type>> depth_traversal() override;
  std::vector<Ptr<Type>> width_traversal() override;
  int args_num() override;
  bool replace_self_iso_type(std::set<std::string>&) override;
  bool replace_self_iso_type() override;
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
  // std::string main_name() const override;

  Ptr<Type> clone() const override;
  void print_type() const override;

  //code by myk
  std::string get_datatype() const override;
  std::vector<Ptr<Type>> depth_traversal() override;
  std::vector<Ptr<Type>> width_traversal() override;
  int args_num() override;
  bool replace_self_iso_type(std::set<std::string>&) override;
  bool replace_self_iso_type() override;
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

  virtual void analyze_var_movable(std::set<std::string>& movables); // for move
  virtual void remark_reuse_var_movable(std::set<std::string>& reuse);

  virtual void print_expr() const = 0;
  virtual void print_expr_type() const;

  virtual void traversal_replace_cons(std::map<std::string,std::string>&)=0;
  virtual int trav_judge_recursive(std::string)=0;
  virtual void trav_count_pattern_var(std::map<std::string, int>&)=0;
  //标记是索引值：-1不操作、0-n增加计数
  virtual void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int)=0;
  virtual std::string get_expr_class()=0;

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

  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;
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
  void remark_reuse_var_movable(std::set<std::string>& reuse) override;

  void print_expr() const override;

  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;
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

  void analyze_var_movable(std::set<std::string>& movables) override;
  void remark_reuse_var_movable(std::set<std::string>& reuse) override;

  std::string gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const override;
  std::string gen_expr_impl_recuCall(FuncEntity& , const TypeInfo&) const;
  std::string gen_expr_impl_listCons(FuncEntity&, const TypeInfo&) const;
  std::string gen_expr_impl_if(FuncEntity&, const TypeInfo&) const;
  std::string gen_expr_impl_datatype(FuncEntity&, const TypeInfo&) const;
  std::string gen_expr_impl_funCall(FuncEntity&, const TypeInfo&) const;
  std::string gen_expr_impl_shortDef(FuncEntity&, const TypeInfo&) const;

  bool get_genTempOrNot(FuncEntity&, const TypeInfo&, const Ptr<Expr>) const;
  std::string gen_expr_impl_recuCall_Temp(FuncEntity&, const TypeInfo&, const Ptr<Expr>) const;
  std::string gen_expr_impl_recuCall_noTemp(FuncEntity&, const TypeInfo&, const Ptr<Expr>) const;
  std::string gen_expr_impl_funCall_Temp(FuncEntity&, const TypeInfo&, const Ptr<Expr>) const;
  std::string gen_expr_impl_funCall_noTemp(FuncEntity&, const TypeInfo&, const Ptr<Expr>) const;

  void print_expr() const override;
  void traversal_replace_cons(std::map<std::string,std::string>&) override;

  int trav_judge_recursive(std::string) override;
  int trav_judge_recursive_common(std::string);
  int trav_judge_recursive_recuCons(std::string);
  int trav_judge_recursive_ifexpr(std::string);
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;

  std::string get_expr_class() override;
  std::string get_constructor() const;
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
  void remark_reuse_var_movable(std::set<std::string>& reuse) override;

  void print_expr() const override;
  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;
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
  void remark_reuse_var_movable(std::set<std::string>& reuse) override;

  void print_expr() const override;
  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;
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
  void remark_reuse_var_movable(std::set<std::string>& reuse) override;

  void print_expr() const override;
  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;
};

struct Equation final
{
  Ptr<Expr> pattern;
  Ptr<Expr> expr;
  // for helpful comment
  std::string raw_str;

public:
  void gen_funcentity(FuncEntity& func) const;
  bool judge_mem_mode(const std::vector<std::map<std::string, int>>&);
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
  void remark_reuse_var_movable(std::set<std::string>& reuse) override;

  void print_expr() const override;
  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;
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
  void remark_reuse_var_movable(std::set<std::string>& reuse) override;
  
  void print_expr() const override;
  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;

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
  void traversal_replace_cons(std::map<std::string,std::string>&) override;
  int trav_judge_recursive(std::string) override;
  void trav_count_pattern_var(std::map<std::string, int>&) override;
  void trav_count_expr_var(std::string, std::vector<std::map<std::string, int>>&, int) override;
  std::string get_expr_class() override;
};

struct Definition
{
  virtual ~Definition() = 0;

  virtual std::string def_name() const { return ""; }

  virtual bool is_error() const { return false; }

  // distinguish and marks the definitions in the predef.thy base on rule-transform
  virtual bool is_predef() const { return false; }
  
  /// check the def has been pre-defined or not base on rule-transform.
  virtual bool is_predefined() const { return false; }
  virtual bool is_datatype_decl() const { return false; }
  virtual bool is_function_decl() const { return false; }

  // Code by myk, to cut down Coriolis complexity.
  virtual bool judge_isomorphism() const { return false; }
  // Isomorphism compare base on defination-transform.
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

  struct Compare
  {
    //比较结构
    struct Structer{
      bool width_res = false;
      std::vector<Ptr<Type>> subtree_cur;
      std::vector<Ptr<Type>> subtree_glo;
      //复制等号左侧的信息，以备处理递归
      Ptr<Type> decl_type_cur;
      Ptr<Type> decl_type_glo;

    public:
      //compare ast structer
      bool compare_structure(std::vector<Ptr<Type>>&, std::vector<Ptr<Type>>&);
      std::vector<Ptr<Type>> clone_width_queue(std::vector<Ptr<Type>>);
      std::vector<Ptr<Type>> get_width_traversual(std::vector<Ptr<Type>>);
      bool get_width_res();
      void initial();
    };

    //比较叶子节点的变量
    struct Leafargu{
      //tuple里面分别是输入参数的类型、等号左侧顺序、等号右侧顺序默认为0
      std::map<std::string, std::tuple<int,int,int>> decl_type_map_cur;
      std::map<std::string, std::tuple<int,int,int>> decl_type_map_glo;
      //等号右侧类型变量顺序总标记
      int var_type_order_cur = 0;
      int var_type_order_glo = 0;
      //复制等号左侧的信息，以备处理递归
      Ptr<Type> decl_type_cur;
      Ptr<Type> decl_type_glo;

      //内部临时变量，每次比较归零
      bool depth_res = false;
      std::vector<Ptr<Type>> types_cur;
      std::vector<Ptr<Type>> types_glo;

    public:
      //compare ast leaf node
      bool compare_arguments(std::vector<Ptr<Type>> &, std::vector<Ptr<Type>> &);
      std::vector<Ptr<Type>> get_depth_vector(std::vector<Ptr<Type>> &);
      bool compare_argu_normal_decl_normal(std::string&, std::string&);
      bool compare_argu_normal_decl_template(std::string&, std::string&);
      bool compare_argu_normal(Ptr<Type>, Ptr<Type>);
      bool compare_argu_argument(Ptr<Type>, Ptr<Type>);
      bool get_depth_res_both(std::vector<Ptr<Type>> &,std::vector<Ptr<Type>> &);
      void initial();
    };

    mutable Structer structer;
    mutable Leafargu leafargu;

    //存储所有规则的排列组合
    std::vector<std::vector<Component>> components_combination;

  public:
    //compare DatatypeDef decl_type
    bool compare_decl_type(Ptr<Type>, Ptr<Type>);

    //sort the component vector by priority(argu nums) for global
    std::vector<Component> get_base_order_vector(std::vector<Component>&);

    //arrange the components to many vectors by priority(argu nums) for this datatypeDef
    void get_components_combinations(std::vector<Component>&);
    //arrange components(n!) for this datatypeDef
    void recursion_arrange_component(std::vector<Component>&, int, int, std::vector<Component>);
    //to cut down the num of components combination by priority
    void cutdown_components_combination_by_order();

    //compare struct component, same return true.
    bool compare_components(std::vector<DatatypeDef::Component>&,std::vector<DatatypeDef::Component>&) const;

    //store the correspondence of the cons between two isomorphism
    void store_cons_to_map(std::map<std::string,std::string>&,std::vector<DatatypeDef::Component>&,std::vector<DatatypeDef::Component>&);

  };

  Ptr<Type> decl_type;
  // mutable for gen_code
  mutable std::vector<Component> components;

  std::string def_name() const override;

  bool is_predefined() const override;
  bool is_datatype_decl() const override;
  DatatypeDef clone() const;
  bool is_isomorphism() const override;

  /*
    Code by myk, to cut down Coriolis complexity.
    Compare this datatypeDef with global datatypeDefs.
    Then change the global map base on compare result.
  */
  bool judge_isomorphism() const override;
  //probably use isomorphism datatype, replace
  void replace_iso_type_in_data();

public:
  void gen_code(Code&) const override;
};

struct FunctionDef : Definition
{
  //code by myk, for handle isomorphism type cons of fun.
  struct Process
  {
    int func_type_size = 0;
    std::map<std::string, std::string> the_all_iso_cons_map;

  public:
    bool handle_head_isomor_type(Ptr<FuncType> );
    bool handle_body_isomor_type(std::vector<Equation>&);
    bool handle_equa_pattern_iso_type_cons(Ptr<Expr>);
    bool handle_equa_expr_iso_type_cons(Ptr<Expr>);
  };


  std::string name;
  Ptr<FuncType> type;
  std::vector<Equation> equations;

  bool nonexhaustive = false;
  bool memoize = false;

  int func_recursive_type = 0;
  //标识函数的生成模式：0值传递、1move优化、2未定义的优化
  int func_gen_mode = 1;
  //标识函数的递归类型：0非递归、1单次递归、2多次递归
  int func_recu_class = 0;
  //标志函数是否启用记忆化：0禁用、1启用
  int func_mem_mode = 0;

  std::string def_name() const override;

  bool is_predefined() const override;
  bool is_function_decl() const override;
  bool is_isomorphism() const override;

public:
  void gen_code(Code&) const override;
  //probably use isomorphism datatype, replace
  bool handle_isomorphism_datatype();
  bool judge_isomorphism() const override;

  //确定函数主体的生成模式：0值传递、1move优化、2未定义的优化
  int judge_func_gen_mode();
  //确定函数的递归次数，也为记忆化判断进行准备：0非递归、1单词递归、2多次递归
  int analyse_func_recu_class();
  //结合close-memo参数、thy中fun前的memoize参数和函数本身的性质判断是否启用记忆化
  int judge_func_mem_mode();
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

  //To generation typedef in hpp for isomorphism datatypes.
  bool gen_isomorphism_typedef(Code&) const;

public:
  Code gen_code() const;
};
} // namespace hol2cpp
