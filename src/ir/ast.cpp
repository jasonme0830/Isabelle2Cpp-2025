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

//code by myk
std::string NormalType::get_datatype() const
{
  return "Normal";
}
std::string ArgumentType::get_datatype() const
{
  return "Argument";
}
std::string TemplateType::get_datatype() const
{
  return "Template";
}
std::string FuncType::get_datatype() const
{
  return "Func";
}

bool DatatypeDef::compare_component(DatatypeDef::Component com_one, DatatypeDef::Component com_two) const
{
  //components里面就两个元素，string的变量和type的指针，只比较type.
  std::vector<Ptr<Type>>::iterator type_ptr_one, type_ptr_two;
  //遍历比较两个vector之前先比较两个vector的元素数量
   if(com_one.arguments.size() != com_two.arguments.size()) return false;
  //先遍历一个vector，谁先谁后无所谓
  for(type_ptr_one=com_one.arguments.begin();type_ptr_one!=com_one.arguments.end();++type_ptr_one){
    //再遍历一个vector
    for(type_ptr_two=com_two.arguments.begin();type_ptr_two!=com_two.arguments.end();++type_ptr_two){
      if(compare_type(*type_ptr_one, *type_ptr_two)){
        //should do something
      }
    }
  }
  return false;
}
bool DatatypeDef::compare_components(std::vector<DatatypeDef::Component> &the_components) const
{
  //用int数组存储比较记录
  int components_res[components.size()] = {0};
  //先遍历非全局的
  std::vector<DatatypeDef::Component>::iterator component_ptr;
  for(component_ptr=components.begin(); component_ptr!=components.end(); ++component_ptr){
    //再遍历全局的
    std::vector<DatatypeDef::Component>::iterator the_component_ptr;
    for(the_component_ptr=the_components.begin();
        the_component_ptr!=the_components.end();
        ++the_component_ptr)
    {
      if(compare_component(*component_ptr, *the_component_ptr)){
        int index = distance(components.begin(),component_ptr);
        components_res[index] = 1;
      } 
    }
  }
  int res = 0;
  for(int i=0;i<(int)components.size();i++){
    res += components_res[i];
  }
  if(res == (int)components.size()) return true;
  else return false;
}
bool DatatypeDef::compare_type(Ptr<Type> ptr_one, Ptr<Type> ptr_two) const
{
  //比较两个指针所指向的变量类型是否一致
  if(ptr_one->get_datatype() != ptr_two->get_datatype()) return false;
  //类型一致才能继续比较
  if(decl_type->get_datatype() == "Normal"){
    cout<<"####### normalType"<<endl;
    return false;
  }
  else if(decl_type->get_datatype() == "Argument"){
    cout<<"####### argumentType"<<endl;
    return false;
  }
  else if(decl_type->get_datatype() == "Template"){
    cout<<"####### templateType"<<endl;
    return false;
  }
  else{
    cout<<"####### typeid bu guan yong"<<endl;
    return false;
  }
}

} // namespace hol2cpp
