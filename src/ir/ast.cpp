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
std::vector<Ptr<Type>> NormalType::depth_traversal()
{
  auto ptr = make_shared<NormalType>(*this);
  std::vector<Ptr<Type>> typev;
  typev.push_back(ptr);
  return typev;
}
std::vector<Ptr<Type>> ArgumentType::depth_traversal()
{
  auto ptr = make_shared<ArgumentType>(*this);
  std::vector<Ptr<Type>> typev;
  typev.push_back(ptr);
  return typev;
}
std::vector<Ptr<Type>> TemplateType::depth_traversal()
{
  std::vector<Ptr<Type>> typev;
  for(auto ptr=args.cbegin();ptr!=args.cend();++ptr){
    std::vector<Ptr<Type>> temp = ptr->get()->depth_traversal();
    typev.insert(typev.end(), temp.begin(), temp.end());
  }
  return typev;
}
std::vector<Ptr<Type>> FuncType::depth_traversal()
{
  std::vector<Ptr<Type>> typev;
  for(auto ptr=types.cbegin();ptr!=types.cend();++ptr){
    std::vector<Ptr<Type>> temp = ptr->get()->depth_traversal();
    typev.insert(typev.end(), temp.begin(), temp.end());
  }
  return typev;
}
std::vector<Ptr<Type>> NormalType::width_traversal()
{
  std::vector<Ptr<Type>> temp;
  return temp;
}
std::vector<Ptr<Type>> ArgumentType::width_traversal()
{
  std::vector<Ptr<Type>> temp;
  return temp;
}
std::vector<Ptr<Type>> TemplateType::width_traversal()
{
  std::vector<Ptr<Type>> temp;
  for(auto ptr=args.cbegin();ptr!=args.cend();++ptr){
    temp.push_back(ptr->get()->clone());
  }
  return temp;
}
std::vector<Ptr<Type>> FuncType::width_traversal()
{
  std::vector<Ptr<Type>> temp;
  for(auto ptr=types.cbegin();ptr!=types.cend();++ptr){
    temp.push_back(ptr->get()->clone());
  }
  return temp;
}
int NormalType::args_num()
{
  return 0;
}
int ArgumentType::args_num()
{
  return 0;
}
int TemplateType::args_num()
{
  return this->args.size();
}
int FuncType::args_num()
{
  return this->types.size();
}

void IntegralExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  return;
}
void VarExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  for(auto& one_cons : cons_map){
    if(name == one_cons.first){
      name = one_cons.second;
    }
  }
  return;
}
void ConsExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  //遍历同构的构造子，进行替换
  for(auto one_cons : cons_map){
    if(constructor == one_cons.first){
      constructor = one_cons.second;
    }
  }
  //表达式中的参数也要递归进行替换
  for(auto one_expr : args){
    one_expr->traversal_replace_cons(cons_map);
  }
}
void ListExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  for(auto one_expr : exprs){
    one_expr.get()->traversal_replace_cons(cons_map);
  }
}
void SetExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  for(auto one_expr : exprs){
    one_expr.get()->traversal_replace_cons(cons_map);
  }
}
void BinaryOpExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  lhs.get()->traversal_replace_cons(cons_map);
  rhs.get()->traversal_replace_cons(cons_map);
}
void LetinExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  equation.pattern.get()->traversal_replace_cons(cons_map);
  equation.expr.get()->traversal_replace_cons(cons_map);
  expr.get()->traversal_replace_cons(cons_map);

}
void CaseExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  expr.get()->traversal_replace_cons(cons_map);
  for(auto& one_equ : equations){
    one_equ.pattern.get()->traversal_replace_cons(cons_map);
    one_equ.expr.get()->traversal_replace_cons(cons_map);
  }
}
void LambdaExpr::traversal_replace_cons(std::map<std::string,std::string>& cons_map)
{
  expr.get()->traversal_replace_cons(cons_map);
}

} // namespace hol2cpp
