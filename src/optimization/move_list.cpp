#include "../codegen/codegen.hpp"
#include "../ir/ast.hpp"
#include "../ir/funcentity.hpp"
#include "../utility/config.hpp"

using namespace std;

namespace hol2cpp {
void
Config::close_moveStd(bool enable)
{
  close_moveStd_ = enable;

  // if (!enable) {
  //   return;
  // }

  // auto origin_as_arg = TypeInfo::as_arg;
  // TypeInfo::as_arg = [=](const TypeInfo& type, int func_recu_class) {
  //   if (type.name == theTemplateTypeMapping["list"]) {
  //     return type.to_str(func_recu_class) + " ";
  //   } else {
  //     return origin_as_arg(type, func_recu_class);
  //   }
  // };
}


} // namespace hol2cpp
