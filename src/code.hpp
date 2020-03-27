#pragma once

#include <iostream>

namespace hol2cpp {
class Code {
  public:
    Code() : entry_func_(-1), indent_(0) {}

    void entry_func() {
      ++entry_func_;
    }

    void print(std::ostream &out = std::cout);

  private:
    int entry_func_;
    int indent_;
};
}
