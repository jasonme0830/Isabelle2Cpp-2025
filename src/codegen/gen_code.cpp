#include "../utility/config.hpp"
#include "codegen.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

namespace hol2cpp {
Code
Theory::gen_code() const
{
  Code code;

  size_t predefs = 0, predefined = 0, datatype_defs = 0, function_defs = 0,
         shortdef_defs = 0;
  vector<string> datatype_gens, function_gens, shortdef_gens;

  for (size_t i = 0; i < definitions.size(); ++i) {
    auto decl = definitions[i].get();

    // pass all pre-defs
    if (decl->is_predef()) {
      ++predefs;
    } else if (decl->is_datatype_decl()) {
      ++datatype_defs;
    } else if (decl->is_function_decl()) {
      ++function_defs;
    } else {
      ++shortdef_defs;
    }

    if (!decl->is_error())
      try {
        if (decl->is_predefined()) {
          ++predefined;
        } else {
          decl->gen_code(code);
        }

        // pass pre-defs
        if (decl->is_predef()) {
          continue;
        }

        if (decl->is_datatype_decl()) {
          datatype_gens.push_back(decl->def_name());
        } else if (decl->is_function_decl()) {
          function_gens.push_back(decl->def_name());
        } else {
          shortdef_gens.push_back(decl->def_name());
        }
      } catch (const exception& e) {
        string name;
        if (auto datatype_decl = dynamic_cast<DatatypeDef*>(decl)) {
          name = "datatype " + datatype_decl->decl_type->main_name();
          code.pop_datatype();
        } else {
          auto fun_decl = static_cast<FunctionDef*>(decl);
          name = "function " + fun_decl->name;
          code.pop_function();
        }
        "$ at No.$ definition of $:\n`$\n\n"_fs.outf(
          cerr, info::light_red("codegen error"), i + 1, name, e.what());
      }
  }

  auto defs = datatype_defs + function_defs;
  "$\n`scanned $ definitions, contain $ datatypes and $ functions;\n"_fs.outf(
    cout, info::light_green("Result:"), defs, datatype_defs, function_defs);

  auto gens = datatype_gens.size() + function_gens.size();
  "`generated $ definitions, contain $ predefined:\n"_fs.outf(
    cout, gens, predefined);
  "``$ datatypes and $ functions.\n"_fs.outf(
    cout, datatype_gens.size(), function_gens.size());

  cout << "````datatypes:" << endl;
  for (auto& datatype : datatype_gens) {
    cout << datatype << ", ";
  }

  cout << "\n````functions:" << endl;
  for (auto& function : function_gens) {
    cout << function << ", ";
  }

  cout << endl;

  return code;
}

void
DatatypeDef::gen_code(Code& code) const
{
  if (!is_predef()) {
    code.add_header("variant");
  }

  auto name = decl_type->main_name();
  auto& datatype = code.entry_datatype(name);

  datatype.is_predef(is_predef());
  datatype.set_name(move(name));
  datatype.set_self(decl_type->gen_datatype(datatype));

  vector<vector<Ptr<Type>>> abstracts;
  for (size_t i = 0; i < components.size(); ++i) {
    abstracts.push_back({});
    datatype.entry_component();
    datatype.add_constructor(components[i].constructor);
    code.bind_cons(components[i].constructor, datatype);

    for (auto& type : components[i].arguments) {
      auto field_type = type->gen_datatype(datatype);
      datatype.add_field_type(field_type);
      if (field_type == datatype.self()) {
        datatype.is_recuisive(true);
        if (!datatype.is_predef()) {
          code.add_header("memory");
        }
      }
      abstracts.back().push_back(move(type));
    }
  }
  datatype.abstracts() = move(abstracts);
}

void
FunctionDef::gen_code(Code& code) const
{
  auto& func = code.entry_func_entity(name);

  func.is_predef(is_predef());
  func.nonexhaustive(nonexhaustive);
  func.memoize(memoize);
  func.name(name);

  type->gen_funcentity(func);

  if (memoize) {
    assert_true(func.typeinfos().size() > 1);
    code.add_header("map");
  }

  for (size_t i = 0; i < equations.size(); ++i) {
    func.is_last_equation(i == equations.size() - 1); // for reduce-cond

    func.entry_equation();
    func.add_expr("// $", equations[i].raw_str);
    equations[i].gen_funcentity(func);
    func.close_equation();
  }

  if (!func.statements().empty() && !func.statements().back().empty()) {
    auto& last_stmt = func.statements().back().back();
    if (last_stmt.back() == '}') {
      func.app_last_stmt(" else { // auto-generated for -Wreturn-type");
      func.add_indent().add_expr("std::abort();").sub_indent().add_expr("}");
    }
  }
}

void
ShortDef::gen_code(Code& code) const
{
  code.add_short_def(name,
                     make_unique<ShortDef>(name, move(parameters), move(expr)));
}

/**
 * @brief check the datatype is pre-defined or not
 *
 * @return true if the datatype is pre-defined
 * @return false if not
 */
bool
DatatypeDef::is_predefined() const
{
  return thePredefinedDatatypes.count(decl_type->main_name());
}
/**
 * @brief check the function is pre-defined or not
 *
 * @return true if the function is pre-defined
 * @return false if not
 */
bool
FunctionDef::is_predefined() const
{
  return thePredefinedFunctions.count(name);
}

//These definition is code by myk.

bool
DatatypeDef::is_defined() const
{
  std::set<Ptr<DatatypeDef>>::iterator it_data;
  for(it_data = theDefinedDatatypes.begin(); it_data != theDefinedDatatypes.end(); it_data++)
  {
    if(compare_type(decl_type, (*it_data)->decl_type)){
      //Same type, handle by type.
      switch (get_type())
      {
      case 0:
        //Something wrong.
        return true;
      case 1:
        //Normal type, even same, temporarily do nothing.
        return false;
      case 2:
        //Argument type, even same, temporarily do nothing.
        return false;
      case 3:
        //Templete type, and same type, then compare compents.
        if(compare_components(components, (*it_data)->components)){
          //Same, need to do something, but temporarily don't know do waht.
          return true;
        }
        else{
          //Not same
          return false;
        }
        break;
      default:
        //Default: not same.
        return false;
        break;
      }
    }
  }
  //compare finished, and type different from all the definition.
  return false;
}
bool
FunctionDef::is_defined() const
{

}
} // namespace hol2cpp
