#pragma once

namespace hol2cpp {
extern class Optimizer theOptimizer;

// the option is also used for some experimental options currently
struct OptimizerOption {
    // optimization options
    bool move_list;
    bool reduce_cond;

    // experimental options
    bool use_class;
};

class Optimizer {
  public:
    const OptimizerOption &option() const;

    void enable_move_list();
    void enable_reduce_cond();

    void enable_use_class();

  private:
    OptimizerOption option_;
};
} // namespace hol2cpp
