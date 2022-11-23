#include "../ir/ast.hpp"

using namespace std;

namespace hol2cpp {
void
NormalType::print_type() const
{
  cout << name;
}

void
ArgumentType::print_type() const
{
  cout << "'" << name;
}

void
TemplateType::print_type() const
{
  cout << "(";
  for (auto& elem : args) {
    elem->print_type();
    cout << " ";
  }
  cout << ")";
  cout << name;
}

void
FuncType::print_type() const
{
  cout << "(";
  for (size_t i = 0; i < types.size() - 1; ++i) {
    types[i]->print_type();
    cout << " => ";
  }
  types[types.size() - 1]->print_type();
  cout << ")";
}

void
Expr::print_expr_type() const
{
  if (expr_type == nullptr)
    return;
  expr_type->print_type();
}

void
IntegralExpr::print_expr() const
{
  cout << '(';
  cout << value;
  cout << " :: ";
  print_expr_type();
  cout << ')';
}

void
VarExpr::print_expr() const
{
  cout << '(';
  cout << name;
  cout << " :: ";
  print_expr_type();
  cout << ')';
}

void
ConsExpr::print_expr() const
{
  cout << "((";
  cout << constructor << ' ';
  for (auto& arg : args) {
    arg->print_expr();
  }
  cout << ')';
  cout << " :: ";
  print_expr_type();
  cout << ')';
}

void
ListExpr::print_expr() const
{
  cout << "([";
  for (auto& expr : exprs) {
    expr->print_expr();
  }
  cout << "]";
  cout << " :: ";
  print_expr_type();
  cout << ')';
}

void
SetExpr::print_expr() const
{
  cout << "({";
  for (auto& expr : exprs) {
    expr->print_expr();
  }
  cout << "}";
  cout << " :: ";
  print_expr_type();
  cout << ')';
}

void
BinaryOpExpr::print_expr() const
{
  cout << "((";
  lhs->print_expr();
  cout << op.value;
  rhs->print_expr();
  cout << ')';
  cout << " :: ";
  print_expr_type();
  cout << ')';
}

void
LetinExpr::print_expr() const
{
  cout << "(let";
  equation.pattern->print_expr();
  cout << " = ";
  equation.expr->print_expr();
  cout << "in (";
  expr->print_expr();
  cout << "))";
}

void
CaseExpr::print_expr() const
{
  cout << "((case ";
  expr->print_expr();
  cout << "of" << endl;
  for (auto& equation : equations) {
    equation.pattern->print_expr();
    cout << " => ";
    equation.expr->print_expr();
    cout << " | " << endl;
  }
  cout << "))";
}

void
LambdaExpr::print_expr() const
{
  cout << "(\\<lambda>";
  for (auto& arg : parameters) {
    cout << arg;
  }
  cout << ".";
  expr->print_expr();
  cout << " :: ";
  print_expr_type();
  cout << ")";
}
} // namespace hol2cpp