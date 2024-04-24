#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
string
NormalType::gen_datatype(Datatype& type) const
{
  // return name;
  if (theNormalTypeMapping.count(name)) {
    if (!type.is_predef()) {
      type.code().add_header("cstdint");
    }

    return theNormalTypeMapping.at(name);
  } else {
    // e.g. bool
    return name;
  }
}

string
ArgumentType::gen_datatype(Datatype& type) const
{
  return type.add_argument_type(name);
}

string
TemplateType::gen_datatype(Datatype& type) const
{
  if (!type.is_predef() && theHeaderMapping.count(name)) {
    type.code().add_header(theHeaderMapping.at(name));
  }

  string map_name = theTemplateTypeMapping.count(name)
                    ? theTemplateTypeMapping.at(name)
                    : name;

  auto res = map_name + '<';
  for (size_t i = 0; i < args.size(); ++i) {
    if (i == 0) {
      res += args[i]->gen_datatype(type);
    } else {
      res += ", " + args[i]->gen_datatype(type);
    }
  }
  return res + '>';
}

string
FuncType::gen_datatype(Datatype& type) const
{
  string res = "std::function<" + result_type()->gen_datatype(type) + '(';
  for (size_t i = 0; i < types.size() - 1; ++i) {
    if (i == 0) {
      res += types[i]->gen_datatype(type);
    } else {
      res += ", " + types[i]->gen_datatype(type);
    }
  }
  return res + ")>";
}

/**
 * @brief get the main name of the type, which is usually used for datatypes
 *
 * @return "list" for 'a list
 */
string
Type::main_name() const
{
  throw CodegenError("failed call of Type::main_name()");
}

string
NormalType::main_name() const
{
  return name;
}

string
ArgumentType::main_name() const
{
  return name;
}

string
TemplateType::main_name() const
{
  return name;
}
// string
// FuncType::main_name() const
// {
//   //权宜之计，函数数据类型是没有名字的
//   return "FuncType";
// }
} // namespace hol2cpp
