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

bool 
DatatypeDef::compare_components(std::vector<DatatypeDef::Component> &the_components,
                                std::vector<DatatypeDef::Component> &this_components) const
{
  //用int数组存储比较记录
  int the_components_res[the_components.size()] = {0};
  //先遍历非全局的
  std::vector<DatatypeDef::Component>::iterator component_ptr;
  for(component_ptr=this_components.begin(); component_ptr!=this_components.end(); ++component_ptr){
    //再遍历全局的
    std::vector<DatatypeDef::Component>::iterator the_component_ptr;
    for(the_component_ptr=the_components.begin();
        the_component_ptr!=the_components.end();
        ++the_component_ptr)
    {
      int index = distance(the_components.begin(),the_component_ptr);
      //如果全局定义中的这个定义，某个模式已经匹配了则可跳过
      if(the_components_res[index] == 1) continue;
      //都只有一个构造子，可以视为相同
      if((*component_ptr).arguments.size()==0 && (*the_component_ptr).arguments.size()==0){
        the_components_res[index] = 1;
        break;        
      }

      //存在树结构时
      DatatypeDef::Compare compare;
      //比较两个树的结构
      if(!compare.compare_structure((*component_ptr).arguments, (*the_component_ptr).arguments)){
        //两个树结构不相同时，不用再比较叶子节点
        continue;
      }

      //两个树结构相同时，比较叶子节点
      cout<<"!! compare arguments same!!!"<<endl;
      if(compare.compare_arguments((*component_ptr).arguments, (*the_component_ptr).arguments)){
        // cout<<"## arguments same"<<endl;
        // cout<<"^^ two argument size: "<<(*component_ptr).arguments.size()
        //     <<" "<<(*the_component_ptr).arguments.size()<<endl;
        the_components_res[index] = 1;
        break;
      }
    }
  }
  int res = 0;
  for(int i=0;i<(int)the_components.size();i++){
    res += the_components_res[i];
  }
  if(res == (int)the_components.size()) return true;
  else return false;
}

bool 
DatatypeDef::Compare::compare_arguments(std::vector<Ptr<Type>> &args_one, std::vector<Ptr<Type>> &args_two)
{
  //数组args是第一层的叶子节点，采用深度优先遍历将树展开成vector
  types_one = get_depth_vector(args_one);
  types_two = get_depth_vector(args_two);
  //需要比较生成数组的长度，长度一致才继续比
  if(types_one.size() != types_two.size()) return false;
  //继续比较，则初始化数组变量
  for(int i=0;i<(int)types_one.size();i++){
    class_one.push_back(-1);
    class_two.push_back(-1);
    ir_one.push_back(-1);
    ir_two.push_back(-1);
  }
  //填充类别标记数组
  for(int i=0;i<(int)types_one.size();i++){
    if((*types_one[i]).get_datatype() == "Normal"){
      class_one[i] = 0;
    }
    if((*types_one[i]).get_datatype() == "Argument"){
      class_one[i] = 1;
    }
    if((*types_two[i]).get_datatype() == "Normal"){
      class_two[i] = 0;
    }
    if((*types_two[i]).get_datatype() == "Argument"){
      class_two[i] = 1;
    }
  }

  gen_ir_one();
  gen_ir_two();
  return get_depth_res();
}
std::vector<Ptr<Type>> 
DatatypeDef::Compare::get_depth_vector(std::vector<Ptr<Type>> &args)
{
  std::vector<Ptr<Type>> typev;
  for(auto ptr=args.cbegin();ptr!=args.cend();++ptr){
    std::vector<Ptr<Type>> temp = ptr->get()->depth_traversal();
    typev.insert(typev.end(), temp.begin(), temp.end());
  }
  return typev;
}
void DatatypeDef::Compare::gen_ir_one()
{
  //识别同类别不同类型，生成中间表示
  for(int i=0;i<(int)types_one.size();i++){
    int cur_type_id = 1;
    //第一个元素，之前没有可查的
    if(i==0){
      ir_one[i] = cur_type_id;
      continue;
    }
    //非第一个元素都需要向前遍历比较
    for(int j=0; j<i; j++){
      //只有类别相同才有必要继续比
      if((*types_one[i]).get_datatype() == (*types_one[j]).get_datatype()){
        if((*types_one[i]).main_name() == (*types_one[j]).main_name()){
          ir_one[i] = ir_one[j];
          break; //找到类型相同的就可以停止了
        }
        else{
          //当前类型不一定是最大，需要比较一下
          if(cur_type_id <= ir_one[j]) cur_type_id = ir_one[j]+1; //不用停止，需要找最大类型数
          //如果cur比当前类型数还大，那不用做处理
        }
        //这样循环完成时cur才能比最大类型数大1
      }
    }
    //向前寻找完成，如果是新类型甚至新类别
    if(ir_one[i] == -1){
      ir_one[i] = cur_type_id;
    }
  }
}
void DatatypeDef::Compare::gen_ir_two(){
  //识别同类别不同类型，生成中间表示
  for(int i=0;i<(int)types_two.size();i++){
    int cur_type_id = 1;
    //第一个元素，之前没有可查的
    if(i==0){
      ir_two[i] = cur_type_id;
      continue;
    }
    //非第一个元素都需要向前遍历比较
    for(int j=0; j<i; j++){
      //只有类别相同才有必要继续比
      if((*types_two[i]).get_datatype() == (*types_two[j]).get_datatype()){
        if((*types_two[i]).main_name() == (*types_two[j]).main_name()){
          ir_two[i] = ir_two[j];
          break; //找到类型相同的就可以停止了
        }
        else{
          //当前类型不一定是最大，需要比较一下
          if(cur_type_id <= ir_two[j]) cur_type_id = ir_two[j]+1; //不用停止，需要找最大类型数
          //如果cur比当前类型数还大，那不用做处理
        }
        //这样循环完成时cur才能比最大类型数大1
      }
    }
    //向前寻找完成，如果是新类型甚至新类别
    if(ir_two[i] == -1){
      ir_two[i] = cur_type_id;
    }
  }
}
bool DatatypeDef::Compare::get_depth_res()
{
  for(int i=0;i<(int)types_one.size();i++){
    if(class_one[i] != class_two[i]){
      break;
    }
    if(ir_one[i] != ir_two[i]){
      break;
    }
    if(i == (int)(types_one.size()-1)){
      depth_res = true;
    }
  }

  // if(res){
    cout<<"### class one: ";
    for(int i=0;i<(int)types_one.size();i++){
      cout<<class_one[i]<<" ";
    } 
    cout<<"class two: ";
    for(int i=0;i<(int)types_one.size();i++){
      cout<<class_two[i]<<" ";
    }  
    cout<<endl;
    cout<<"### type one: ";
    for(int i=0;i<(int)types_one.size();i++){
      cout<<ir_one[i]<<" ";
    } 
    cout<<"type two: ";
    for(int i=0;i<(int)types_one.size();i++){
      cout<<ir_two[i]<<" ";
    }  
    cout<<endl;
  // }
  return depth_res;
}

bool
DatatypeDef::Compare::compare_structure(std::vector<Ptr<Type>>& args_one, 
                                        std::vector<Ptr<Type>>& args_two)
{
  //只有arguments的长度一样才用继续比
  if(args_one.size() != args_two.size()) return false;
  //初始化子树数组
  subtree_one = get_width_queue(args_one);
  subtree_two = get_width_queue(args_two);
  //开始广度优先遍历
  subtree_one = get_width_traversual(subtree_one);
  subtree_two = get_width_traversual(subtree_two);
  //对广度遍历数组进行比较
  return get_width_res();
}
std::vector<Ptr<Type>>
DatatypeDef::Compare::get_width_queue(std::vector<Ptr<Type>> args)
{
  // cout<<"@ args size: "<<args.size()<<endl;
  std::vector<Ptr<Type>> tempv;
  for(auto ptr=args.cbegin(); ptr!=args.cend();++ptr){
    tempv.push_back(ptr->get()->clone());
    // cout<<"  push third. tempv size: "<<tempv.size()<<endl;
  }
  return tempv;
}
std::vector<Ptr<Type>>
DatatypeDef::Compare::get_width_traversual(std::vector<Ptr<Type>> args)
{
  //将所有子树全部拆分整合到同一层级
  // cout<<"# subtree vector sub_num: ";
  int pos = 0;
  while(true){
    if(pos >= (int)(args.size())) break;
    // cout<<args[pos].get()->args_num()<<" ";
    if(args[pos].get()->args_num() > 0){
      std::vector<Ptr<Type>> temp = args[pos].get()->width_traversal();
      args.insert(args.end(), temp.begin(), temp.end());
    }
    pos++;
  }
  // cout<<endl;
  return args;
}
bool DatatypeDef::Compare::get_width_res()
{
  //比较两个生成数组的长度
  if(subtree_one.size() != subtree_two.size()) return false;
  //比较所有子树的节点数量
  for(int i=0;i<(int)subtree_one.size();i++){
    if(subtree_one[i]->args_num() != subtree_two[i]->args_num()){
      return false;
    }
  }
  //所有子树全部遍历完成
  width_res = true;
  return true;
}
} // namespace hol2cpp
