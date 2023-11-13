#include "../utility/config.hpp"
#include "codegen.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

namespace hol2cpp {
Code
Theory::gen_code() const
{
  Code code;

  size_t predefs = 0, predefined = 0, datatype_defs = 0, function_defs = 0,
         shortdef_defs = 0;
  vector<string> datatype_gens, function_gens, shortdef_gens;

  for (size_t i = 0; i < definitions.size(); ++i) {
    auto decl = definitions[i].get();

    // pass all pre-defs
    if (decl->is_predef()) {
      ++predefs;
    } else if (decl->is_datatype_decl()) {
      ++datatype_defs;
    } else if (decl->is_function_decl()) {
      ++function_defs;
    } else {
      ++shortdef_defs;
    }

    if (!decl->is_error())
      try {
        if (decl->is_predefined()) {
          ++predefined;
        } else {
          decl->gen_code(code);
        }

        // pass pre-defs
        if (decl->is_predef()) {
          continue;
        }

        if (decl->is_datatype_decl()) {
          datatype_gens.push_back(decl->def_name());
        } else if (decl->is_function_decl()) {
          function_gens.push_back(decl->def_name());
        } else {
          shortdef_gens.push_back(decl->def_name());
        }
      } catch (const exception& e) {
        string name;
        if (auto datatype_decl = dynamic_cast<DatatypeDef*>(decl)) {
          name = "datatype " + datatype_decl->decl_type->main_name();
          code.pop_datatype();
        } else {
          auto fun_decl = static_cast<FunctionDef*>(decl);
          name = "function " + fun_decl->name;
          code.pop_function();
        }
        "$ at No.$ definition of $:\n`$\n\n"_fs.outf(
          cerr, info::light_red("codegen error"), i + 1, name, e.what());
      }
  }

  auto defs = datatype_defs + function_defs;
  "$\n`scanned $ definitions, contain $ datatypes and $ functions;\n"_fs.outf(
    cout, info::light_green("Result:"), defs, datatype_defs, function_defs);

  auto gens = datatype_gens.size() + function_gens.size();
  "`generated $ definitions, contain $ predefined:\n"_fs.outf(
    cout, gens, predefined);
  "``$ datatypes and $ functions.\n"_fs.outf(
    cout, datatype_gens.size(), function_gens.size());

  cout << "````datatypes:" << endl;
  for (auto& datatype : datatype_gens) {
    cout << datatype << ", ";
  }

  cout << "\n````functions:" << endl;
  for (auto& function : function_gens) {
    cout << function << ", ";
  }

  cout << endl;

  return code;
}

void
DatatypeDef::gen_code(Code& code) const
{
  if (!is_predef()) {
    code.add_header("variant");
  }

  auto name = decl_type->main_name();
  auto& datatype = code.entry_datatype(name);

  datatype.is_predef(is_predef());
  datatype.set_name(move(name));
  datatype.set_self(decl_type->gen_datatype(datatype));

  vector<vector<Ptr<Type>>> abstracts;
  for (size_t i = 0; i < components.size(); ++i) {
    abstracts.push_back({});
    datatype.entry_component();
    datatype.add_constructor(components[i].constructor);
    code.bind_cons(components[i].constructor, datatype);

    for (auto& type : components[i].arguments) {
      auto field_type = type->gen_datatype(datatype);
      datatype.add_field_type(field_type);
      if (field_type == datatype.self()) {
        datatype.is_recuisive(true);
        if (!datatype.is_predef()) {
          code.add_header("memory");
        }
      }
      abstracts.back().push_back(move(type));
    }
  }
  datatype.abstracts() = move(abstracts);
}

void
FunctionDef::gen_code(Code& code) const
{
  auto& func = code.entry_func_entity(name);

  func.is_predef(is_predef());
  func.nonexhaustive(nonexhaustive);
  func.memoize(memoize);
  func.name(name);

  type->gen_funcentity(func);

  if (memoize) {
    assert_true(func.typeinfos().size() > 1);
    code.add_header("map");
  }

  for (size_t i = 0; i < equations.size(); ++i) {
    func.is_last_equation(i == equations.size() - 1); // for reduce-cond

    func.entry_equation();
    func.add_expr("// $", equations[i].raw_str);
    equations[i].gen_funcentity(func);
    func.close_equation();
  }

  if (!func.statements().empty() && !func.statements().back().empty()) {
    auto& last_stmt = func.statements().back().back();
    if (last_stmt.back() == '}') {
      func.app_last_stmt(" else { // auto-generated for -Wreturn-type");
      func.add_indent().add_expr("std::abort();").sub_indent().add_expr("}");
    }
  }
}

void
ShortDef::gen_code(Code& code) const
{
  code.add_short_def(name,
                     make_unique<ShortDef>(name, move(parameters), move(expr)));
}

/**
 * @brief check the datatype is pre-defined or not
 *
 * @return true if the datatype is pre-defined
 * @return false if not
 */
bool
DatatypeDef::is_predefined() const
{
  return thePredefinedDatatypes.count(decl_type->main_name());
}
/**
 * @brief check the function is pre-defined or not
 *
 * @return true if the function is pre-defined
 * @return false if not
 */
bool
FunctionDef::is_predefined() const
{
  return thePredefinedFunctions.count(name);
}

//These definition is code by myk.
bool 
DatatypeDef::is_isomorphism() const
{
  Compare compare;
  //声明一个int数组用来存储对比信息
  int index = -1;
  std::vector<DatatypeDef>::iterator decl_ptr;
  //提前将本定义的构造规则进行排列组合
  compare.get_components_combinations(this->components);

  //遍历全局已经比较完成的数据类型定义
  for(decl_ptr=theDefinedDatatypes.begin();decl_ptr!=theDefinedDatatypes.end();++decl_ptr){
    
    cout<<"^ two datatype name: "<<this->def_name()<<" "<<decl_ptr->def_name()<<endl;
    //等号左边的类型不同直接退出
    if(!compare.compare_decl_type(decl_type,decl_ptr->decl_type)) continue;
    //接着比较components内元素的数量，不一致就跳出本次循环
    if(components.size() != decl_ptr->components.size()) continue;

    //对全局定义的规则数组进行排序
    std::vector<Component> the_base_components = compare.get_base_order_vector(decl_ptr->components);

    //遍历本定义所有排列的规则组合
    std::vector<std::vector<Component>>::iterator combina_ptr;
    for(combina_ptr = compare.components_combination.begin();
        combina_ptr != compare.components_combination.end();
        ++combina_ptr)
    {
      //输出
      // std::vector<Component>::iterator this_ptr;
      // for(this_ptr=(*combina_ptr).begin();this_ptr!=(*combina_ptr).end();++this_ptr){
      //   cout<<(*this_ptr).constructor<<" ";
      // }
      // cout<<endl;
      // for(this_ptr=the_base_components.begin();this_ptr!=the_base_components.end();++this_ptr){
      //   cout<<(*this_ptr).constructor<<" ";
      // }
      // cout<<endl;

      // cout<<"^ compare components ^"<<endl;
      if(compare.compare_components(the_base_components,(*combina_ptr))){
        //components也相同，则这两个datatypedef同构
        index = distance(theDefinedDatatypes.begin(), decl_ptr);
        //所有的排列，对应上一个就可以
        break;
      }
    }

    //利用index判断是否已经找到同构类型，找到就退出全局定义循环
    if(index != -1) break;
  }

  //不同构，则自己与自己建立对应关系
  if(index == -1){
    //用数据类型左侧类型的类型名，functype没有类型名
    if(decl_type->get_datatype() != "Func"){
      theIsomorphismDatatypeMap.insert(pair<std::string,std::string>
                                      (decl_type->main_name(),decl_type->main_name()));
    }
    else{
      //TODO:数据类型的定义中出现函数类型，处理方式待定
    }

    return false;
  }
  //与前文定义的数据类型同构，建立对应关系
  else{
    if(decl_type->get_datatype() != "Func"){
      theIsomorphismDatatypeMap.insert(pair<std::string,std::string>
                                      (decl_type->main_name(),theDefinedDatatypes[index].decl_type->main_name()));
    }
    else{
      //TODO:数据类型的定义中出现函数类型，处理方式待定
    }
    return true;
  }
}

bool
DatatypeDef::Compare::compare_decl_type(Ptr<Type> ptr_one, Ptr<Type> ptr_two)
{
  //等号左边类型不同，直接退出
  if(ptr_one->get_datatype() != ptr_two->get_datatype()) return false;
  //将等号左侧信息复制存储
  structer.decl_type_one = ptr_one->clone();
  structer.decl_type_two = ptr_two->clone();
  leafargu.decl_type_one = ptr_one->clone();
  leafargu.decl_type_two = ptr_two->clone();

  //对normal和Template采用不同的处理方式
  if(ptr_one->get_datatype() == "Normal"){
    //normal数据类型的类型名不重要，只需比较构造规则
    return true;
  }
  else if(ptr_one->get_datatype() == "Template"){
    //深度优先遍历获取ast叶子
    std::vector<Ptr<Type>> vec_one = ptr_one->depth_traversal();
    std::vector<Ptr<Type>> vec_two = ptr_two->depth_traversal();
  
    //等号左边输入参数的数量不同，直接退出
    if(vec_one.size() != vec_two.size()) return false;
    
    // //输出
    // for(int i=0;i<(int)vec_one.size();i++){
    //   cout<<vec_one[i]->main_name()<<" ";
    // }
    // cout<<"| ";
    // for(int i=0;i<(int)vec_two.size();i++){
    //   cout<<vec_two[i]->main_name()<<" ";
    // }
    // cout<<endl;

    //形成输入参数记录
    for(int i=0;i<(int)vec_one.size();i++){
      //输入参数的类型须一致
      if(vec_one[i]->get_datatype() != vec_two[i]->get_datatype()) return false;

      //将信息进行存储
      int type_class_both = -1;
      std::string type_name_one;
      std::string type_name_two;
      if(vec_one[i]->get_datatype() == "Normal"){
        type_class_both = 0;
        //normal类型比较的是对应的同构类型是否相同
        type_name_one = theIsomorphismDatatypeMap.find(vec_one[i]->main_name())->second;
        type_name_two = theIsomorphismDatatypeMap.find(vec_two[i]->main_name())->second;
        //同构类型不一致，直接退出
        if(type_name_one != type_name_two) return false;
      }
      if(vec_one[i]->get_datatype() == "Argument"){
        type_class_both = 1;
        //类型变量没有对应的同构类型，名字可以不一致
        type_name_one = vec_one[i]->main_name();
        type_name_two = vec_two[i]->main_name();
      }
    
      leafargu.decl_type_map_one.insert(std::make_pair(type_name_one, std::make_tuple(type_class_both,i,0)));
      leafargu.decl_type_map_two.insert(std::make_pair(type_name_two, std::make_tuple(type_class_both,i,0)));
    }
    return true;
  }
  else{
    //其他类型暂不考虑，直接输出false
    cout<<"Compare decl type: other decl type"<<endl;
    return false;
  }
}
std::vector<DatatypeDef::Component>
DatatypeDef::Compare::get_base_order_vector(std::vector<DatatypeDef::Component>& the_sort)
{
  std::vector<DatatypeDef::Component> order_components;
  //用来标记哪个已经被排序过了
  bool the_sort_flag[the_sort.size()] = {false};
  //用来标记当前规则的参数数量
  std::vector<int> the_sort_argu_num;
  
  //遍历，获取所有规则中的参数数量
  std::vector<DatatypeDef::Component>::iterator sort_ptr;
  for(sort_ptr=the_sort.begin();sort_ptr!=the_sort.end();++sort_ptr){
    the_sort_argu_num.push_back((int)sort_ptr->arguments.size());
  }

  //获取最大参数个数
  int the_sort_argu_num_max = *std::max_element(the_sort_argu_num.begin(),the_sort_argu_num.end());
  //从大到小，将规则排序
  for(int i=the_sort_argu_num_max; i>=0; i--){
    //每一个参数数量都进行一次遍历
    for(sort_ptr=the_sort.begin();sort_ptr!=the_sort.end();++sort_ptr){
      int index = distance(the_sort.begin(), sort_ptr);
      //如果已经被排序过则直接跳过
      if(the_sort_flag[index] == true) continue;
      //参数数量不匹配也直接跳过
      if(the_sort_argu_num[index] != i) continue;
      //参数数量匹配则插入排序并做记号
      else{
        order_components.push_back(*sort_ptr);
        the_sort_flag[index] = true;
      }
    }
  }
  //检查，排序完成的数组和原数组应该长度相等
  if(order_components.size() != the_sort.size()){
    cout<<"## the base order component vector sort failed!"<<endl;
    cout<<"length: "<<order_components.size()<<" "<<the_sort.size()<<endl;
    exit(0);
  }
  return order_components;
}
void
DatatypeDef::Compare::get_components_combinations(std::vector<DatatypeDef::Component>& the_arrange)
{
  int the_arrange_size = (int)the_arrange.size();
  std::vector<Component> the_store;
  //完成排列组合
  recursion_arrange_component(the_arrange, 0, the_arrange_size-1, the_store);
  
  //根据规则内参数数量的多少进行精简
  cutdown_components_combination_by_order();

  //输出所有排列的规则构造子
  // std::vector<std::vector<Component>>::iterator combina_ptr;
  // for(combina_ptr=components_combination.begin();combina_ptr!=components_combination.end();++combina_ptr){
  //   std::vector<Component>::iterator vector_ptr;
  //   for(vector_ptr=(*combina_ptr).begin();vector_ptr!=(*combina_ptr).end();++vector_ptr){
  //     cout<<(*vector_ptr).constructor<<" ";
  //   }
  //   cout<<endl;
  // }
  // cout<<"size: "<<components_combination.size()<<endl;
}
void
DatatypeDef::Compare::recursion_arrange_component(std::vector<DatatypeDef::Component>& the_raw,
                                                  int start_index,
                                                  int end_index,
                                                  std::vector<DatatypeDef::Component> the_store)
{
  //递归到末尾，将当前生成的规则序列存储到全局
  if(start_index == end_index+1){
    components_combination.push_back(the_store);
    return;
  }

  //将规则依次调换顺序进行排列
  for(int i=start_index; i<=end_index; i++){
    Component temp = the_raw[i];
    the_raw[i] = the_raw[start_index];
    the_raw[start_index] = temp;

    the_store.push_back(the_raw[start_index]);
    recursion_arrange_component(the_raw, start_index+1, end_index, the_store);
    the_store.pop_back();

    Component peory = the_raw[i];
    the_raw[i] = the_raw[start_index];
    the_raw[start_index] = peory;    
  }
}
void
DatatypeDef::Compare::cutdown_components_combination_by_order()
{
  //遍历所有的规则排列
  std::vector<std::vector<Component>>::iterator combina_ptr;
  for(combina_ptr=components_combination.begin();combina_ptr!=components_combination.end();){
    //标记参数数量是否降序
    bool is_argu_num_order = true;
    //找到当前规则下的参数数量
    std::vector<Component>::iterator vector_ptr;
    std::vector<Component>::iterator vector_ptr_next;
    for(vector_ptr=(*combina_ptr).begin();vector_ptr!=(*combina_ptr).end();++vector_ptr){
      vector_ptr_next = vector_ptr;
      if(++vector_ptr_next != (*combina_ptr).end()){
        //如果出现参数数量的升序，就将当前排列删除
        if((*vector_ptr).arguments.size() < (*vector_ptr_next).arguments.size()){
          is_argu_num_order = false;
          break;
        }
      }
      else break;
    }
    //出现升序就删除
    if(!is_argu_num_order){
      combina_ptr = components_combination.erase(combina_ptr);
    }
    //没有升序就继续
    else{
      ++combina_ptr;
    }
  }
}
bool 
DatatypeDef::Compare::compare_components(std::vector<DatatypeDef::Component> &the_components,
                                         std::vector<DatatypeDef::Component> &this_components) const
{
  //用int数组存储全局比较记录
  int the_components_res[the_components.size()] = {0};

  //先遍历本地的
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

      // cout<<"^^ compare structer ^^"<<endl;
      structer.initial();
      // 比较两个树的结构
      if(!structer.compare_structure((*component_ptr).arguments, (*the_component_ptr).arguments)){
        //两个树结构不相同时，不用再比较叶子节点
        // cout<<"^^^ structer not same ^^^"<<endl;
        continue;
      }

      // cout<<"^^ compare leafnode ^^"<<endl;
      leafargu.initial();
      //两个树结构相同时，比较叶子节点
      if(leafargu.compare_arguments((*component_ptr).arguments, (*the_component_ptr).arguments)){
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

void
DatatypeDef::Compare::Structer::initial()
{
  subtree_one.clear();
  subtree_two.clear();
}
bool
DatatypeDef::Compare::Structer::compare_structure(std::vector<Ptr<Type>>& args_one, 
                                                  std::vector<Ptr<Type>>& args_two)
{
  //只有arguments的长度一样才用继续比
  if(args_one.size() != args_two.size()) return false;
  //将值复制出来再操作
  subtree_one = clone_width_queue(args_one);
  subtree_two = clone_width_queue(args_two);

  //开始广度优先遍历
  subtree_one = get_width_traversual(subtree_one);
  subtree_two = get_width_traversual(subtree_two);

  //对广度遍历数组进行比较
  return get_width_res();
}
std::vector<Ptr<Type>>
DatatypeDef::Compare::Structer::clone_width_queue(std::vector<Ptr<Type>> args)
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
DatatypeDef::Compare::Structer::get_width_traversual(std::vector<Ptr<Type>> args)
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
bool 
DatatypeDef::Compare::Structer::get_width_res()
{
  //比较两个生成数组的长度
  if(subtree_one.size() != subtree_two.size()) return false;
  //比较所有子树的类型和节点数量
  for(int i=0;i<(int)subtree_one.size();i++){
    if(subtree_one[i]->get_datatype() != subtree_two[i]->get_datatype()){
      return false;
    }
    if(subtree_one[i]->args_num() != subtree_two[i]->args_num()){
      return false;
    }
  }

  //比较所有非叶子节点的名字
  for(int i=0;i<(int)subtree_one.size();i++){
    // cout<<i<<" ";
    if(subtree_one[i]->get_datatype() == "Normal"){
      //虽然也可以适用同构匹配，但是Normal类型属于叶子节点，暂定在叶子比较中处理
      continue;
    }
    else if(subtree_one[i]->get_datatype() == "Argument"){
      // cout<<"Argument"<<endl;
      continue;
    }
    else if(subtree_one[i]->get_datatype() == "Func"){
      //TODO:数据类型定义中出现函数类型，处理方式待定
      continue;
    }
    else if(subtree_one[i]->get_datatype() == "Template"){
      // cout<<"Template"<<endl;
      //右侧构造，但是左侧不是构造，出错
      if(decl_type_one->get_datatype() != "Template"){
        width_res = false;
        return false;
      }
      
      // cout<<"one type: "<<decl_type_one->main_name()<<" "<<subtree_one[i]->main_name()<<endl;
      // cout<<"two type: "<<decl_type_two->main_name()<<" "<<subtree_two[i]->main_name()<<endl;
      //情况1：递归使用自己
      if((decl_type_one->main_name()==subtree_one[i]->main_name())
       &&(decl_type_two->main_name()==subtree_two[i]->main_name())){
        //两个子树都是递归调用自己,子树类型名与左侧类型名相同
        // cout<<"^ digui Template ^"<<endl;
        continue;
      }
      //情况2：非递归，使用之前定义过的，查看是否为同构的
      else{
        // cout<<"^ isomorphism Template ^"<<endl;
        //TODO 非递归调用的情况复杂，目前只做简单处理
        if(theIsomorphismDatatypeMap[subtree_one[i]->main_name()] 
        != theIsomorphismDatatypeMap[subtree_two[i]->main_name()]){
          //不同名也不同构
          width_res = false;
          return false;
        }
      }
    }
    //四种类型之外的情况
    else{
      cout<<" other datatype:"<<subtree_one[i]->get_datatype()<<endl;
      width_res = false;
      return false;
    }
  }
  // cout<<"^ compare subtree name finished ^"<<endl;
  //所有子树全部遍历完成
  width_res = true;
  return true;
}

void
DatatypeDef::Compare::Leafargu::initial()
{
  types_one.clear();
  types_two.clear();
}
bool 
DatatypeDef::Compare::Leafargu::compare_arguments(std::vector<Ptr<Type>> &args_one, std::vector<Ptr<Type>> &args_two)
{
  //数组args是第一层的叶子节点，采用深度优先遍历将树展开成vector
  types_one = get_depth_vector(args_one);
  types_two = get_depth_vector(args_two);
  //需要比较生成数组的长度，长度一致才继续比
  if(types_one.size() != types_two.size()) return false;
  for(int i=0;i<(int)types_one.size();i++){
    //类别不同，直接退出
    if(types_one[i]->get_datatype() != types_two[i]->get_datatype()) return false;
    //类别相同，继续比较
    if(types_one[i]->get_datatype() == "Normal"){
      if(!compare_argu_normal(types_one[i],types_two[i])) return false;
    }
    if(types_one[i]->get_datatype() == "Argument"){
      if(!compare_argu_argument(types_one[i], types_two[i])) return false;
    }
  }
  return get_depth_res_both(types_one, types_two);
}
std::vector<Ptr<Type>> 
DatatypeDef::Compare::Leafargu::get_depth_vector(std::vector<Ptr<Type>> &args)
{
  std::vector<Ptr<Type>> typev;
  for(auto ptr=args.cbegin();ptr!=args.cend();++ptr){
    std::vector<Ptr<Type>> temp = ptr->get()->depth_traversal();
    typev.insert(typev.end(), temp.begin(), temp.end());
  }
  return typev;
}
bool DatatypeDef::Compare::Leafargu::compare_argu_normal(Ptr<Type> one, Ptr<Type> two)
{
  //找到该类型名的同构类型名，有可能有，也可能是它自己。
  std::string iso_name_one = theIsomorphismDatatypeMap.find(one->main_name())->second;
  std::string iso_name_two = theIsomorphismDatatypeMap.find(two->main_name())->second;
  //两类型的同构类型名应该相同
  if(iso_name_one != iso_name_two) return false;
  //同构类型名可能在等号左侧出现，也可能不出现
  auto itr_one = decl_type_map_one.find(iso_name_one);
  auto itr_two = decl_type_map_two.find(iso_name_two);
  //都没出现
  if((itr_one==decl_type_map_one.end())&&(itr_two==decl_type_map_two.end())){
    //都没在等号左侧出现，同构类型名一致即可
  }
  //都出现了
  else if((itr_one!=decl_type_map_one.end())&&(itr_two!=decl_type_map_two.end())){
    //都在等号左侧出现，normal类型不存在顺序问题
  }
  //一个出现，一个没出现
  else{
    //同构类型名相同，却一个出现一个没出现，注定不同构
    return false;
  }
  return true;
}
bool DatatypeDef::Compare::Leafargu::compare_argu_argument(Ptr<Type> one, Ptr<Type> two)
{
  auto itr_one = decl_type_map_one.find(one->main_name());
  auto itr_two = decl_type_map_two.find(two->main_name());
  //只要有一个类型变量没在等号左侧出现，直接退出
  if(itr_one==decl_type_map_one.end() || itr_two==decl_type_map_two.end()) return false;
  
  //首先查看类型变量是否已经在右侧出现过，第一个
  if(std::get<2>(itr_one->second)==0){
    //如果没有在前面的比较中出现，则利用顺序标识给出它的出现顺序
    var_type_order_one += 1;
    auto& tuple_one = itr_one->second;
    tuple_one = std::make_tuple(std::get<0>(tuple_one),std::get<1>(tuple_one),var_type_order_one);
    decl_type_map_one[one->main_name()] = tuple_one;
  }
  else{
    //如果已经在前面的比较中出现，则不做处理，直接使用出现顺序
  }

  //首先查看类型变量是否已经在右侧出现过，第二个
  if(std::get<2>(itr_two->second)==0){
    //如果没有在前面的比较中出现，则利用顺序标识给出它的出现顺序
    var_type_order_two += 1;
    auto& tuple_two = itr_two->second;
    tuple_two = std::make_tuple(std::get<0>(tuple_two),std::get<1>(tuple_two),var_type_order_one);
    decl_type_map_two[two->main_name()] = tuple_two;
  }
  else{
    //如果已经在前面的比较中出现，则不做处理，直接使用出现顺序
  }
  
  // cout<<"argument order: "<<std::get<2>(itr_one->second)<<" "<<std::get<2>(itr_two->second)<<endl;
  //比较两个类型变量在右侧的出现顺序，不一致就退出
  if(std::get<2>(itr_one->second) != std::get<2>(itr_two->second)){
    return false;
  }
  return true;
}
bool DatatypeDef::Compare::Leafargu::get_depth_res_both(std::vector<Ptr<Type>> &one,std::vector<Ptr<Type>> &two)
{

  return true;
}

} // namespace hol2cpp
