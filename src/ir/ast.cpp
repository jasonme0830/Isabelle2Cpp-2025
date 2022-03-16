#include "ast.hpp"

#define assert(expr) assert_impl(expr, CodegenError)

namespace hol2cpp {
Type::~Type() = default;
Expr::~Expr() = default;
Definition::~Definition() = default;

Type *FuncType::result_type() const {
    assert(!types.empty());
    return types.back().get();
}
} // namespace hol2cpp
