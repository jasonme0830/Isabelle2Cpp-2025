#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
void FuncType::gen_funcentity(FuncEntity &func) const {
    for (auto &type : types) {
        func.add_typeinfo(type->gen_typeinfo(func));
    }
}

void Equation::gen_funcentity(FuncEntity &func) const {
    pattern->gen_pattern(func, "");
    func.add_expr("return $;", expr->gen_expr(func, func.result_typeinfo()));
}
} // namespace hol2cpp
