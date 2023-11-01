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
  // cout<<"@ one compare isomorphism"<<endl;
  //声明一个int数组用来存储对比信息
  int index = -1;
  std::vector<DatatypeDef>::iterator decl_ptr;
  for(decl_ptr=theDefinedDatatypes.begin();decl_ptr!=theDefinedDatatypes.end();++decl_ptr){
    cout<<"^ two datatype name: "<<this->def_name()<<" "<<decl_ptr->def_name()<<endl;
    //比较等号左边的类型,相同才有必要继续比
    if(decl_type->get_datatype() == decl_ptr->decl_type->get_datatype()){
      //接着比较components内元素的数量，不一致就跳出本次循环
      if(components.size() != decl_ptr->components.size()) continue;
      if(compare_components(decl_ptr->components, this->components)){
        //components也相同，则这两个datatypedef同构
        index = distance(theDefinedDatatypes.begin(), decl_ptr);
        break;
      }
    }
  }

  if(index == -1){
    //不同构，则自己与自己建立对应关系
    theIsomorphismDatatypeMap.insert(this->def_name(),this->def_name());
    return false;
  }
  else{
    //与前文定义的数据类型同构，建立对应关系
    theIsomorphismDatatypeMap.insert(this->def_name(),theDefinedDatatypes[index].def_name());
    return true;
  }
}

bool 
DatatypeDef::compare_components(std::vector<DatatypeDef::Component> &the_components,
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
  //比较所有非叶子节点的名字
  for(int i=0;i<(int)subtree_one.size();i++){
    if(subtree_one[i]->get_datatype() == "Normal") continue;
    if(subtree_one[i]->get_datatype() == "Argument") continue;
    if(subtree_one[i]->main_name() != subtree_two[i]->main_name()){
      //构造规则中使用的类型名不同，查看是否是不同名但同构的
      if(theIsomorphismDatatypeMap[subtree_one[i]->main_name()] 
      != theIsomorphismDatatypeMap[subtree_two[i]->main_name()]){
        //不同名也不同构
        width_res = false;
        return false;
      }
    }
  }
  //所有子树全部遍历完成
  width_res = true;
  return true;
}

} // namespace hol2cpp
