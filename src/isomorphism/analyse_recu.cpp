#include "isomorphism.hpp"

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
        // cout << "* one function " << endl;
        function.analyse_func_recu_class();
      }
    }
    else{
      // cout << " not function " << endl;
    }
  }
}


int
FunctionDef::analyse_func_recu_class()
{
  if(theConfig.close_move()){
    func_recursive_type = -1;
    return func_recursive_type;
  }
  if(theConfig.close_recu()){
    func_recursive_type = 0;
    return func_recursive_type;
  }


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
    func_recursive_type = 0;
    cout<<func_recursive_type<<endl;
  }

  return func_recursive_type; 
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