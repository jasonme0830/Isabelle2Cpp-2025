#pragma once

#include <vector>
#include <iostream>
#include "funcentity.hpp"

namespace hol2cpp {
class Code {
  public:
    Code() : indent_(0), entry_func_(-1) {}

    void entry_func() {
      entry_func_ = func_entities_.size();
      func_entities_.emplace_back();
    }

    FuncEntity &current_entity() {
      return func_entities_[entry_func_];
    }

    void generate(std::ostream &out = std::cout);

  private:
    void generate_single(std::ostream &out, FuncEntity &entity);
    void generate_normal(std::ostream &out, FuncEntity &entity);
    void generate_template(std::ostream &out, FuncEntity &entity);

    int indent_;
    std::size_t entry_func_;
    std::vector<FuncEntity> func_entities_;
};
}
