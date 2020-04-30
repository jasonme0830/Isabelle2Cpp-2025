#pragma once

#include <set>
#include <vector>
#include <iostream>
#include "funcentity.hpp"

namespace hol2cpp
{
class Code
{
  public:
    Code(std::ostream &out = std::cout);

    void
    entry_func();

    FuncEntity
    &current_entity();

    void
    generate();

    static
    std::size_t
    &indent_size();

    void
    add_header(const std::string &header);

  private:
    void
    generate_single(FuncEntity &entity);
    void
    generate_normal(FuncEntity &entity);
    void
    generate_template(FuncEntity &entity);

    std::ostream
    &new_line();

    void
    add_indent();
    void
    sub_indent();

    std::ostream &out_;
    int indent_;

    std::size_t entry_func_;
    std::vector<FuncEntity> func_entities_;

    std::set<std::string> headers_;
};
}
