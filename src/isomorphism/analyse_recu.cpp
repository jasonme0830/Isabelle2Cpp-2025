#include "isomorphism.hpp"
#include <typeinfo>

using namespace std;

namespace hol2cpp{

void Isomorphism::analyse_func_recu_class()
{
  //循环遍历definitions
  std::vector<Ptr<Definition>>::iterator ptr_def;
  for (ptr_def = thy.definitions.begin(); ptr_def != thy.definitions.end(); ++ptr_def)
  {
    //找到所有fun的定义
    if((*ptr_def)->is_function_decl() == true){
      //将defs.pre中的函数过滤掉
      if((*ptr_def)->is_predef() == true){
        // cout << " pre func " << endl;
        continue;
      }
      else{
        //只检查新加入进来的函数定义
        FunctionDef &function = dynamic_cast<FunctionDef &>(*(*ptr_def));
        
        function.judge_func_gen_mode();
        function.analyse_func_recu_class();
        function.judge_func_mem_mode();

        // cout << "typeinfo: " << typeid(function.func_memo_mode).name() << endl;
        cout << "## recu_class: " << function.func_recursive_type << endl;
        // cout << "## recu_mode: " << function.func_recu_mode << endl;
        cout << "## gen_mode: " << function.func_gen_mode << endl;
        // cout << "## mem_mode: " << function.func_mem_mode << endl;
      }
    }
    else{
      // cout << " not function " << endl;
    }
  }
}


int
FunctionDef::judge_func_gen_mode()
{
  //0:值传递  1:move优化  2:未定义的优化
  if(theConfig.close_move()){
    func_gen_mode = 0;
  }else{
    //默认为1，开启move优化
    func_gen_mode = 1;
  }

  return func_gen_mode;
}
int
FunctionDef::analyse_func_recu_class()
{

  std::vector<Equation>::iterator ptr_equa;
  //初始化设置为非递归函数
  func_recursive_type = 0;
  //区分是否存在递归
  int all_equa_call_num = 0;  

  for (ptr_equa = equations.begin(); ptr_equa != equations.end(); ++ptr_equa)
  {
    int one_equa_call_num = ptr_equa->expr->trav_judge_recursive(this->name);
    if(one_equa_call_num >= 2){
        func_recursive_type = 2;
    }
    all_equa_call_num += one_equa_call_num;
  }

  cout << "function: " << this->def_name()<<" num: ";
  if(func_recursive_type == 2){
    cout<<func_recursive_type<<endl;
  }
  else if(all_equa_call_num > 0){
    func_recursive_type = 1;
    cout<<func_recursive_type<<endl;
  }
  else{
    func_recursive_type = 1;
    cout<<func_recursive_type<<endl;
  }

  return func_recursive_type; 
}
int
FunctionDef::judge_func_mem_mode()
{
  if(theConfig.close_memo()){
    //调用程序时的--close-memo具有最高优先级
    func_mem_mode = 0;
  }else if(memoize == true){
    //thy文件中fun定义前的memoize参数具有第2优先级
    func_mem_mode = 1;
  }else if(func_recursive_type == 2){
    //函数本身是否发生重复计算优先级最低
    int input_arg_num = type->types.size() - 1;
    cout << "---functype num: " << input_arg_num << endl;

    // std::vector<Equation>::iterator ptr_equa;
    // for (ptr_equa = equations.begin(); ptr_equa != equations.end(); ++ptr_equa){

    // }
    func_mem_mode = 1;
  }else{   
    // 必须先判断函数的递归类型，因为统计递归调用的参数时，没有办法
    // 处理if-expr中的变量数量问题，需要判断递归调用次数的时候顺便
    // 提前将if、case这种表达式的equation剔除出去
    func_mem_mode = 0;
  }
  
  return func_mem_mode;
}


int
IntegralExpr::trav_judge_recursive(std::string func_name)
{
  //表达式树结构中的叶子节点，直接返回
  return 0;
}
int
VarExpr::trav_judge_recursive(std::string func_name)
{
  //表达式树结构中的叶子节点，直接返回
  return 0; 
}
int
ConsExpr::trav_judge_recursive(std::string func_name)
{
  int cons_call = 0;
  if(constructor == func_name){
    cons_call = trav_judge_recursive_recuCons(func_name);
  }
  else if(constructor == "If"){
    cons_call = trav_judge_recursive_ifexpr(func_name);
  }
  else{
    cons_call = trav_judge_recursive_common(func_name);
  }

  return cons_call;
}
int 
ListExpr::trav_judge_recursive(std::string func_name)
{
  std::vector<Ptr<Expr>>::iterator ptr_expr;
  int all_expr_call_num = 0;
  for (ptr_expr = exprs.begin(); ptr_expr != exprs.end(); ++ptr_expr)
  {
    all_expr_call_num += ptr_expr->get()->trav_judge_recursive(func_name);
  }

  return all_expr_call_num;
}
int
SetExpr::trav_judge_recursive(std::string func_name)
{
  std::vector<Ptr<Expr>>::iterator ptr_expr;
  int all_expr_call_num = 0;
  for (ptr_expr = exprs.begin(); ptr_expr != exprs.end();++ptr_expr)
  {
    all_expr_call_num += ptr_expr->get()->trav_judge_recursive(func_name);
  }
  return all_expr_call_num;
}
int
BinaryOpExpr::trav_judge_recursive(std::string func_name)
{
  int left_call_num = 0;
  int right_call_num = 0;
  left_call_num = lhs->trav_judge_recursive(func_name);
  right_call_num = rhs->trav_judge_recursive(func_name);

  return left_call_num+right_call_num;
}
int
LetinExpr::trav_judge_recursive(std::string func_name)
{ 
  //letin表达式的equation部分，按照极端情况考虑，返回所有equation的调用次数和
  int equa_call_num_sum = equation.expr->trav_judge_recursive(func_name);

  // letin表达式的expr部分
  int expr_call_num = expr->trav_judge_recursive(func_name);

  //TODO:暂时不考虑equation中pattern和expr中的重叠问题

  return expr_call_num + equa_call_num_sum;
}
int
CaseExpr::trav_judge_recursive(std::string func_name)
{
  //case表达式的输入部分
  int expr_call_num = expr->trav_judge_recursive(func_name);
  //case表达式的case部分
  int max_equa_call_num = 0;
  std::vector<Equation>::iterator ptr_case;
  for (ptr_case = equations.begin(); ptr_case != equations.end();++ptr_case)
  {
    //case表达式中equation部分，应该选择其中最大的调用数量
    int one_equa_call_num = ptr_case->expr->trav_judge_recursive(func_name);
    if(max_equa_call_num < one_equa_call_num){
      max_equa_call_num = one_equa_call_num;
    }
  }
  //返回expr和equation中最大值的和，作为case表达式的递归调用次数
  return expr_call_num + max_equa_call_num;
}
int
LambdaExpr::trav_judge_recursive(std::string func_name)
{
  //lambda表达式的参数部分暂时不做处理

  //lambda表达式的函数体部分
  int expr_call_num = expr->trav_judge_recursive(func_name);

  return expr_call_num;
}


int
ConsExpr::trav_judge_recursive_recuCons(std::string func_name)
{
  std::vector<Ptr<Expr>>::iterator ptr_args;
  int all_args_call_num = 0;
  for (ptr_args = args.begin(); ptr_args != args.end(); ++ptr_args)
  {
    all_args_call_num += ptr_args->get()->trav_judge_recursive(func_name);
  }
  return all_args_call_num + 1;
}
int
ConsExpr::trav_judge_recursive_ifexpr(std::string func_name)
{
  int max_call_num = 0;
  std::vector<Ptr<Expr>>::iterator ptr_args;
  for(ptr_args = args.begin(); ptr_args != args.end(); ++ptr_args){
    int one_args_call_num = ptr_args->get()->trav_judge_recursive(func_name);
    if(one_args_call_num > max_call_num){
      max_call_num = one_args_call_num;
    }
  }
  return max_call_num;
}
int
ConsExpr::trav_judge_recursive_common(std::string func_name)
{
  std::vector<Ptr<Expr>>::iterator ptr_args;
  int all_args_call_num = 0;
  for (ptr_args = args.begin(); ptr_args != args.end(); ++ptr_args)
  {
    all_args_call_num += ptr_args->get()->trav_judge_recursive(func_name);
  }
  return all_args_call_num;
}

}