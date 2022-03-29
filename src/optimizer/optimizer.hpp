#pragma once

namespace hol2cpp {
extern class Optimizer theOptimizer;

// the option is also used for some experimental options currently
struct OptimizerOption {
    // optimization options
    bool move_list; // move list as much as possible
    bool reduce_cond; // removes the conditions for the last pattern for total functions (not use option `nonexhaustive`)
    bool use_deque; // use std::deque instead of std::list as the target type for list

    // experimental options that do not improve performance
    bool use_class; // generates classes instead of structs for datatypes
    bool uncurry; // returns lambda-expression if the arguments are less than parameters of the callee
};

class Optimizer {
  public:
    const OptimizerOption &option() const;

    void enable_move_list();
    void enable_reduce_cond();
    void enable_use_deque();

    void enable_use_class();
    void enable_uncurry();

  private:
    OptimizerOption option_;
};
} // namespace hol2cpp
