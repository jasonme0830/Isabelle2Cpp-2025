#include "isomorphism.hpp"

using namespace std;

namespace hol2cpp{


void Isomorphism::replace_iso_type_in_func(){
  //循环遍历definitions
  std::vector<Ptr<Definition>>::iterator ptr_def;
  for (ptr_def = thy.definitions.begin(); ptr_def != thy.definitions.end(); ++ptr_def)
  {
    //将func定义中用到同构数据类型全部替换掉
    if((*ptr_def)->is_function_decl() == true){
      //将defs.pre中的函数过滤掉
      if((*ptr_def)->is_predef() == true){
        // cout << " pre func " << endl;
        continue;
      }
      else{
        //只检查新加入进来的函数定义
        FunctionDef &function = dynamic_cast<FunctionDef &>(*(*ptr_def));
        // cout << "* one function " << endl;
        function.handle_isomorphism_datatype();
      }
    }
    else{
      // cout << " not function " << endl;
    }
  }
}

void
DatatypeDef::replace_iso_type_in_data(){
  std::vector<Component>::iterator ptr_component;
  for (ptr_component = components.begin(); ptr_component != components.end(); ++ptr_component){
    std::vector<Ptr<Type>>::iterator ptr_vector;
    for (ptr_vector = ptr_component->arguments.begin();
         ptr_vector != ptr_component->arguments.end();
         ++ptr_vector){
      ptr_vector->get()->replace_self_iso_type();
    }
  }
}

bool NormalType::replace_self_iso_type(){
  if(theAllDatatypeNameMap.find(main_name()) == theAllDatatypeNameMap.end()){
    return false;
  }
  //当前类型是同构类型，存储临时变量并进行类型名的替换
  if(theAllDatatypeNameMap[main_name()] != main_name()){
    name = theAllDatatypeNameMap[main_name()];
    return true;
  }
  else return false;
}
bool ArgumentType::replace_self_iso_type(){
  //类型变量不用查看
  return false;
}
bool TemplateType::replace_self_iso_type(){
  if(theAllDatatypeNameMap.find(main_name()) == theAllDatatypeNameMap.end()){
    return false;
  }
  bool result = false;
  // 首先判断自己是否是同构类型
  if(theAllDatatypeNameMap[main_name()] != main_name()){
    //进行ast中的字符替换
    name = theAllDatatypeNameMap[main_name()];
    result = true;
  }
  //接着判断参数内是否有同构类型
  std::vector<Ptr<Type>>::iterator ptr_args;
  for(ptr_args=args.begin();ptr_args!=args.end();++ptr_args){
    //递归判断同时替换
    result = result || ptr_args->get()->replace_self_iso_type();
  }
  return result;
}
bool FuncType::replace_self_iso_type(){
  //将函数类型内部递归遍历进行查找和替换
  std::vector<Ptr<Type>>::iterator ptr_types;
  for(ptr_types=types.begin();ptr_types!=types.end();++ptr_types){
    ptr_types->get()->replace_self_iso_type();
  }
  return false;
}

bool
FunctionDef::judge_isomorphism() const
{
  return false;
}
bool
FunctionDef::handle_isomorphism_datatype()
{
  Process handle_func_iso_type;
  handle_func_iso_type.func_type_size = type->args_num();

  //处理函数参数的数据类型，替换并返回同构的类型名数组
  // cout<<"^ handle head ^"<<endl;
  handle_func_iso_type.handle_head_isomor_type(type);

  //处理函数等式内同构类型的构造子
  // cout<<"^ handle body ^"<<endl;
  handle_func_iso_type.handle_body_isomor_type(equations);

  return true;
}
bool
FunctionDef::Process::handle_head_isomor_type(Ptr<FuncType> type)
{
  std::vector<Ptr<Type>>::iterator type_ptr;
  //遍历函数头部所有的输入输出类型
  for(type_ptr=type->types.begin();type_ptr!=type->types.end();++type_ptr){
    std::set<std::string> iso_types;
    //递归查找并替换当前类型中存在的同构数据类型
    bool iso_exist = type_ptr->get()->replace_self_iso_type(iso_types);
    //将当前类型存在的同构类型的构造子map提取到临时存储中
    for(const std::string& iso_type : iso_types){
      auto& one_map = theIsoDatatypeConsMap[iso_type] ;
      for(auto one_cons : one_map){
        the_all_iso_cons_map[one_cons.first] = one_cons.second;
      }
    }
  }

  return true;
}
bool 
FunctionDef::Process::handle_body_isomor_type(std::vector<Equation>& equations)
{
  for(auto equa_ptr=equations.begin();equa_ptr!=equations.end();++equa_ptr){
    //处理等号左侧模式匹配中的构造子
    handle_equa_pattern_iso_type_cons(equa_ptr->pattern);

    //处理等号右侧表达式中的构造子
    handle_equa_expr_iso_type_cons(equa_ptr->expr);
  }
  return true;
}
bool
FunctionDef::Process::handle_equa_pattern_iso_type_cons(Ptr<Expr> pattern)
{
  pattern->traversal_replace_cons(the_all_iso_cons_map);
  return true;
}
bool
FunctionDef::Process::handle_equa_expr_iso_type_cons(Ptr<Expr> expr)
{
  expr->traversal_replace_cons(the_all_iso_cons_map);
  return true;
}

bool NormalType::replace_self_iso_type(std::set<std::string>& iso_types)
{
  //当前类型是同构类型，存储临时变量并进行类型名的替换
  if(theAllDatatypeNameMap[main_name()] != main_name()){
    iso_types.insert(main_name());
    name = theAllDatatypeNameMap[main_name()];
    return true;
  }
  else return false;
}
bool ArgumentType::replace_self_iso_type(std::set<std::string>& iso_types)
{
  //一般情况下Argument类型不存在同构数据类型
  return false;
}
bool TemplateType::replace_self_iso_type(std::set<std::string>& iso_types)
{
  bool result = false;
  // 首先判断自己是否是同构类型
  if(theAllDatatypeNameMap[main_name()] != main_name()){
    iso_types.insert(main_name());
    //进行ast中的字符替换
    name = theAllDatatypeNameMap[main_name()];
    result = true;
  }
  //接着判断参数内是否有同构类型
  std::vector<Ptr<Type>>::iterator ptr_args;
  for(ptr_args=args.begin();ptr_args!=args.end();++ptr_args){
    //递归判断同时替换
    result = result || ptr_args->get()->replace_self_iso_type(iso_types);
  }
  return result;
}
bool FuncType::replace_self_iso_type(std::set<std::string>& iso_types)
{
  //将函数类型内部递归遍历进行查找和替换
  std::vector<Ptr<Type>>::iterator ptr_types;
  for(ptr_types=types.begin();ptr_types!=types.end();++ptr_types){
    ptr_types->get()->replace_self_iso_type(iso_types);
  }
  return false;
}


}