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
} // namespace hol2cpp
