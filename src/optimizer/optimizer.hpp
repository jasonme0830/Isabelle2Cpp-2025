#pragma once

namespace hol2cpp {
extern class Optimizer theOptimizer;

struct OptimizerOption {
    bool move_list;
};

class Optimizer {
  public:
    void enable_move_list();

    const OptimizerOption &option() const;

  private:
    OptimizerOption option_;
};
} // namespace hol2cpp
