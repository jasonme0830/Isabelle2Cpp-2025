#include "optimizer.hpp"
#include "../ir/funcentity.hpp"

using namespace std;

namespace hol2cpp {
Optimizer theOptimizer;

const OptimizerOption &Optimizer::option() const {
    return option_;
}
} // namespace hol2cpp
