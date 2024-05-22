#include "../utility/config.hpp"
#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
void
FuncType::gen_funcentity(FuncEntity& func) const
{
  for (auto& type : types) {
    func.add_typeinfo(type->gen_typeinfo(func));
  }
}

void
Equation::gen_funcentity(FuncEntity& func) const
{
  pattern->gen_pattern(func, "");
  func.close_pattern(); // for delay declarations

  // if (theConfig.move_list()) {
  if(func.func_recu_class() > -1){
    set<string> movables;
    expr->analyze_var_movable(movables);
    func.add_expr("return $;", unmove_expr(expr->gen_expr(func)));
  } else {
    func.add_expr("return $;", expr->gen_expr(func));
  }
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
