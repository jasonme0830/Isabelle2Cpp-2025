#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace hol2cpp {
AST::~AST() = default;
Type::~Type() = default;
Expr::~Expr() = default;

void NormalType::build_entity(FuncEntity &entity) const {
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

void ArgumentType::build_entity(FuncEntity &entity) const {
  entity.add_argument_type(name);
}

void TemplateType::build_entity(FuncEntity &entity) const {
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
  arg->build_entity(entity);
}

void FuncType::build_entity(FuncEntity &entity) const {
  entity.entry_type();
  result_type()->build_entity(entity);
  for (size_t i = 0; i < types.size() - 1; ++i) {
    entity.entry_type();
    types[i]->build_entity(entity);
  }
}

void VarExpr::build_entity(FuncEntity &entity) const {
  static map<string, string> mapping {
    { "0", "0" }
  };
}

void ConsExpr::build_entity(FuncEntity &entity) const {
  static map<string, string> mapping {
    { "Suc", " + 1" }
  };
}

void Equation::build_entity(FuncEntity &entity) const {
  pattern->build_entity(entity);
  entity.entry_expr();
  expr->build_entity(entity);
}

void FuncDecl::build_entity(FuncEntity &entity) const {
  entity.set_name(name);
  type->build_entity(entity);
  for (auto &equation : equations) {
    entity.entry_equation();
    equation->build_entity(entity);
  }
}
}
