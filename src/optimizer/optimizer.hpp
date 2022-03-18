#pragma once

namespace hol2cpp {
extern class Optimizer theOptimizer;

struct OptimizerOption {
    bool enable_list_move;
};

class Optimizer {
  public:
    void enable_list_move();

    const OptimizerOption &option() const;

  private:
    OptimizerOption option_;
};
} // namespace hol2cpp
