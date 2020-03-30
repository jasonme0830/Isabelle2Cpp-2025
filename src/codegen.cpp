#include <exception>
#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace hol2cpp {
AST::~AST() = default;
Type::~Type() = default;
Expr::~Expr() = default;

string Type::gen_typeinfo(FuncEntity &entity) const {
  throw std::logic_error("no gen_typeinfo implementation");
}

string NormalType::gen_typeinfo(FuncEntity &entity) const {
  static map<string, string> mapping {
    { "nat", "std::uint64_t" },
    { "int", "std::int64_t" }
  };

  return mapping.count(name) ? mapping[name] : name;
}

string ArgumentType::gen_typeinfo(FuncEntity &entity) const {
  return entity.add_argument_type(name);
}

string TemplateType::gen_typeinfo(FuncEntity &entity) const {
  static map<string, string> mapping {
    { "set", "std::set" },
    { "option", "std::optional" },
    { "list", "std::list" }
  };

  return mapping.count(name)
    ? mapping[name] + '<' + arg->gen_typeinfo(entity) + '>'
    : name + '<' + arg->gen_typeinfo(entity) + '>';
}

void FuncType::build_entity(FuncEntity &entity) const {
  entity.add_type(result_type()->gen_typeinfo(entity));
  for (size_t i = 0; i < types.size() - 1; ++i) {
    entity.add_type(types[i]->gen_typeinfo(entity));
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
  entity.name() = name;
  type->build_entity(entity);
  for (auto &equation : equations) {
    entity.entry_equation();
    equation->build_entity(entity);
  }
}
}
