#pragma once

#include <set>
#include <map>
#include <vector>
#include <iostream>
#include "datatype.hpp"
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

    DataType
    &entry_data_type(const std::string &);

    DataType
    *find_data_type(const std::string &);

    void
    bind_cons(const std::string &, DataType &);
    DataType
    *find_data_type_by_cons(const std::string &);

    /**
     * entry new function entity ane the return it
    */
    FuncEntity
    &entry_func_entity(const std::string &);

    FuncEntity
    *find_func_entity(const std::string &);

    /**
     * generate code and output by the out_ member
    */
    void
    generate();

    /**
     * define or get the indent size
     * 4 by default
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
    void
    gen_data_type(DataType &data_type);
    void
    gen_type_rest(DataType &data_type);
    void
    gen_normal_type_header(DataType &data_type);
    void
    gen_template_type_header(DataType &data_type);

    /**
     * generate code for each function declaration
    */
    void
    gen_single_func(FuncEntity &entity);
    /**
     * generate code for type determined function
    */
    void
    gen_normal_func(FuncEntity &entity);
    /**
     * generate code for template function
     * add the template declaration before function declaration
     *  such as template<typename T0...>
     * then call method gen_normal_func to generate the rest code
    */
    void
    gen_template_func(FuncEntity &entity);

    /**
     * start a new line with the indent
    */
    std::ostream
    &newline();

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

    std::vector<std::string> names_of_data_types_;
    std::map<std::string, DataType> data_types_;
    std::map<std::string, DataType&> cons_types_;

    std::vector<std::string> names_of_func_entities_;
    std::map<std::string, FuncEntity> func_entities_;
    std::set<std::string> headers_;
};
}
