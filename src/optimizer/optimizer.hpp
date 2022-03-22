#pragma once

namespace hol2cpp {
extern class Optimizer theOptimizer;

struct OptimizerOption {
    bool move_list;
    bool reduce_cond;
};

class Optimizer {
  public:
    void enable_move_list();
    void enable_reduce_cond();

    const OptimizerOption &option() const;

  private:
    OptimizerOption option_;
};
} // namespace hol2cpp
