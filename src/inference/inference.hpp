#pragma once

#include "../ir/ast.hpp"
#include <type_traits>

namespace hol2cpp {

using LambdaMap = std::multimap<std::string, std::reference_wrapper<Ptr<Type>>>;

struct TypeInference
{
public:
  TypeInference(Theory& thy);
  ~TypeInference();

public:
  void theory_infer();

  void map_init();
  void map_dtypedef_cons(DatatypeDef& dtypedef);
  void map_funcdef_type(FunctionDef& funcdef);
  void map_srtdef_type(ShortDef& srtdef);

  void definition_infer(Definition& def);

  void shortdef_infer(ShortDef& srtdef);

  void functiondef_infer(FunctionDef& funcdef);
  void function_pattern_infer(Expr& pattern, FunctionDef& funcdef);
  void function_expr_infer(Expr& expr, const std::string& funcname);

  void pattern_infer(Ptr<Expr>& expr, const std::string& funcname);
  void pattern_consexpr_infer(ConsExpr& expr, const std::string& funcname);
  void pattern_list_or_set_infer(Expr& expr, const std::string& funcname);

  void analy_from_bottom(Expr& expr,
                         const std::string& funcname,
                         LambdaMap& lambda_types_map,
                         LambdaMap& return_lambda_types_map);

  void analy_lambda_expr(Expr& expr, const std::string& funcname);

  void analy_from_top(Expr& expr);

  void template_type_ins(Ptr<Type>& type);
  void template_type_ins_lambda(Ptr<Expr>& lambda_expr);
  void template_type_map(Ptr<Type>& com_type, Ptr<Type>& ins_type);
  void template_type_map_lambda(Ptr<Type>& com_type, Ptr<Type>& ins_type);

  void type_mangling(Ptr<Type>& type);
  void type_mangling_aux(Ptr<Type>& type);

  void dtypedef_mangling(DatatypeDef& dtypedef);

  Ptr<Type> function_apply(Ptr<Type>& functype, std::vector<Ptr<Expr>>& params);
  void function_apply_aux(Ptr<Type>& func_param_type,
                          Ptr<Type>& ins_param_type);
  void function_update(std::vector<Ptr<Type>>& args);

  Ptr<Type> gen_new_lambda_arg();

  bool is_mangled(Ptr<Type>& type);
  bool is_lambda(Ptr<Type>& type);

  void print_theory();

private:
  Theory& thy;

  Ptr<NormalType> normal_nat_type;
  Ptr<NormalType> normal_bool_type;
  Ptr<NormalType> normal_unknown_type;
  LambdaMap null_map{};

  size_t lambda_counter;
  size_t argument_counter;

  bool ins_map_clear_flag;

  /**
   * map constructor's name to its Datatypedef:
   * e.g. <Cons, slist>, <Nil, slist>...
   */
  std::map<std::string, std::reference_wrapper<DatatypeDef>>
    the_dtypedef_mapping;

  /**
   * map constructor's name to its construct rule which has been transformed
   * into FuncType: e.g. <Cons, ('a => 'a list => 'a list)>, <Nil, 'a list>...
   */
  std::map<std::string, Ptr<FuncType>> the_cons_type_mapping;

  /**
   * map function's name to its Functiondef:
   * e.g. <func1, func1::ref<FunctionDef>>
   */
  std::map<std::string, std::reference_wrapper<FunctionDef>>
    the_func_type_mapping;

  /**
   * map function's arguments to its Type:
   * e.g.
   * fun "test :: 'a list => nat" where
   * "test Nil = 0" |
   * "test (Cons x xs) = length (If ((length xs) = 0) Nil xs) + 1"
   * map:
   * <test, <x, ref(Ptr<ArgumentType>>)>, <test, <xs, ref(Ptr<TemplateType>>)>
   */
  std::map<std::string, std::reference_wrapper<Ptr<Type>>> the_arg_type_mapping;

  /**
   * map type araguments to its institution type:
   * e.g.
   * fun "test :: nat list => nat" where...
   * map:
   * <'a, nat>
   */
  std::map<std::string, Ptr<Type>> the_argument_type_ins_mapping;
};
} // namespace hol2cpp