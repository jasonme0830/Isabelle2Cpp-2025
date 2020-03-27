#include "ast.hpp"
#include "code.hpp"
#include "context.hpp"

using namespace std;

namespace hol2cpp {
AST::~AST() = default;
Type::~Type() = default;
Expr::~Expr() = default;

void NormalType::gen_typeinfos(Context &context) const {
  context.add_normal_type(name);
}

void ArgumentType::gen_typeinfos(Context &context) const {
  context.add_argument_type(name);
}

void TemplateType::gen_typeinfos(Context &context) const {
  context.add_template_type(name);
  arg->gen_typeinfos(context);
}

void FuncType::gen_typeinfos(Context &context) const {
  for (size_t i = 0; i < types.size() - 1; ++i) {
    context.entry_argument_type(i);
    types[i]->gen_typeinfos(context);
  }
  context.entry_result_type();
  result_type()->gen_typeinfos(context);
}

void FuncDecl::codegen(Context &context, Code &code) const {
  code.entry_func();
  type->gen_typeinfos(context);
}
}
