#include "../codegen/codegen.hpp"
#include "../ir/ast.hpp"
#include "../ir/funcentity.hpp"
#include "../utility/config.hpp"

using namespace std;

namespace hol2cpp {
void
Config::move_list(bool enable)
{
  move_list_ = enable;

  if (!enable) {
    return;
  }

  auto origin_as_arg = TypeInfo::as_arg;
  TypeInfo::as_arg = [=](const TypeInfo& type) {
    if (type.name == theTemplateTypeMapping["list"]) {
      return type.to_str() + " ";
    } else {
      return origin_as_arg(type);
    }
  };
}

// todo use 'unmove_expr' correctly
void
Expr::analyze_var_movable(set<string>&)
{
  // ...
}

void
VarExpr::analyze_var_movable(set<string>& movables)
{
  if (!movables.count(name)) {
    movable = true;
    movables.insert(name);
  } else {
    movable = false;
  }
}

// todo do different by different constructors. e.g., If-expr
void
ConsExpr::analyze_var_movable(set<string>& movables)
{
  if (constructor == "If") {
    auto origin = movables;
    for (size_t i = 1; i < 3; ++i) {
      auto self_branch = origin;
      args[i]->analyze_var_movable(self_branch);
      movables.merge(self_branch);
    }
    args[0]->analyze_var_movable(movables);
    return;
  }

  for (auto it = args.rbegin(); it != args.rend(); ++it) {
    (*it)->analyze_var_movable(movables);
  }
}

void
ListExpr::analyze_var_movable(set<string>& movables)
{
  for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) {
    (*it)->analyze_var_movable(movables);
  }
}

void
SetExpr::analyze_var_movable(set<string>& movables)
{
  for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) {
    (*it)->analyze_var_movable(movables);
  }
}

void
BinaryOpExpr::analyze_var_movable(set<string>& movables)
{
  rhs->analyze_var_movable(movables);
  lhs->analyze_var_movable(movables);
}

void
LetinExpr::analyze_var_movable(set<string>& movables)
{
  expr->analyze_var_movable(movables);
  equation.expr->analyze_var_movable(movables);
}

void
CaseExpr::analyze_var_movable(set<string>& movables)
{
  auto origin = movables;
  for (auto& equation : equations) {
    auto self_branch = origin;
    equation.expr->analyze_var_movable(self_branch);
    movables.merge(self_branch);
  }
  expr->analyze_var_movable(movables);
}
} // namespace hol2cpp
