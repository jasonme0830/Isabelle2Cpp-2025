#pragma once

namespace hol2cpp {
extern class Optimizer theOptimizer;

// the option is also used for some experimental options currently
struct OptimizerOption {
    // optimization options
    bool move_list; // move std::list as much as possible
    bool reduce_cond; // removes the conditions for the last pattern

    // experimental options
    bool use_class; // generates classes instead of structs for datatypes
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
