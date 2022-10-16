#include "ast.hpp"

#define assert_true(expr) assert_impl(expr, CodegenError)

using namespace std;

namespace hol2cpp {
Type::~Type() = default;
Expr::~Expr() = default;
Definition::~Definition() = default;

NormalType::NormalType(string name)
  : name(move(name)) {
    // ...
}

ArgumentType::ArgumentType(string name)
  : name(move(name)) {
    // ...
}

TemplateType::TemplateType(string name)
  : name(move(name)) {
    // ...
}

TemplateType::TemplateType(string name, Ptr<Type> &&arg)
  : name(move(name)) {
    args.push_back(move(arg));
}

TemplateType::TemplateType(string name, vector<Ptr<Type>> &&args)
  : name(move(name)), args(move(args)) {
    // ...
}

IntegralExpr::IntegralExpr(string value)
  : value(move(value)) {
    // ...
}

VarExpr::VarExpr(string name)
  : name(move(name)) {
    // ...
}

ConsExpr::ConsExpr(string constructor)
  : constructor(move(constructor)) {
    // ...
}

ConsExpr::ConsExpr(string constructor, Ptr<Expr> lhs, Ptr<Expr> rhs)
  : constructor(move(constructor)) {
    args.push_back(move(lhs));
    args.push_back(move(rhs));
}

SetExpr::SetExpr() = default;

SetExpr::SetExpr(vector<Ptr<Expr>> &&exprs)
  : exprs(move(exprs)) {
    // ...
}

BinaryOpExpr::BinaryOpExpr(Token op, Ptr<Expr> &&lhs, Ptr<Expr> &&rhs)
  : op(move(op)) , lhs(move(lhs)), rhs(move(rhs)) {
    // ...
}

LetinExpr::LetinExpr(Equation &&equation)
  : equation(move(equation)) {
    // ...
}

CaseExpr::CaseExpr(Ptr<Expr> &&expr)
  : expr(move(expr)) {
    // ...
}

void Definition::gen_code(Code &) const {
    throw;
}

ErrorDefinition::ErrorDefinition(PEType type)
  : type(type) {
    // ...
}

bool ErrorDefinition::is_error() const {
    return true;
}

bool ErrorDefinition::is_datatype_decl() const {
    return type == PEType::Datatype;
}

bool ErrorDefinition::is_function_decl() const {
    return type == PEType::Function;
}

string DatatypeDef::def_name() const {
    return decl_type->main_name();
}

bool DatatypeDef::is_datatype_decl() const {
    return true;
}

string FunctionDef::def_name() const {
    return name;
}

bool FunctionDef::is_function_decl() const {
    return true;
}

ShortDef::ShortDef() = default;

ShortDef::ShortDef(string name, vector<std::string> params, Ptr<Expr> expr)
  : name(move(name)), parameters(move(params)), expr(move(expr)) {
    // ...
}

string ShortDef::def_name() const {
    return name;
}

Type *FuncType::result_type() const {
    assert_true(!types.empty());
    return types.back().get();
}
} // namespace hol2cpp
