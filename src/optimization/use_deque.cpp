#include "../codegen/codegen.hpp"
#include "../utility/config.hpp"

using namespace std;

namespace hol2cpp {
void
Config::use_deque(bool enable)
{
  use_deque_ = !enable;

  if (!enable) {
    return;
  }

  theTemplateTypeMapping["list"] = "std::deque";
  theHeaderMapping["list"] = "deque";
}
} // namespace hol2cpp
