#include "optimizer.hpp"
#include "../ir/funcentity.hpp"

using namespace std;

namespace hol2cpp {
Optimizer theOptimizer;

const OptimizerOption &Optimizer::option() const {
    return option_;
}

void Optimizer::enable_reduce_cond() {
    option_.reduce_cond = true;
}
} // namespace hol2cpp
