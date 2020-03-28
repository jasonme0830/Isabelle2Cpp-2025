#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace hol2cpp {
AST::~AST() = default;
Type::~Type() = default;
Expr::~Expr() = default;

void NormalType::gen_typeinfo(FuncEntity &entity) const {
  static map<string, string> mapping {
    { "nat", "std::uint64_t" },
    { "int", "std::int64_t" }
  };

  if (mapping.count(name)) {
    entity.add_normal_type(mapping[name]);
  } else {
    entity.add_normal_type(name);
  }
}

void ArgumentType::gen_typeinfo(FuncEntity &entity) const {
  entity.add_argument_type(name);
}

void TemplateType::gen_typeinfo(FuncEntity &entity) const {
  static map<string, string> mapping {
    { "set", "std::set" },
    { "option", "std::optional" },
    { "list", "std::list" }
  };

  if (mapping.count(name)) {
    entity.add_template_type(mapping[name]);
  } else {
    entity.add_template_type(name);
  }
  arg->gen_typeinfo(entity);
}

void FuncType::gen_typeinfo(FuncEntity &entity) const {
  entity.entry_type();
  result_type()->gen_typeinfo(entity);
  for (size_t i = 0; i < types.size() - 1; ++i) {
    entity.entry_type();
    types[i]->gen_typeinfo(entity);
  }
}

void VarExpr::codegen(Code &code) const {

}

void ConsExpr::codegen(Code &code) const {

}

void FuncDecl::codegen(Code &code) const {
  code.entry_func();
  code.current_entity().set_name(name);
  type->gen_typeinfo(code.current_entity());
}
}
