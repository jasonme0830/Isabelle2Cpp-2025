#include "../utility/config.hpp"
#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
string
IntegralExpr::gen_expr_impl(FuncEntity&, const TypeInfo&) const
{
  return value;
}

string
VarExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  // for nat
  if (name == "Zero"){
    return "0";
  }

  // for `bool`
  else if (name == "True") {
    return "true";
  } else if (name == "False") {
    return "false";
  }

  // for `list` and `option`
  else if (name == "Nil" || name == "None") {
    return typeinfo.to_str() + "()";
  }

  // for user-defined datatypes
  else if (func.code().find_datatype_by_cons(name)) {
    typeinfo.avoid_lack(func, name);

    // assume the typeinfo exits
    return "$::$()"_fs.format(typeinfo.to_str(), name);
  }

  // for variables
  else {
    auto var = func.get_variable(name);
    // movable表示最后一次使用，typeinfo代表当前变量的类型
    if (movable && typeinfo.movable()) {
      // cout<<"var: std::move($)"_fs.format(var)<<endl;
      return "std::move($)"_fs.format(var); // for move-list
    } else {
      // cout<<"var: " << var<<endl;
      return var;
    }
  }
}

string
ConsExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  // for recursive calls
  if (constructor == func.name()) {
    return gen_expr_impl_recuCall(func, typeinfo);
  }

  // for nat
  else if (constructor == "Suc") {
    assert_true(args.size() == 1);
    auto expr = args[0]->gen_expr(func);
    return enclose_if_needed(expr) + " + 1";
  }

  // for option
  else if (constructor == "Some") {
    assert_true(args.size() == 1);
    return "std::make_optional<$>($)"_fs.format(typeinfo[0].to_str(),
                                                args[0]->gen_expr(func));
  }

  // for list
  else if (constructor == "Cons") {
    return gen_expr_impl_listCons(func, typeinfo);
  } else if (constructor == "length") {
    assert_true(args.size() == 1);
    return unmove_expr(args[0]->gen_expr(func)) + ".size()";
  } else if (constructor == "take") {
    assert_true(args.size() == 2);

    auto n = args[0]->gen_expr(func);
    auto xs = args[1]->gen_expr(func);

    // if (is_moved(xs) && theConfig.move_list()) 
    if(is_moved(xs) && theConfig.close_moveStd() == false){
      auto temp_n = func.gen_temp();
      auto temp_xs = func.gen_temp();
      func.add_expr("auto $ = $;", temp_n, n);
      func.add_expr("auto $ = $;", temp_xs, xs);
      func.add_expr("$.erase(std::next($.begin(), $), $.end());",
                    temp_xs,
                    temp_xs,
                    temp_n,
                    temp_xs);
      return move_expr(temp_xs);
    }

    xs = unmove_expr(xs);
    if (theConfig.use_deque()) {
      return "$($.begin(), $.begin() + $)"_fs.format(
        typeinfo.to_str(), xs, xs, n);
    } else {
      return "$($.begin(), std::next($.begin(), $))"_fs.format(
        typeinfo.to_str(), xs, xs, n);
    }
  } else if (constructor == "drop") {
    assert_true(args.size() == 2);

    auto n = args[0]->gen_expr(func);
    auto xs = args[1]->gen_expr(func);

    // if (is_moved(xs) && theConfig.move_list()) 
    if(is_moved(xs) && theConfig.close_moveStd() == false)
    {
      auto temp_n = func.gen_temp();
      auto temp_xs = func.gen_temp();
      func.add_expr("auto $ = $;", temp_n, n);
      func.add_expr("auto $ = $;", temp_xs, xs);
      func.add_expr("$.erase($.begin(), std::next($.begin(), $));",
                    temp_xs,
                    temp_xs,
                    temp_xs,
                    temp_n);
      return move_expr(temp_xs);
    }

    xs = unmove_expr(xs);
    if (theConfig.use_deque()) {
      return "$($.begin() + $, $.end())"_fs.format(
        typeinfo.to_str(), xs, n, xs);
    } else {
      return "$(std::next($.begin(), $), $.end())"_fs.format(
        typeinfo.to_str(), xs, n, xs);
    }
  } else if (constructor == "append") {
    assert_true(args.size() == 2);

    auto l = args[0]->gen_expr(func);
    auto r = args[1]->gen_expr(func);

    auto temp0 = func.gen_temp();
    auto temp1 = func.gen_temp();
    func.add_expr("auto $ = $;", temp0, l).add_expr("auto $ = $;", temp1, r);

    if (theConfig.use_deque()) {
      func.add_expr(
        "$.insert($.end(), $.begin(), $.end());", temp0, temp0, temp1, temp1);
    } else {
      func.add_expr("$.splice($.end(), $);", temp0, temp0, temp1);
    }

    //暂时不清楚是否应该改成theConfig.close_moveStd()
    // if(func.func_gen_mode() == 2){
    if(theConfig.close_moveStd() == false){
      return move_expr(temp0);
    } else {
      return temp0;
    }
  } else if (constructor == "nth") {
    assert_true(args.size() == 2);

    auto l = args[0]->gen_expr(func);
    auto r = args[1]->gen_expr(func);

    auto temp0 = func.gen_temp();
    func.add_expr("auto $ = $;", temp0, l);

    if (theConfig.use_deque()) {
      return "$[$]"_fs.format(temp0, r);
    } else {
      return "*std::next($.begin(), $)"_fs.format(temp0, r);
    }
  } else if (constructor == "upto") {
    auto start = func.gen_temp();
    func.add_expr("auto $ = $;", start, args[0]->gen_expr(func));

    auto end = func.gen_temp();
    func.add_expr("auto $ = $;", end, args[1]->gen_expr(func));

    auto res = func.gen_temp();
    func.add_expr("$ $;", typeinfo.to_str(), res);

    auto i = func.gen_temp();
    func.add_expr("for (auto $ = $; $ <= $; ++$) {", i, start, i, end, i)
      .add_indent()
      .add_expr("$.push_back($);", res, i)
      .sub_indent()
      .add_expr("}");

    return res;
  } else if (constructor == "upt") {
    auto start = func.gen_temp();
    func.add_expr("auto $ = $;", start, args[0]->gen_expr(func));

    auto end = func.gen_temp();
    func.add_expr("auto $ = $;", end, args[1]->gen_expr(func));

    auto res = func.gen_temp();
    func.add_expr("$ $;", typeinfo.to_str(), res);

    auto i = func.gen_temp();
    func.add_expr("for (auto $ = $; $ < $; ++$) {", i, start, i, end, i)
      .add_indent()
      .add_expr("$.push_back($);", res, i)
      .sub_indent()
      .add_expr("}");

    return res;
  } 
  
  else if (constructor == "set") {
    assert_true(args.size() == 1);
    func.code().add_header("set");

    auto temp = func.gen_temp();
    auto list = args[0]->gen_expr(func);
    func.add_expr("auto $ = $;", temp, list);

    auto set_type = typeinfo.replace_with("std::set");
    return "$($.begin(), $.end())"_fs.format(set_type.to_str(), temp, temp);
  }

  // for If-expression
  else if (constructor == "If") {
    return gen_expr_impl_if(func, typeinfo);
  }

  // for pairs
  else if (constructor == "Pair") {
    assert_true(args.size() == 2);
    return "std::make_pair(" + args[0]->gen_expr(func) + ", " +
           args[1]->gen_expr(func) + ")";
  } else if (constructor == "fst") {
    assert_true(args.size() == 1);
    return "$.first"_fs.format(args[0]->gen_expr(func));
  } else if (constructor == "snd") {
    assert_true(args.size() == 1);
    return "$.second"_fs.format(args[0]->gen_expr(func));
  }

  // for user-defined datatypes
  else if (auto datatype = func.code().find_datatype_by_cons(constructor)) {
    return gen_expr_impl_datatype(func, typeinfo);
  }

  // for common calls
  else if (auto other_func = func.code().find_func_entity(constructor)) {
    return gen_expr_impl_funCall(func, typeinfo);
  }

  // for ShortDef
  else if (auto short_def = func.code().get_short_def(constructor)) {
    return gen_expr_impl_shortDef(func, typeinfo);
  }

  // else as the common call without determined function
  else {
    // todo @xubo support currying
    auto foo = func.get_variable(constructor);
    return foo + enclose(join(
                   args, [&](auto& expr) { return expr->gen_expr(func); }));
  }
}
string
ConsExpr::gen_expr_impl_recuCall(FuncEntity& func, const TypeInfo& typeinfo) const
{
  typeinfo.avoid_lack(func, constructor);
  assert_true(func.args_size() == args.size());

  string content;
  for(const Ptr<Expr> arg:args){
    //用if找出所有不用生成temp的情况
    if(arg->get_expr_class() == "Integral"){
      // content += gen_expr_impl_recuCall_noTemp(func, typeinfo, arg);
      // continue;
    }
    if(arg->get_expr_class() == "Var"){
      // content += gen_expr_impl_recuCall_noTemp(func, typeinfo, arg);
      // continue;      
    }
    if(arg->get_expr_class() == "Cons"){ 
      //进行强制类型转换
      Ptr<const ConsExpr> copy_arg = std::dynamic_pointer_cast<const ConsExpr>(arg);
      if(copy_arg->get_genTempOrNot(func, typeinfo, arg)){
        content += gen_expr_impl_recuCall_Temp(func, typeinfo, arg);
        continue;
      }
    }
    //除上述的特殊情况，其余全都不用生成临时变量
    content += gen_expr_impl_recuCall_noTemp(func, typeinfo, arg);
    // content += gen_expr_impl_recuCall_Temp(func, typeinfo, arg);
  }

  content.pop_back();
  content.pop_back();
  return constructor+enclose(content);
 
}
string
ConsExpr::gen_expr_impl_listCons(FuncEntity& func, const TypeInfo& typeinfo) const
{
  assert_true(args.size() == 2);
  auto x = args[0]->gen_expr(func);
  auto xs = args[1]->gen_expr(func);

  auto temp = func.gen_temp();
  func.add_expr("auto $ = $;", temp, xs)
    .add_expr("$.push_front($);", temp, x);

  // if (theConfig.move_list()) 
  if(theConfig.close_moveStd() == false)
  {
    return move_expr(temp);
  } else {
    return temp;
  }
}
string
ConsExpr::gen_expr_impl_if(FuncEntity& func, const TypeInfo& typeinfo) const
{
  assert_true(args.size() == 3);

  auto cond = args[0]->gen_expr(func);
  auto res = func.gen_temp();

  func.add_expr("$ $;", typeinfo.to_str(), res)
  .add_expr("if ($) {", cond)
  .add_indent();

  string right_var_one = args[1]->gen_expr(func);
  bool var_bool_one = func.find_declVar(right_var_one);
  bool temp_bool_one = right_var_one.substr(0,4).compare("temp") == 0;
  bool arg_bool_one = right_var_one.substr(0,3).compare("arg") == 0;
  int type_class_one = args[1]->expr_type->get_exprType_class();
  // cout << right_var_one << " " << var_bool_one << temp_bool_one << arg_bool_one << " " << type_class_one << endl;
  string decl_statement_one = "$ = $;";
  if((var_bool_one || temp_bool_one || arg_bool_one)&&(type_class_one == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_one = "$ = $.self();";
    }
  }

  // .add_expr("$ = $;", res, args[1]->gen_expr(func))
  func.add_expr(decl_statement_one, res, right_var_one)
  .sub_indent()
  .add_expr("} else {")
  .add_indent();

  string right_var_two = args[2]->gen_expr(func);
  bool var_bool_two = func.find_declVar(right_var_two);
  bool temp_bool_two = right_var_two.substr(0,4).compare("temp") == 0;
  bool arg_bool_two = right_var_two.substr(0,3).compare("arg") == 0;
  int type_class_two = args[1]->expr_type->get_exprType_class();
  // cout << right_var_two << " " << var_bool_two << temp_bool_two << arg_bool_two << " " << type_class_two << endl;
  string decl_statement_two = "$ = $;";
  if((var_bool_two || temp_bool_two || arg_bool_two)&&(type_class_two == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_two = "$ = $.self();";
    }
  }

  // func.add_expr("$ = $;", res, args[2]->gen_expr(func))
  func.add_expr(decl_statement_two, res, right_var_two)
  .sub_indent()
  .add_expr("}");

  return res;
}
string
ConsExpr::gen_expr_impl_datatype(FuncEntity& func, const TypeInfo& typeinfo) const
{
  typeinfo.avoid_lack(func, constructor);

  //将类型构造规则内的重复变量标记为不可移动
  set<std::string> reuse_var;
  for(auto& arg : args){
    arg->remark_reuse_var_movable(reuse_var);
  }

  vector<string> arguments;
  for (auto& arg : args) {
    arguments.push_back(arg->gen_expr(func));
  }

  // //给构造函数生成临时变量
  // auto temp = func.gen_temp();
  // //临时变量右侧是构造函数，不用调用self函数
  // func.add_expr("auto $ = $::$(", temp, typeinfo.to_str(), constructor)
  //   .add_indent();
  // for (size_t i = 0; i < arguments.size(); ++i) {
  //   if (i == 0) {
  //     func.add_expr(arguments[i]);
  //   } else {
  //     func.app_last_stmt(",");
  //     func.add_expr(arguments[i]);
  //   }
  // }
  // func.sub_indent().add_expr(");");

  //构造函数不生成临时变量
  string statements;
  statements = typeinfo.to_str() +"::"+constructor + "(";
  for (size_t i = 0; i < arguments.size(); ++i) {
    if (i == 0) {
      statements += arguments[i];
    } else {
      statements += ", " + arguments[i];
    }
  }
  statements += ")";
  return statements;
}
string
ConsExpr::gen_expr_impl_funCall(FuncEntity& func, const TypeInfo& typeinfo) const
{
  auto other_func = func.code().find_func_entity(constructor);
  typeinfo.avoid_lack(func, constructor);
  assert_true(other_func->args_size() == args.size());

  string content;
  for(const Ptr<Expr> arg:args){
    //用if找出所有生成temp的情况
    if(arg->get_expr_class() == "Integral"){
      // content += gen_expr_impl_funCall_noTemp(func, typeinfo, arg);
      // continue;
    }
    if(arg->get_expr_class() == "Var"){
      // content += gen_expr_impl_funCall_noTemp(func, typeinfo, arg);
      // continue;      
    }
    if(arg->get_expr_class() == "Cons"){ 
      //进行强制类型转换
      Ptr<const ConsExpr> copy_arg = std::dynamic_pointer_cast<const ConsExpr>(arg);
      if(copy_arg->get_genTempOrNot(func, typeinfo, arg)){
        content += gen_expr_impl_funCall_Temp(func, typeinfo, arg);
        continue;
      }
    }
    //除上述的特殊情况，其余全都不用生成临时变量
    content += gen_expr_impl_funCall_noTemp(func, typeinfo, arg);
    // content += gen_expr_impl_funCall_Temp(func, typeinfo, arg);
  }

  content.pop_back();
  content.pop_back();
  return constructor+enclose(content);
}
string
ConsExpr::gen_expr_impl_shortDef(FuncEntity& func, const TypeInfo& typeinfo) const
{
  auto short_def = func.code().get_short_def(constructor);
  assert_true(short_def->parameters.size() == args.size());
  for (size_t i = 0; i < args.size(); ++i) {

    //等号右侧应该仿照判断是否使用self函数
    string right_var_one = args[i]->gen_expr(func);
    bool var_bool_one = func.find_declVar(right_var_one);
    bool temp_bool_one = right_var_one.substr(0,4).compare("temp") == 0;
    bool arg_bool_one = right_var_one.substr(0,3).compare("arg") == 0;
    int type_class_one = args[i]->expr_type->get_exprType_class();
    // cout << right_var_one << " " << var_bool_one << temp_bool_one << arg_bool_one << " " << type_class_one << endl;
    string decl_statement_one = "auto $ = $;";
    if((var_bool_one || temp_bool_one || arg_bool_one)&&(type_class_one == 0)){
      if(theConfig.close_typeCons() == false){
        decl_statement_one = "auto $ = $.self();";
      }
    }

    // func.add_expr("auto $ = $;", short_def->parameters[i], args[i]->gen_expr(func));
    func.add_expr(decl_statement_one, short_def->parameters[i], right_var_one);
  }
  return short_def->expr->gen_expr(func);
}

bool 
ConsExpr::get_genTempOrNot(FuncEntity& func, const TypeInfo& typeinfo, const Ptr<Expr> arg) const
{
  Ptr<const ConsExpr> copy_arg = std::dynamic_pointer_cast<const ConsExpr>(arg);
  std::string arg_cons = copy_arg->get_constructor();
  // 原则是尽量不生成临时变量
  // for recursive calls
  if (arg_cons == func.name()) {
    return true;
  }  else if (arg_cons == "Suc") {
    return false;
  }  else if (arg_cons == "Some") {
    assert_true(args.size() == 1);
    return true;
  }
  else if (arg_cons == "Cons") {
    return true;
    // return false;
  } else if (arg_cons == "length") {
    assert_true(args.size() == 1);
    return true;
  } else if (arg_cons == "take") {
    assert_true(args.size() == 2);
    // return true;
    return false;
  } else if (arg_cons == "drop") {
    assert_true(args.size() == 2);
    // return true;
    return false;
  } else if (arg_cons == "append") {
    assert_true(args.size() == 2);
    return true;
  } else if (arg_cons == "nth") {
    assert_true(args.size() == 2);
    return true;
  } else if (arg_cons == "upto") {
    return true;
  } else if (arg_cons == "upt") {
    return true;
  } else if (arg_cons == "set") {
    assert_true(args.size() == 1);
    return true;
  }  else if (arg_cons == "If") {
    return true;
  } else if (arg_cons == "Pair") {
    assert_true(args.size() == 2);
    return true;
  } else if (arg_cons == "fst") {
    assert_true(args.size() == 1);
    return true;
  } else if (arg_cons == "snd") {
    assert_true(args.size() == 1);
    return true;
  }

  // for user-defined datatypes
  else if (auto datatype = func.code().find_datatype_by_cons(arg_cons)) {
    return false;
  }

  // for common calls
  else if (auto other_func = func.code().find_func_entity(arg_cons)) {
    return true;
  }

  // for ShortDef
  else if (auto short_def = func.code().get_short_def(arg_cons)) {
    return true;
  }

  // else as the common call without determined function
  else {
    return true;
  }
}
std::string
ConsExpr::gen_expr_impl_recuCall_noTemp(FuncEntity& func, const TypeInfo& typeinfo, const Ptr<Expr> arg) const
{
  std::string one_param = arg->gen_expr(func);
  switch (func.func_gen_mode())
  {
  case 2:
    break;
  case 1:
    break;
  case 0:
    break;
  default:
    break;
  }
  return one_param+", ";
}
std::string
ConsExpr::gen_expr_impl_recuCall_Temp(FuncEntity& func, const TypeInfo& typeinfo, const Ptr<Expr> arg) const
{
  if(func.func_gen_mode() == 0){
    return arg->gen_expr(func)+", ";
  }

  auto temp = func.gen_temp();

  //等号右侧是函数的参数，应该使用self函数
  string right_var_one = arg->gen_expr(func);
  bool var_bool_one = func.find_declVar(right_var_one);
  bool temp_bool_one = right_var_one.substr(0,4).compare("temp") == 0;
  bool arg_bool_one = right_var_one.substr(0,3).compare("arg") == 0;
  int type_class_one = arg->expr_type->get_exprType_class();
  // cout << right_var_one << " " << var_bool_one << temp_bool_one << arg_bool_one << " " << type_class_one << endl;
  string decl_statement_one = "auto $ = $;";
  if((var_bool_one || temp_bool_one || arg_bool_one)&&(type_class_one == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_one = "auto $ = $.self();";
    }
  }

  // func.add_expr("auto $ = $;", temp, arg->gen_expr(func));
  func.add_expr(decl_statement_one, temp, right_var_one);

  if(theConfig.close_moveStd() == false){
    //如果等号右侧是可以移动，那么当前返回的临时变量也可以移动
    // if(is_moved(right_var_one)){
    //   temp = move_expr(temp);
    // }
    //函数调用作为参数，就应该移动
    if(type_class_one == 1){
      temp = move_expr(temp);
    }
  }
  return temp+", ";
}
std::string
ConsExpr::gen_expr_impl_funCall_noTemp(FuncEntity& func, const TypeInfo& typeinfo, const Ptr<Expr> arg) const
{
  std::string one_param = arg->gen_expr(func);
  switch (func.func_gen_mode())
  {
  case 2:
    break;
  case 1:
    break;
  case 0:
    break;
  default:
    break;
  }
  return one_param+", ";
}
std::string
ConsExpr::gen_expr_impl_funCall_Temp(FuncEntity& func, const TypeInfo& typeinfo, const Ptr<Expr> arg) const
{
  if(func.func_gen_mode() == 0){
    return arg->gen_expr(func)+", ";
  }

  auto temp = func.gen_temp();
  //等号右侧是函数的参数，应该使用self函数
  string right_var_one = arg->gen_expr(func);
  bool var_bool_one = func.find_declVar(right_var_one);
  bool temp_bool_one = right_var_one.substr(0,4).compare("temp") == 0;
  bool arg_bool_one = right_var_one.substr(0,3).compare("arg") == 0;
  int type_class_one = arg->expr_type->get_exprType_class();
  // cout << right_var_one << " " << var_bool_one << temp_bool_one << arg_bool_one << " " << type_class_one << endl;
  string decl_statement_one = "auto $ = $;";
  if((var_bool_one || temp_bool_one || arg_bool_one)&&(type_class_one == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_one = "auto $ = $.self();";
    }
  }

  // func.add_expr("auto $ = $;", temp, arg->gen_expr(func));
  func.add_expr(decl_statement_one, temp, right_var_one);

  if(theConfig.close_moveStd() == false){
    //如果等号右侧是可以移动，那么当前返回的临时变量也可以移动
    // if(is_moved(right_var_one)){
      // temp = move_expr(temp);
    // }
    //函数调用作为参数，就应该移动
    if(type_class_one == 1){
      temp = move_expr(temp);
    }
  }
  return temp+", ";
}

std::string
ConsExpr::get_constructor()const{
  return constructor;
}

string
ListExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  return typeinfo.to_str() +
         enclose<'{', '}'>(
           join(exprs, [&](const auto& expr) { return expr->gen_expr(func); }));
}

string
SetExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  return typeinfo.to_str() +
         enclose<'{', '}'>(
           join(exprs, [&](const auto& expr) { return expr->gen_expr(func); }));
}

string
BinaryOpExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  static const map<Token::Type, string> bop_mapping{
    { Token::Type::And, "&&" },   { Token::Type::Or, "||" },
    { Token::Type::Equiv, "==" }, { Token::Type::NotEq, "!=" },

    { Token::Type::Le, "<=" },    { Token::Type::Lt, "<" },
    { Token::Type::Ge, ">=" },    { Token::Type::Gt, ">" },

    { Token::Type::Add, "+" },    { Token::Type::Sub, "-" },
    { Token::Type::Mul, "*" },    { Token::Type::Div, "/" },
    { Token::Type::Mod, "%" },
  };

  auto l = lhs->gen_expr(func);
  auto r = rhs->gen_expr(func);

  auto lv = func.gen_temp();
  auto rv = func.gen_temp();

  //判断l和r是否需要调用self函数
  bool var_bool_l = func.find_declVar(l);
  bool temp_bool_l = l.substr(0,4).compare("temp") == 0;
  bool arg_bool_l = l.substr(0,3).compare("arg") == 0;
  int type_class_l = lhs->expr_type->get_exprType_class();
  string decl_statement_l = "auto $ = $;";
  if((var_bool_l || temp_bool_l || arg_bool_l)&&(type_class_l == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_l = "auto $ = $.self();";
    }
  }
  bool var_bool_r = func.find_declVar(r);
  bool temp_bool_r = r.substr(0,4).compare("temp") == 0;
  bool arg_bool_r = r.substr(0,3).compare("arg") == 0;
  int type_class_r = rhs->expr_type->get_exprType_class();
  string decl_statement_r = "auto $ = $;";
  if((var_bool_r || temp_bool_r || arg_bool_r)&&(type_class_r == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_r = "auto $ = $.self();";
    }
  }

  switch (op.type) {
    case Token::Type::Inter: {
      // auto l = lhs->gen_expr(func);
      // auto r = rhs->gen_expr(func);

      // auto lv = func.gen_temp();
      // auto rv = func.gen_temp();
      // func.add_expr("auto $ = $;", lv, l)
        // .add_expr("auto $ = $;", rv, r)
        // .add_expr("$.merge($);", lv, rv)
      func.add_expr(decl_statement_l, lv, l)
        .add_expr(decl_statement_r, rv, r)
        .add_expr("$.merge($);", lv, rv)
      ;
      return rv;
    }
    case Token::Type::Union: {
      // auto l = lhs->gen_expr(func);
      // auto r = rhs->gen_expr(func);

      // auto lv = func.gen_temp();
      // auto rv = func.gen_temp();

      // func.add_expr("auto $ = $;", lv, l)
      //   .add_expr("auto $ = $;", rv, r)
      //   .add_expr("$.merge($);", lv, rv)
      func.add_expr(decl_statement_l, lv, l)
        .add_expr(decl_statement_r, rv, r)
        .add_expr("$.merge($);", lv, rv)
      ;
      return lv;
    }
    case Token::Type::SubsetEq:
      break;
    case Token::Type::Subset:
      break;
    case Token::Type::SupsetEq:
      break;
    case Token::Type::Supset:
      break;
    case Token::Type::In:
      return "$.count($)"_fs.format(rhs->gen_expr(func), lhs->gen_expr(func));
    case Token::Type::NotIn:
      return "!$.count($)"_fs.format(rhs->gen_expr(func), lhs->gen_expr(func));

    case Token::Type::Pow:
      func.code().add_header("cmath");
      return "std::pow($, $)"_fs.format(lhs->gen_expr(func),
                                        rhs->gen_expr(func));

    /**
     * WARN: BE CAREFUL HERE!
     * `== Nil` is special
     * binary_tree: "right=Tip" also special
     */
    case Token::Type::Equiv:
      if (auto val_expr = dynamic_cast<VarExpr*>(lhs.get())) {
        if (val_expr->name == "Nil") {
          return "$.empty()"_fs.format(unmove_expr(rhs->gen_expr(func)));
        }
      }

      if (auto val_expr = dynamic_cast<VarExpr*>(rhs.get())) {
        if (val_expr->name == "Nil") {
          return "$.empty()"_fs.format(unmove_expr(lhs->gen_expr(func)));
        }

        // for binary_tree
        if (auto datatype = func.code().find_datatype_by_cons(val_expr->name)){
          return "$.is_$()"_fs.format(unmove_expr(lhs->gen_expr(func)), val_expr->name);
        }
      }

    default: {
      assert_true(bop_mapping.count(op.type));

      return "$ $ $"_fs.format(enclose_if_needed(unmove_expr(lhs->gen_expr(func))),
                               bop_mapping.at(op.type),
                               enclose_if_needed(unmove_expr(rhs->gen_expr(func))));
    }
  }
  throw CodegenError(
    "failed call BinaryOpExpr::gen_expr(...): unsupported binary operator " +
    op.value);
}

string
CaseExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  auto temp0 = func.gen_temp();
  func.add_expr("auto $ = ([&] {", temp0).add_indent();

  auto e = expr->gen_expr(func);
  auto temp1 = func.gen_temp();

  //判断等号右侧是否调用self函数
  bool var_bool_one = func.find_declVar(e);
  bool temp_bool_one = e.substr(0,4).compare("temp") == 0;
  bool arg_bool_one = e.substr(0,3).compare("arg") == 0;
  int type_class_one = expr->expr_type->get_exprType_class();
  // cout << e << " " << var_bool_one << temp_bool_one << arg_bool_one << " " << type_class_one << endl;
  string decl_statement_one = "auto $ = $;\n";
  if((var_bool_one || temp_bool_one || arg_bool_one)&&(type_class_one == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_one = "auto $ = $.self();\n";
    }
  } 

  // func.add_expr("auto $ = $;\n", temp1, e);
  func.add_expr(decl_statement_one, temp1, e);

  auto is_last_equation = func.is_last_equation(); // for reduce-cond
  for (size_t i = 0; i < equations.size(); ++i) {
    func.is_last_equation(i == equations.size() - 1); // for reduce-cond

    if (i != 0) {
      func.app_last_stmt("\n");
    }
    auto condition_cnt = func.condition_count();
    func.add_expr("// $", equations[i].raw_str);

    equations[i].pattern->gen_pattern(func, temp1);
    func.close_pattern(); // for reduce-cond

    func.add_expr("return $;", equations[i].expr->gen_expr(func));
    func.close_sub_equation(condition_cnt);
  }
  func.is_last_equation(is_last_equation); // for reduce-cond

  if (func.statements().back().back().back() == '}') {
    func.app_last_stmt(" else { // auto-generated for -Wreturn-type");
    func.add_indent().add_expr("std::abort();").sub_indent().add_expr("}");
  }

  func.sub_indent().add_expr("})();");

  return temp0;
}

string
LetinExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  auto temp = func.gen_temp();

  //判断等号右侧是否调用self函数
  string right_var_one = equation.expr->gen_expr(func);
  bool var_bool_one = func.find_declVar(right_var_one);
  bool temp_bool_one = right_var_one.substr(0,4).compare("temp") == 0;
  bool arg_bool_one = right_var_one.substr(0,3).compare("arg") == 0;
  int type_class_one = equation.expr->expr_type->get_exprType_class();
  // cout << right_var_one << " " << var_bool_one << temp_bool_one << arg_bool_one << " " << type_class_one << endl;
  string decl_statement_one = "auto $ = $;";
  if((var_bool_one || temp_bool_one || arg_bool_one)&&(type_class_one == 0)){
    if(theConfig.close_typeCons() == false){
      decl_statement_one = "auto $ = $.self();";
    }
  }
  // func.add_expr("auto $ = $;", temp, equation.expr->gen_expr(func));
  func.add_expr(decl_statement_one, temp, right_var_one);

  equation.pattern->gen_pattern(func, temp);
  func.close_pattern(); // for reduce-cond
  return expr->gen_expr(func);
}

string
LambdaExpr::gen_expr_impl(FuncEntity& func, const TypeInfo& typeinfo) const
{
  assert_true(!typeinfo.lack());
  assert_true(typeinfo.is_function());

  // todo @xubo support currying
  assert_true(typeinfo.args_size() == parameters.size());

  func.code().add_header("functional");

  string params;
  for (size_t i = 0; i < parameters.size(); ++i) {
    if (i != 0) {
      params += ", ";
    }
    params += typeinfo[i].to_str_as_arg(func.func_gen_mode()) + parameters[i];
  }

  auto temp = func.gen_temp();
  func.add_expr("std::function $ = [=] ($) {", temp, params)
    .add_indent()
    .add_expr("return $;", expr->gen_expr(func))
    .sub_indent()
    .add_expr("};");

  return temp;
}
} // namespace hol2cpp
