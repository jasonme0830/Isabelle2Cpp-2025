#include "ast.hpp"
#include "code.hpp"
#include "context.hpp"

namespace hol2cpp {
AST::~AST() = default;
Type::~Type() = default;
Expr::~Expr() = default;

void FuncDecl::codegen(Context &context, Code &code) const {
  auto result_type = type->result_type();
}
}
