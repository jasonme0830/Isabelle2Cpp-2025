#pragma once

#include <set>
#include <vector>
#include <iostream>
#include "funcentity.hpp"

namespace hol2cpp
{
/**
 * Code contains all function entities
 * provides methods to generate C++ code
*/
class Code
{
  public:
    /**
     * @out: generated C++ code will be outputed by out
     *  use std::cout by default
    */
    Code(std::ostream &out = std::cout);

    /**
     * entry new function entity
    */
    void
    entry_func();

    /**
     * return the current entity, always the last one, to build
    */
    FuncEntity
    &current_entity();

    /**
     * generate code and output by the out_ member
    */
    void
    generate();

    /**
     * define or get the indent size
     *  4 by default
    */
    static
    std::size_t
    &indent_size();

    /**
     * add header file
    */
    void
    add_header(const std::string &header);

  private:
    /**
     * generate code for each function declaration
    */
    void
    generate_single(FuncEntity &entity);
    /**
     * generate code for type determined function
    */
    void
    generate_normal(FuncEntity &entity);
    /**
     * generate code for template function
     * add the template declaration before function declaration
     *  such as template<typename T0...>
     * then call method generate_normal to generate the rest code
    */
    void
    generate_template(FuncEntity &entity);

    /**
     * start a new line with the indent
    */
    std::ostream
    &new_line();

    /**
     * add the indent's length by indent size
    */
    void
    add_indent();
    /**
     * sub the indent's length by indent size
    */
    void
    sub_indent();

    std::ostream &out_;
    int indent_;

    std::vector<FuncEntity> func_entities_;
    std::set<std::string> headers_;
};
}
