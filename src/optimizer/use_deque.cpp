#include "../codegen/codegen.hpp"
#include "optimizer.hpp"

using namespace std;

namespace hol2cpp {
void
Optimizer::enable_use_deque()
{
  option_.use_deque = true;

  theTemplateTypeMapping["list"] = "std::deque";
  theHeaderMapping["list"] = "deque";
}
} // namespace hol2cpp
