#pragma once

#include <vector>
#include <iostream>
#include "funcentity.hpp"

namespace hol2cpp {
class Code {
  public:
    Code(std::ostream &out = std::cout)
      : out_(out), indent_(0), entry_func_(-1) {}

    void entry_func() {
      entry_func_ = func_entities_.size();
      func_entities_.emplace_back();
    }

    FuncEntity &current_entity() {
      return func_entities_[entry_func_];
    }

    void generate();

    static std::size_t &indent_size() {
      static std::size_t value = 4;
      return value;
    }

    static std::string raw_indent() {
      return std::string(indent_size(), ' ');
    }

  private:
    void generate_single(FuncEntity &entity);
    void generate_normal(FuncEntity &entity);
    void generate_template(FuncEntity &entity);

    std::ostream &new_line() {
      out_ << std::string(indent_, ' ');
      return out_;
    }

    void add_indent() {
      indent_ += indent_size();
    }
    void sub_indent() {
      indent_ -= indent_size();
    }

    std::ostream &out_;
    int indent_;

    std::size_t entry_func_;
    std::vector<FuncEntity> func_entities_;
};
}
