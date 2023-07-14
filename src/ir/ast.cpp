#include "ast.hpp"

#define assert_true(expr) assert_impl(expr, CodegenError)

using namespace std;

namespace hol2cpp {
Type::~Type() = default;
Expr::~Expr() = default;
Definition::~Definition() = default;

NormalType::NormalType(string name)
  : name(move(name))
{
  // ...
}

ArgumentType::ArgumentType(string name)
  : name(move(name))
{
  // ...
}

TemplateType::TemplateType(string name)
  : name(move(name))
{
  // ...
}

TemplateType::TemplateType(string name, Ptr<Type>&& arg)
  : name(move(name))
{
  args.push_back(move(arg));
}

TemplateType::TemplateType(string name, vector<Ptr<Type>>&& args)
  : name(move(name))
  , args(move(args))
{
  // ...
}

IntegralExpr::IntegralExpr(string value)
  : value(move(value))
{
  // ...
}

VarExpr::VarExpr(string name)
  : name(move(name))
{
  // ...
}

ConsExpr::ConsExpr(string constructor)
  : constructor(move(constructor))
{
  // ...
}

ConsExpr::ConsExpr(string constructor, Ptr<Expr> lhs, Ptr<Expr> rhs)
  : constructor(move(constructor))
{
  args.push_back(move(lhs));
  args.push_back(move(rhs));
}

SetExpr::SetExpr() = default;

SetExpr::SetExpr(vector<Ptr<Expr>>&& exprs)
  : exprs(move(exprs))
{
  // ...
}

BinaryOpExpr::BinaryOpExpr(Token op, Ptr<Expr>&& lhs, Ptr<Expr>&& rhs)
  : op(move(op))
  , lhs(move(lhs))
  , rhs(move(rhs))
{
  // ...
}

LetinExpr::LetinExpr(Equation&& equation)
  : equation(move(equation))
{
  // ...
}

CaseExpr::CaseExpr(Ptr<Expr>&& expr)
  : expr(move(expr))
{
  // ...
}

void
Definition::gen_code(Code&) const
{
  throw;
}

ErrorDefinition::ErrorDefinition(PEType type)
  : type(type)
{
  // ...
}

bool
ErrorDefinition::is_error() const
{
  return true;
}

bool
ErrorDefinition::is_datatype_decl() const
{
  return type == PEType::Datatype;
}

bool
ErrorDefinition::is_function_decl() const
{
  return type == PEType::Function;
}

string
DatatypeDef::def_name() const
{
  return decl_type->main_name();
}

bool
DatatypeDef::is_datatype_decl() const
{
  return true;
}

int 
DatatypeDef::get_type() const {
  if(typeid(this->decl_type) == typeid(NormalType)) return 1;
  if(typeid(this->decl_type) == typeid(ArgumentType)) return 2;
  if(typeid(this->decl_type) == typeid(TemplateType)) return 3;
  return 0;
}
bool
DatatypeDef::compare_type (Ptr<Type> type_1, Ptr<Type> type_2) const {
  if(typeid(type_1) == typeid(type_2)) return true;
  else return false;
}
bool
DatatypeDef::compare_components(vector<Component> com_1, vector<Component> com_2) const{
  //First, compare the size of the components vector.
  if(com_1.size() != com_2.size()) return false;
  //Then, same size, compare the content of the components.
  std::vector<Component>::iterator it_com_1;
  std::vector<Component>::iterator it_com_2;
  for(it_com_1=com_1.begin(); it_com_1!=com_1.end(); it_com_1++){
    for(it_com_2=com_2.begin(); it_com_2!=com_2.end(); it_com_2++){
      //Component variable constructor affect nothing, so ignore.
      if(compare_arguments(it_com_1->arguments,it_com_2->arguments)){
        //If arguments same, should do something, but temporarily don't know do what.
        continue;
      }
    }
  }
  return false;
}
bool
DatatypeDef::compare_arguments(vector<Ptr<Type>> argu_1, vector<Ptr<Type>> argu_2) const{
  std::vector<Ptr<Type>>::iterator it_argu_1;
  std::vector<Ptr<Type>>::iterator it_argu_2;
  for(it_argu_1=argu_1.begin(); it_argu_1!=argu_1.end(); it_argu_1++){
    for(it_argu_2=argu_2.begin(); it_argu_2!=argu_2.end(); it_argu_2++){

    }

  }
  return false;
}

string
FunctionDef::def_name() const
{
  return name;
}

bool
FunctionDef::is_function_decl() const
{
  return true;
}

ShortDef::ShortDef() = default;

ShortDef::ShortDef(string name, vector<std::string> params, Ptr<Expr> expr)
  : name(move(name))
  , parameters(move(params))
  , expr(move(expr))
{
  // ...
}

string
ShortDef::def_name() const
{
  return name;
}

Type*
FuncType::result_type() const
{
  assert_true(!types.empty());
  return types.back().get();
}
} // namespace hol2cpp
