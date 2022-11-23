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

  void syn_class(const Datatype& datatype);
  void syn_class_template(const Datatype& datatype);
  void syn_class_definition(const Datatype& datatype);

  void syn_func(const FuncEntity& func, bool is_impl = true);
  void syn_func_template(const FuncEntity& func);
  void syn_func_definition(const FuncEntity& func, bool is_impl);

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
