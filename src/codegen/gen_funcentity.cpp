#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
void FuncType::gen_funcentity(FuncEntity &entity) const {
    for (auto &type : types) {
        entity.add_typeinfo(type->gen_typeinfo(entity));
    }
}

void Equation::gen_funcentity(FuncEntity &entity) const {
    pattern->gen_pattern(entity, "");
    entity.add_expr("return $;", expr->gen_expr(entity, entity.result_typeinfo()));
}
} // namespace hol2cpp
