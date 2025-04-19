#include "funcentity.hpp"
#include "../codegen/codegen.hpp"
#include "../utility/config.hpp"
#include "../utility/error.hpp"
#include "../utility/format.hpp"
#include "code.hpp"

#include <regex>

#define assert_true(expr) assert_impl(expr, CodegenError)

using namespace std;

namespace hol2cpp {
namespace asArg_namespace{
string
default_as_arg(const TypeInfo& type, int arg_gen_mode)
{
  return type.to_str(arg_gen_mode)+" ";
}
string
const_as_arg(const TypeInfo& type, int arg_gen_mode)
{
  return "const " + type.to_str(arg_gen_mode) + " &";
}
} // namespace for gen func type argus

function<string(const TypeInfo&, int)> TypeInfo::as_arg = asArg_namespace::const_as_arg;

TypeInfo::TypeInfo() = default;

TypeInfo::TypeInfo(string name)
  : name(move(name))
{
  // ...
}

TypeInfo::TypeInfo(string name, TypeInfo argument)
  : name(move(name))
{
  if (argument.lack()) {
    name.clear();
  } else {
    arguments.push_back(move(argument));
  }
}

TypeInfo
TypeInfo::replace_with(string name) const
{
  auto type = *this;
  type.name = move(name);
  return type;
}

string
TypeInfo::to_str(int arg_gen_mode) const
{
  if (lack()) {
    return "UNKNOWN_TYPE";
  }

  if (arguments.empty()) {
    return name;
  }

  if (is_function()) {
    auto type = name + '<' + arguments.back().to_str(arg_gen_mode) + '(';
    for (size_t i = 0; i < arguments.size() - 1; ++i) {
      if (i == 0) {
        type += arguments[i].to_str_as_arg(arg_gen_mode);
      } else {
        type += ", " + arguments[i].to_str_as_arg(arg_gen_mode);
      }
    }
    return type + ")>";
  } else {
    auto type = name + '<';
    for (size_t i = 0; i < arguments.size(); ++i) {
      if (i == 0) {
        type += arguments[i].to_str(arg_gen_mode);
      } else {
        type += ", " + arguments[i].to_str(arg_gen_mode);
      }
    }
    return type + '>';
  }
}

string
TypeInfo::to_str_as_arg(int arg_gen_mode) const
{
  switch (arg_gen_mode)
  {
  case 0:
    TypeInfo::as_arg = asArg_namespace::default_as_arg;
    break;
  case 1:
    TypeInfo::as_arg = asArg_namespace::default_as_arg;
    break;
  case 2:
    TypeInfo::as_arg = asArg_namespace::const_as_arg;
    break;
  default:
    TypeInfo::as_arg = asArg_namespace::default_as_arg;
    break;
  }

  return as_arg(*this, arg_gen_mode);
}

void
TypeInfo::avoid_lack(FuncEntity& func, const string& constructor) const
{
  if (lack()) {
    throw CodegenError(
      "Shit! Here is a bug!\n  The constructor $ lacks of type information in the definition of function $"_fs
        .format(info::strong(constructor), info::strong(func.name())));
  }
}

bool
TypeInfo::lack() const
{
  return name.empty();
}

bool
TypeInfo::is_function() const
{
  return name == "std::function";
}

const TypeInfo&
TypeInfo::result_typeinfo() const
{
  assert_true(is_function());
  return arguments.back();
}

size_t
TypeInfo::args_size() const
{
  return arguments.size() - 1;
}

bool
TypeInfo::movable() const
{
  // return theConfig.move_list() && name == theTemplateTypeMapping["list"];
  return !theConfig.close_move();
}

const TypeInfo&
TypeInfo::operator[](int i) const
{
  static TypeInfo theNullTypeInfo;

  if (lack()) {
    return theNullTypeInfo;
  } else if (i == -1) {
    return result_typeinfo();
  } else {
    auto index = (size_t)i;
    assert_true((size_t)index < arguments.size());
    return arguments[i];
  }
}

FuncEntity::FuncEntity(Code& code)
  : code_(code)
  , indent_(0)
{
  // ...
}

Code&
FuncEntity::code()
{
  return code_;
}

FuncEntity&
FuncEntity::add_indent()
{
  indent_ += theIndentSize;
  return *this;
}

FuncEntity&
FuncEntity::sub_indent()
{
  indent_ -= theIndentSize;
  return *this;
}

void
FuncEntity::name(std::string name)
{
  name_ = move(name);
}

const string&
FuncEntity::name() const
{
  return name_;
}

void
FuncEntity::add_typeinfo(TypeInfo typeinfo)
{
  typeinfos_.push_back(move(typeinfo));
}

string
FuncEntity::add_argument_type(string name)
{
  if (!template_mapping_.count(name)) {
    template_mapping_[name] = template_args_.size();
    template_args_.push_back("T" + to_string(template_args_.size() + 1));
  }
  return template_args_[template_mapping_[name]];
}

const TypeInfo&
FuncEntity::result_typeinfo() const
{
  return typeinfos_.back();
}

const vector<TypeInfo>&
FuncEntity::typeinfos() const
{
  return typeinfos_;
}

size_t
FuncEntity::args_size() const
{
  return typeinfos_.size() - 1;
}

const vector<string>&
FuncEntity::template_args() const
{
  return template_args_;
}

void
FuncEntity::decl_variable(const string& var, const string& expr)
{
  static regex arg_regex(R"(arg[1-9][0-9]*)");
  if (regex_match(expr, arg_regex)) {
    /* 这些可以省略不必要的解包复制，但是牺牲了代码的可读性，暂时放弃 */
    // var_mapping_[var] = expr;
    // // experimental
    // auto n = stoull(expr.substr(3));
    // var_typeinfos_[expr] = typeinfos_[n - 1];

    // code by myk
    unused_var_count_[var] = delay_statements_.size();
    switch (func_gen_mode())
    {
    case 2:
      add_delay_statement("auto $ = $;", var, expr);
      break;
    case 1:
      add_delay_statement("auto $ = $;", var, move_expr(expr));
      break;
    case 0:
      add_delay_statement("auto $ = $;", var, expr);
      break;
    default:
      add_delay_statement("auto $ = $;", var, expr);
      break;
    }
  } else {
    unused_var_count_[var] = delay_statements_.size();
    add_delay_statement("auto $ = $;", var, expr);
  }
}

string
FuncEntity::get_variable(const string& var)
{
  auto it = var_mapping_.find(var);
  if (it != var_mapping_.end()) {
    return it->second;
  } else {
    unused_var_count_.erase(var);
    return var;
  }
}

// experimental
TypeInfo
FuncEntity::get_var_typeinfo(const string& var)
{
  auto it = var_typeinfos_.find(var);
  if (it != var_typeinfos_.end()) {
    return it->second;
  } else {
    return TypeInfo();
  }
}

string
FuncEntity::gen_temp()
{
  return "temp" + to_string(temp_count_++);
}

void
FuncEntity::entry_equation()
{
  temp_count_ = 0;
  condition_count_ = 0;
  statements_.emplace_back();
  var_mapping_.clear();
  unused_var_count_.clear();
  decl_base_ = 0;
}

void
FuncEntity::close_equation()
{
  while (condition_count_--) {
    sub_indent();
    statements_.back().push_back(string(indent_, ' ') + "}");
  }
  for (auto& [_, ind] : unused_var_count_) {
    statements_.back()[decl_base_ + ind].clear();
  }
}

void
FuncEntity::close_sub_equation(size_t n)
{
  while (condition_count_ > n) {
    --condition_count_;
    sub_indent();
    statements_.back().push_back(string(indent_, ' ') + "}");
  }
}

void
FuncEntity::add_pattern(const string& pattern)
{
  statements_.back().push_back(string(indent_, ' ') + pattern);
}

void
FuncEntity::add_pattern_cond(const string& cond)
{
  // remove only if the option is used and the function is total
  auto remove_last_cond = theConfig.reduce_cond() && !nonexhaustive_;

  if (!is_last_equation_ || !remove_last_cond) {
    statements_.back().push_back(string(indent_, ' ') +
                                 "if ($) {"_fs.format(cond));
    add_indent();
    ++condition_count_;
  }
}

void
FuncEntity::add_delay_statement(const string& pattern)
{
  delay_statements_.push_back(pattern);
}

void
FuncEntity::close_pattern()
{
  decl_base_ = statements_.back().size();
  for (auto& decl : delay_statements_) {
    statements_.back().push_back(string(indent_, ' ') + decl);
  }
  delay_statements_.clear();
}

FuncEntity&
FuncEntity::add_expr(const string& expr)
{
  statements_.back().push_back(std::string(indent_, ' ') + expr);
  return *this;
}

size_t
FuncEntity::condition_count() const
{
  return condition_count_;
}

void
FuncEntity::app_last_stmt(const string& app_stmt)
{
  statements_.back().back() += app_stmt;
}

const vector<vector<string>>&
FuncEntity::statements() const
{
  return statements_;
}

const vector<string>&
FuncEntity::delay_declarations() const
{
  return delay_statements_;
}

void
FuncEntity::nonexhaustive(bool is_nonexhaustive)
{
  nonexhaustive_ = is_nonexhaustive;
}

bool
FuncEntity::nonexhaustive() const
{
  return nonexhaustive_;
}

void
FuncEntity::is_last_equation(bool is_last)
{
  is_last_equation_ = is_last;
}

bool
FuncEntity::is_last_equation() const
{
  return is_last_equation_;
}

void
FuncEntity::func_gen_mode(int num)
{
  func_gen_mode_ = num;
}

int
FuncEntity::func_gen_mode() const
{
  return func_gen_mode_;
}

void
FuncEntity::func_recu_class(int num)
{
  func_recu_class_ = num;
}

int
FuncEntity::func_recu_class() const
{
  return func_recu_class_;
}

void
FuncEntity::func_mem_mode(int num)
{
  func_mem_mode_ = num;
}

int
FuncEntity::func_mem_mode() const
{
  return func_mem_mode_;
}

void
FuncEntity::is_predef(bool value)
{
  is_predef_ = value;
}

bool
FuncEntity::is_predef() const
{
  return is_predef_;
}
} // namespace hol2cpp
