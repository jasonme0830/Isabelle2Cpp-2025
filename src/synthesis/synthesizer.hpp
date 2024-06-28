#pragma once

#include "../ir/ir.hpp"

namespace hol2cpp {
class Synthesizer
{
public:
  Synthesizer(std::string);

  void synthesize(const Code&);

private:
  void syn_header(const Code&);
  void syn_impl(const Code&);

  void syn_typedefs(const Code&);
  void syn_single_typedef(const std::string&, const Datatype&);
  void syn_single_typedef_template(const std::string&, const Datatype&);
  void syn_single_typedef_definition(const std::string&, const Datatype&);

  void syn_class(const Datatype& datatype);
  void syn_class_template(const Datatype& datatype);
  void syn_class_definition(const Datatype& datatype);

  void syn_class_def_variant(const Datatype &, const TypeInfo &);
  void syn_class_def_defaultConstructor(const Datatype &, const TypeInfo &);
  void syn_class_def_valueConstructor(const Datatype &, const TypeInfo &);
  void syn_class_def_copyConstructor(const Datatype &);
  void syn_class_def_moveConstructor(const Datatype &);
  void syn_class_def_staticConstructor(const Datatype &);
  void syn_class_def_isfunction(const Datatype &);
  void syn_class_def_asfunction(const Datatype &);

  void syn_class_def_moveOperator(const Datatype &);
  void syn_class_def_copyOperator(const Datatype &);
  void syn_class_def_equivOperator(const Datatype &);
  void syn_class_def_ltOperator(const Datatype &);
  void syn_class_def_gtOperator(const Datatype &);
  void syn_class_def_leOperator(const Datatype &);
  void syn_class_def_geOperator(const Datatype &);

  void syn_class_def_struct_empty(const Datatype &, size_t);
  void syn_class_def_struct_members(const Datatype &, size_t, std::string &, std::string &);
  void syn_class_def_struct_getMethod(const Datatype &, size_t);
  void syn_class_def_struct_paramConstructor(const Datatype &, size_t);
  void syn_class_def_struct_copyConstructor(const Datatype &, size_t);
  void syn_class_def_struct_moveConstructor(const Datatype &, size_t);

  void syn_class_def_struct_copyOperator(const Datatype &, size_t);
  void syn_class_def_struct_moveOperator(const Datatype &, size_t);
  void syn_class_def_struct_equivOperator(const Datatype &, size_t, std::string, std::string);
  void syn_class_def_struct_ltOperator(const Datatype &, size_t, std::string, std::string);
  void syn_class_def_struct_gtOperator(const Datatype &, size_t, std::string, std::string);
  void syn_class_def_struct_leOperator(const Datatype &, size_t, std::string, std::string);
  void syn_class_def_struct_geOperator(const Datatype &, size_t, std::string, std::string);

  void syn_func(const FuncEntity& func, bool is_impl = true);
  void syn_func_template(const FuncEntity& func);
  void syn_func_definition(const FuncEntity& func, bool is_impl);
  void syn_func_body(const FuncEntity& func);
  std::string syn_func_params(const FuncEntity& func);
  std::string syn_func_param_types(const FuncEntity& func);
  std::string syn_func_args(const FuncEntity& func);

  std::ostream& newline(int more_indent = 0);

  void add_indent();
  void sub_indent();

private:
  std::string filename_;
  std::ofstream header_out_;
  std::ofstream impl_out_;
  std::reference_wrapper<std::ofstream> out_;
  int indent_;
};
} // namespace hol2cpp
