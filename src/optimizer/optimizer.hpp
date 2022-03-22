#pragma once

namespace hol2cpp {
extern class Optimizer theOptimizer;

// the option is also used for some experimental options currently
struct OptimizerOption {
    bool move_list;
    bool reduce_cond;
};

class Optimizer {
  public:
    const OptimizerOption &option() const;

    void enable_move_list();
    void enable_reduce_cond();

  private:
    OptimizerOption option_;
};
} // namespace hol2cpp
