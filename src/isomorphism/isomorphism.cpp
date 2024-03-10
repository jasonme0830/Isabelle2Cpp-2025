#include "isomorphism.hpp"

using namespace std;

namespace hol2cpp{

Isomorphism::Isomorphism(Theory& thy)
:thy(thy)
{
  //第一次遍历，比较是否有同构的数据类型
  find_isomorphism_datatype();
  // 第二次遍历，处理函数中的同构数据类型
  replace_iso_type_in_func();
  // 第三次遍历，判别函数定义中递归函数类别
  find_rescusive_func();
}

Isomorphism::~Isomorphism() = default;

void Isomorphism::find_isomorphism_datatype(){
  //循环遍历definitions
  std::vector<Ptr<Definition>>::iterator ptr_def;
  for (ptr_def = thy.definitions.begin(); ptr_def != thy.definitions.end();++ptr_def)
  {
    if((*ptr_def)->is_datatype_decl() == true){
      DatatypeDef &datatype = dynamic_cast<DatatypeDef &>(*(*ptr_def));
      //将defs.pre中的类型添加到全局变量中
      if(datatype.is_predef() == true){
        // theNotIsoDatatypes.push_back(datatype);
        // cout << "^ insert pre datatype finished! ^" << endl;
        theAllDatatypeNameMap[datatype.decl_type->main_name()] = datatype.decl_type->main_name();
      }
      //非defs.pre中的类型进行同构比较
      else{
        // cout << "### one datatype" << endl;
        // 全局变量全部在judge内部处理完成
        if(datatype.judge_isomorphism() == false)
        {
          // theNotIsoDatatypes.push_back(datatype);
          // cout << "^^ insert new datatype finished! ^" << endl;
        }
        //之前有同构的，删掉
        else{
          ptr_def = thy.definitions.erase(ptr_def);
          --ptr_def;
          // cout << "^^ delete iso datatype finished! ^" << endl;
        }
      }
    }
  }


}

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

void Isomorphism::find_rescusive_func(){
  //循环遍历definitions
  std::vector<Ptr<Definition>>::iterator ptr_def;
  for (ptr_def = thy.definitions.begin(); ptr_def != thy.definitions.end(); ++ptr_def)
  {
    if((*ptr_def)->is_function_decl() == true){
      //跳过所有defs.pre中的函数定义
      if((*ptr_def)->is_predef() == true){
        continue;
      }
      //判断非pre的函数是否是递归函数
      else{
        FunctionDef &function = dynamic_cast<FunctionDef &>(*(*ptr_def));
        function.is_rescusive();
      }
    }
  }
}

void
Isomorphism::print_theory()
{
  for (auto& def : thy.definitions) {
    if (typeid(*def) == typeid(FunctionDef)) {
      auto& def_trans = reinterpret_cast<FunctionDef&>(*def);
      cout << def_trans.name << endl;
      for (auto& equation : def_trans.equations) {
        auto& expr = equation.expr;
        expr->print_expr();
        cout << endl;
      }
    }
  }
}

bool 
DatatypeDef::judge_isomorphism() const
{
  Compare compare;
  //store the cons of the isomorphsim type
  std::map<std::string, std::string> temp_store_cons;
  //声明一个int数组用来存储对比信息
  int index = -1;
  std::vector<DatatypeDef>::iterator decl_ptr;

  //提前将本定义的构造规则进行排列组合
  compare.get_components_combinations(this->components);
  // cout<<"^ combinate this datatypedef finished! ^"<<endl;

  //遍历全局已经检查过的数据类型定义
  for(decl_ptr=theNotIsoDatatypes.begin();decl_ptr!=theNotIsoDatatypes.end();++decl_ptr){

    //等号左边的类型不同直接退出
    if(!compare.compare_decl_type(decl_type,decl_ptr->decl_type)) continue;
    // cout<<"^ compare the decl_type finished! ^"<<endl;

    //接着比较components内元素的数量，不一致就跳出本次循环
    if(components.size() != decl_ptr->components.size()) continue;

    //对全局定义的规则数组进行排序
    std::vector<Component> the_base_components = compare.get_base_order_vector(decl_ptr->components);
    // cout<<"^ order the global datatypedef finished! ^"<<endl;

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
        index = distance(theNotIsoDatatypes.begin(), decl_ptr);
        //所有的排列，对应上一个就可以，存储当前构造子的顺序对应关系
        compare.store_cons_to_map(temp_store_cons,(*combina_ptr),the_base_components);
        break;
      }
    }

    //利用index判断是否已经找到同构类型，找到就退出全局定义循环
    if(index != -1) break;
  }

  //不同构的处理方式
  if(index == -1){
    cout << "^^ not isomorphism ^^" << endl;
    //用数据类型左侧类型的类型名，functype没有类型名
    if(decl_type->get_datatype() != "Func"){
      //添加到非同构全局变量中存储
      theNotIsoDatatypes.push_back(*this);
    }
    else{
      //TODO:数据类型的定义中出现函数类型，处理方式待定
    }

    //建立起原数据所有datatype的是否同构标记：非同构则key=value.
    theAllDatatypeNameMap[decl_type->main_name()] = decl_type->main_name();
    return false;
  }
  //是同构的处理方式
  else{
    cout<<"^^ two iso datatype name: "<<this->def_name()<<" "<<decl_ptr->def_name()<<endl;
    if(decl_type->get_datatype() != "Func"){
      //建立两个数据类型的构造子对应关系
      theIsoDatatypeConsMap[decl_type->main_name()] = move(temp_store_cons);
    }
    else{
      //TODO:数据类型的定义中出现函数类型，处理方式待定
    }

    //建立起原数据中所有datatype是否同构的标记：是同构的则key是当前name，value是全局name.
    theAllDatatypeNameMap[decl_type->main_name()] = decl_ptr->decl_type->main_name();
    return true;
  }
}

bool
DatatypeDef::Compare::compare_decl_type(Ptr<Type> ptr_cur, Ptr<Type> ptr_glo)
{
  //等号左边类型不同，直接退出
  if(ptr_cur->get_datatype() != ptr_glo->get_datatype()) return false;
  //将等号左侧信息复制存储
  structer.decl_type_cur = ptr_cur->clone();
  structer.decl_type_glo = ptr_glo->clone();
  leafargu.decl_type_cur = ptr_cur->clone();
  leafargu.decl_type_glo = ptr_glo->clone();

  //对normal和Template采用不同的处理方式
  if(ptr_cur->get_datatype() == "Normal"){
    //normal数据类型的类型名不重要，只需比较构造规则
    return true;
  }
  else if(ptr_cur->get_datatype() == "Template"){
    //深度优先遍历获取ast叶子
    std::vector<Ptr<Type>> vec_cur = ptr_cur->depth_traversal();
    std::vector<Ptr<Type>> vec_glo = ptr_glo->depth_traversal();
  
    //等号左边输入参数的数量不同，直接退出
    //TODO:等号左侧输入参数数量不同也可能同构，以右侧参数数量为准
    if(vec_cur.size() != vec_glo.size()) return false;
    
    //输出
    // for(int i=0;i<(int)vec_one.size();i++){
    //   cout<<vec_one[i]->main_name()<<" ";
    // }
    // cout<<"| ";
    // for(int i=0;i<(int)vec_two.size();i++){
    //   cout<<vec_two[i]->main_name()<<" ";
    // }
    // cout<<endl;

    //形成输入参数记录
    for(int i=0;i<(int)vec_cur.size();i++){
      //TODO：输出参数的顺序不确定，参数类型可以不一致，暂定必须一致
      if(vec_cur[i]->get_datatype() != vec_glo[i]->get_datatype()) return false;

      //将信息进行存储
      int type_class_both = -1;
      std::string type_name_cur;
      std::string type_name_glo;
      if(vec_cur[i]->get_datatype() == "Normal"){
        //等号左侧的输入参数没有顺序
        type_class_both = 0;
        //该normal类型输入参数对应的同构类型是否相同，在构造规则内考虑，此处不做考虑
        type_name_cur = vec_cur[i]->main_name();
        type_name_glo = vec_glo[i]->main_name();
      }
      if(vec_cur[i]->get_datatype() == "Argument"){
        type_class_both = 1;
        //类型变量没有对应的同构类型，名字可以不一致
        type_name_cur = vec_cur[i]->main_name();
        type_name_glo = vec_glo[i]->main_name();
      }
      if(vec_cur[i]->get_datatype() == "Template"){
        //TODO：等号左侧构造类型处理方式待定
      }
      else{
        //TODO：等号左侧函数类型处理方式待定
      }
    
      leafargu.decl_type_map_cur.insert(std::make_pair(type_name_cur, std::make_tuple(type_class_both,i,0)));
      leafargu.decl_type_map_glo.insert(std::make_pair(type_name_glo, std::make_tuple(type_class_both,i,0)));
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
  //因为规则顺序是确定的，所以必须一一对应上
  std::vector<DatatypeDef::Component>::iterator component_ptr = this_components.begin();
  std::vector<DatatypeDef::Component>::iterator the_component_ptr = the_components.begin();
  //本地和全局等长，一起遍历。
  for(int i=0; i<(int)this_components.size(); i++){

    //都只有一个构造子，可以视为相同
    if((*component_ptr).arguments.size()==0 && (*the_component_ptr).arguments.size()==0){
      continue;  
    }
    // cout<<"^^ compare structer ^^"<<endl;
    structer.initial();
    // 比较两个树的结构
    if(!structer.compare_structure((*component_ptr).arguments, (*the_component_ptr).arguments)){
      //两个树结构不相同时，不用再比较叶子节点
      // cout<<"^^^ structer not same ^^^"<<endl;
      return false;
    }

    // cout<<"^^ compare leafnode ^^"<<endl;
    leafargu.initial();
    //两个树结构相同时，比较叶子节点
    if(!leafargu.compare_arguments((*component_ptr).arguments, (*the_component_ptr).arguments)){
      // cout<<"## arguments not same"<<endl;
      // cout<<"^^ two argument size: "<<(*component_ptr).arguments.size()
      //     <<" "<<(*the_component_ptr).arguments.size()<<endl;
      return false;
    }
    component_ptr++;
    the_component_ptr++;
  }

  return true;
}

void
DatatypeDef::Compare::Structer::initial()
{
  subtree_cur.clear();
  subtree_glo.clear();
}
bool
DatatypeDef::Compare::Structer::compare_structure(std::vector<Ptr<Type>>& args_cur, 
                                                  std::vector<Ptr<Type>>& args_glo)
{
  //只有arguments的长度一样才用继续比
  if(args_cur.size() != args_glo.size()) return false;
  //将值复制出来再操作
  subtree_cur = clone_width_queue(args_cur);
  subtree_glo = clone_width_queue(args_glo);

  //开始广度优先遍历
  subtree_cur = get_width_traversual(subtree_cur);
  subtree_glo = get_width_traversual(subtree_glo);

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
  if(subtree_cur.size() != subtree_glo.size()) return false;
  //比较所有子树的类型和节点数量
  for(int i=0;i<(int)subtree_cur.size();i++){
    if(subtree_cur[i]->get_datatype() != subtree_glo[i]->get_datatype()){
      return false;
    }
    if(subtree_cur[i]->args_num() != subtree_glo[i]->args_num()){
      return false;
    }
  }

  //比较所有非叶子节点的名字
  for(int i=0;i<(int)subtree_cur.size();i++){
    // cout<<i<<" ";
    if(subtree_cur[i]->get_datatype() == "Normal"){
      //虽然也可以适用同构匹配，但是Normal类型属于叶子节点，暂定在叶子比较中处理
      continue;
    }
    else if(subtree_cur[i]->get_datatype() == "Argument"){
      // cout<<"Argument"<<endl;
      continue;
    }
    else if(subtree_cur[i]->get_datatype() == "Func"){
      //TODO:数据类型定义中出现函数类型，处理方式待定
      continue;
    }
    else if(subtree_cur[i]->get_datatype() == "Template"){
      // cout<<"Template"<<endl;
      //右侧构造，但是左侧不是构造，出错
      if(decl_type_cur->get_datatype() != "Template"){
        width_res = false;
        return false;
      }
      
      // cout<<"one type: "<<decl_type_one->main_name()<<" "<<subtree_one[i]->main_name()<<endl;
      // cout<<"two type: "<<decl_type_two->main_name()<<" "<<subtree_two[i]->main_name()<<endl;
      //情况1：递归使用自己
      if((decl_type_cur->main_name()==subtree_cur[i]->main_name())
       &&(decl_type_glo->main_name()==subtree_glo[i]->main_name())){
        //两个子树都是递归调用自己,子树类型名与左侧类型名相同
        // cout<<"^ digui Template ^"<<endl;
        continue;
      }
      //情况2：非递归，使用之前定义过的，查看是否为同构的
      else{
        // cout<<"^ isomorphism Template ^"<<endl;
        //TODO 非递归调用的情况复杂，目前只做简单处理
        if(theAllDatatypeNameMap[subtree_cur[i]->main_name()] 
        != theAllDatatypeNameMap[subtree_glo[i]->main_name()]){
          //不同名也不同构
          width_res = false;
          return false;
        }
      }
    }
    //四种类型之外的情况
    else{
      cout<<" other datatype:"<<subtree_cur[i]->get_datatype()<<endl;
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
  types_cur.clear();
  types_glo.clear();
}
bool 
DatatypeDef::Compare::Leafargu::compare_arguments(std::vector<Ptr<Type>> &args_cur, std::vector<Ptr<Type>> &args_glo)
{
  //数组args是第一层的叶子节点，采用深度优先遍历将树展开成vector
  types_cur = get_depth_vector(args_cur);
  types_glo = get_depth_vector(args_glo);
  // cout<<"^^^ get depth vector finished! ^^^"<<endl;

  //需要比较生成数组的长度，长度一致才继续比
  if(types_cur.size() != types_glo.size()) return false;
  for(int i=0;i<(int)types_cur.size();i++){
    //类别不同，直接退出
    if(types_cur[i]->get_datatype() != types_glo[i]->get_datatype()) return false;
    //类别相同，继续比较
    if(types_cur[i]->get_datatype() == "Normal"){
      if(!compare_argu_normal(types_cur[i],types_glo[i])) return false;
    }
    if(types_cur[i]->get_datatype() == "Argument"){
      if(!compare_argu_argument(types_cur[i], types_glo[i])) return false;
    }
  }
  return get_depth_res_both(types_cur, types_glo);
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
bool
DatatypeDef::Compare::Leafargu::compare_argu_normal_decl_normal(std::string &name_cur, std::string &name_glo)
{
  // 构造规则内的normal参数只有两种可能:1、递归调用自己
  if((name_cur==decl_type_cur->main_name())&&(name_glo==decl_type_glo->main_name())){
    //两个都是递归调用定义类型名
    if(name_cur == name_glo) return false;
    else return true;
  }
  else if((name_cur!=decl_type_cur->main_name())&&(name_glo!=decl_type_glo->main_name())){
    //两个都不是递归调用等号左侧的定义类型名，则查看是否是第二种情形
  }
  else{
    //一个递归调用，一个不是递归调用，也可能正确
  }

  // 构造规则内的normal参数只有两种可能:2、使用之前定义完成的normal类型
  std::string iso_name_cur = theAllDatatypeNameMap.find(name_cur)->second;
  std::string iso_name_glo = theAllDatatypeNameMap.find(name_glo)->second;
  //两个构造规则内参数都使用之前定义的normal，则比较同构名
  if(iso_name_cur != iso_name_glo) return false;
  else return true;
}
bool
DatatypeDef::Compare::Leafargu::compare_argu_normal_decl_template(std::string &name_cur, std::string &name_glo)
{
  // 构造规则中的normal类别参数有两种情形：1、出现在等号左侧的参数列表中
  auto itr_cur = decl_type_map_cur.find(name_cur);
  auto itr_glo = decl_type_map_glo.find(name_glo);
  //都没在参数列表中出现，则继续查看是否使用定义完成的数据类型名
  if((itr_cur==decl_type_map_cur.end())&&(itr_glo==decl_type_map_glo.end())){

  }
  //都在参数列表出现，normal类型不存在顺序问题，只考虑是否对应
  else if((itr_cur!=decl_type_map_cur.end())&&(itr_glo!=decl_type_map_glo.end())){

  }
  //一个出现，一个没出现
  else{
    //TODO：构造规则中的normal类别参数在左侧参数列表中不影响
    return false;
  }

  // 构造规则中的normal类别参数有两种情形：2、不在参数列表中，使用已经定义的数据类型
  std::string iso_name_cur = theAllDatatypeNameMap.find(name_cur)->second;
  std::string iso_name_glo = theAllDatatypeNameMap.find(name_glo)->second;
  //两个构造规则内参数都使用之前定义的normal，则比较同构名
  if(iso_name_cur != iso_name_glo) return false;
  else return true;
}
bool 
DatatypeDef::Compare::Leafargu::compare_argu_normal(Ptr<Type> current, Ptr<Type> global)
{
  std::string name_cur = current->main_name();
  std::string name_glo = global->main_name();

  //如果等号左侧是normal类型
  if(decl_type_cur->get_datatype()=="Normal"){
    return compare_argu_normal_decl_normal(name_cur, name_glo);
  }
  //如果等号左侧是template类型
  else if(decl_type_cur->get_datatype()=="template"){
    return compare_argu_normal_decl_template(name_cur, name_glo);
  }
  //一般不会出现第三种类别
  else{
    return false;
  }
}
bool 
DatatypeDef::Compare::Leafargu::compare_argu_argument(Ptr<Type> cur, Ptr<Type> glo)
{
  auto itr_cur = decl_type_map_cur.find(cur->main_name());
  auto itr_glo = decl_type_map_glo.find(glo->main_name());
  //只要有一个类型变量没在等号左侧出现，直接退出
  if(itr_cur==decl_type_map_cur.end() || itr_glo==decl_type_map_glo.end()) return false;
  
  //首先查看类型变量是否已经在右侧出现过，当前的def
  if(std::get<2>(itr_cur->second)==0){
    //如果没有在前面的比较中出现，则利用顺序标识给出它的出现顺序
    var_type_order_cur += 1;
    auto& tuple_cur = itr_cur->second;
    tuple_cur = std::make_tuple(std::get<0>(tuple_cur),std::get<1>(tuple_cur),var_type_order_cur);
    decl_type_map_cur[cur->main_name()] = tuple_cur;
  }
  else{
    //如果已经在前面的比较中出现，则不做处理，直接使用出现顺序
  }

  //首先查看类型变量是否已经在右侧出现过，全局的def
  if(std::get<2>(itr_glo->second)==0){
    //如果没有在前面的比较中出现，则利用顺序标识给出它的出现顺序
    var_type_order_glo += 1;
    auto& tuple_glo = itr_glo->second;
    tuple_glo = std::make_tuple(std::get<0>(tuple_glo),std::get<1>(tuple_glo),var_type_order_cur);
    decl_type_map_glo[glo->main_name()] = tuple_glo;
  }
  else{
    //如果已经在前面的比较中出现，则不做处理，直接使用出现顺序
  }
  
  // cout<<"argument order: "<<std::get<2>(itr_one->second)<<" "<<std::get<2>(itr_two->second)<<endl;
  //比较两个类型变量在右侧的出现顺序，不一致就退出
  if(std::get<2>(itr_cur->second) != std::get<2>(itr_glo->second)){
    return false;
  }
  return true;
}
bool 
DatatypeDef::Compare::Leafargu::get_depth_res_both(std::vector<Ptr<Type>> &cur,std::vector<Ptr<Type>> &glo)
{

  return true;
}
void
DatatypeDef::Compare::store_cons_to_map(std::map<std::string,std::string>& cons_map,
                                        std::vector<DatatypeDef::Component>& combination,
                                        std::vector<DatatypeDef::Component>& the_base)
{
  std::vector<DatatypeDef::Component>::iterator cur_ptr = combination.begin();
  std::vector<DatatypeDef::Component>::iterator glo_ptr = the_base.begin();
  for(int i=0;i<(int)the_base.size();i++){
    cons_map[cur_ptr->constructor] = glo_ptr->constructor;
    cur_ptr++;
    glo_ptr++;
  }
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
  std::vector<Ptr<Type>>::iterator type_ptr = type->types.begin();

  for(;type_ptr!=type->types.end();++type_ptr){
    //首先在原数据的所有数据类型中查看
    if(theAllDatatypeNameMap[type_ptr->get()->main_name()] != type_ptr->get()->main_name()){
      int index = distance(type->types.begin(), type_ptr);
      //将同构类型的所有构造子map都存储进临时map中
      auto& one_map = theIsoDatatypeConsMap[type_ptr->get()->main_name()] ;
      for(auto one_cons : one_map){
        the_all_iso_cons_map[one_cons.first] = one_cons.second;
      }
      //直接对函数内同构类型的名字做替换
      type_ptr->get()->replace_name(theAllDatatypeNameMap[type_ptr->get()->main_name()]);
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




int
FunctionDef::is_rescusive()
{
  std::vector<Equation>::iterator ptr_equa;
  //用来区别非递归和递归
  int all_equa_call_num = 0;
  //用来区别单次递归和多次递归
  bool more_than_once_call = false;

  for (ptr_equa = equations.begin(); ptr_equa != equations.end(); ++ptr_equa)
  {
    int one_equa_call_num = ptr_equa->expr->trav_judge_recursive(this->name);
    if(one_equa_call_num > 1){
      more_than_once_call = true;
    }
    all_equa_call_num += one_equa_call_num;
  }

  cout << "function: " << this->def_name()<<" ";

  if(all_equa_call_num == 0){
    //所有equation中没有调用
    this->func_recursive_type = 0;
    //非递归类型的函数不用记忆加速
    this->memoize = false;
    cout << "not recursive " << all_equa_call_num << endl;
    return 0;
  }
  else if(more_than_once_call == false)
  {
    //有调用但任一equation调用不超过一次
    this->func_recursive_type = 1;
    //单次递归类型的函数使用值复用，不用记忆加速
    this->memoize = false;
    cout << "once recursive " << all_equa_call_num << endl;
    return 1;
  }
  else{
    //存在任一equation递归调用超过一次
    this->func_recursive_type = 2;
    //多次递归类型的函数使用记忆加速
    // this->memoize = true;
    cout<<"more recursive "<< all_equa_call_num<<endl;
    return 2;
  }
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
    cons_call = 1;
  }

  std::vector<Ptr<Expr>>::iterator ptr_args;
  int all_args_call_num = 0;
  for (ptr_args = args.begin(); ptr_args != args.end(); ++ptr_args)
  {
    all_args_call_num += ptr_args->get()->trav_judge_recursive(func_name);
  }

  return cons_call+all_args_call_num;
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


}