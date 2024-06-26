#include "isomorphism.hpp"

using namespace std;

namespace hol2cpp{

Isomorphism::Isomorphism(Theory& thy)
:thy(thy)
{
  if(!theConfig.close_isomor())
  {
    // 第一次遍历，比较是否有同构的数据类型
    find_isomorphism_datatype();
    // 第二次遍历，处理函数中的同构数据类型
    replace_iso_type_in_func();

    // this->print_theory();
  }


  // 第三次遍历，分析函数的递归类型，方便后续优化
  analyse_func_recu_class();
}

Isomorphism::~Isomorphism() = default;


void
Isomorphism::print_theory()
{
  for (auto& def : thy.definitions) {
    if (typeid(*def) == typeid(FunctionDef)) {
      auto& def_trans = reinterpret_cast<FunctionDef&>(*def);
      cout << def_trans.name ;
      cout<<"::";
      def_trans.type->print_type();
      cout<<endl;
      for (auto& equation : def_trans.equations) {
        auto& pattern = equation.pattern;
        auto& expr = equation.expr;
        pattern->print_expr();
        cout<<" == ";
        expr->print_expr();
        cout << endl;
      }
    }
  }
}






}