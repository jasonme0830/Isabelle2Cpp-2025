#pragma once

#include <map>
#include <string>
#include <vector>

namespace hol2cpp
{
class Code;

/**
 * FuncEntity contains types occurs in the parsed function
 *  and generated statements
*/
class FuncEntity
{
  public:
    /**
     * @code: bind the code then could get the code
     *  and call its method add_header to add header by needed
    */
    FuncEntity(Code &code);

    /**
     * return the binded code
    */
    Code
    &code();

    /**
     * add indent by the defined indent size
    */
    void
    add_indent();
    /**
     * sub indent by the defined indent size
    */
    void
    sub_indent();

    /**
     * set or get the name of this function
    */
    std::string
    &name();

    /**
     * get the name of this function
    */
    const std::string
    &name()
    const;

    /**
     * add the determined type
     *  argument type or result type
    */
    void
    add_type(std::string type);

    /**
     * mapping the given name to the argument type in C++
     * @name: the raw name of the argument in isabelle/hol
     *  for example, a from 'a
     * return the mapped argument type in C++
    */
    std::string
    add_argument_type(std::string name);

    /**
     * return the result type
    */
    const std::string
    &result_type();

    /**
     * return the types of this function
     * note: the types contain the result type, which is the last
    */
    const std::vector<std::string>
    &types()
    const;

    /**
     * return the template argument types
     *  used for generate code for template functions
    */
    const std::vector<std::string>
    &template_args()
    const;

    /**
     * generate a temporary variable name
     *  format as temp0, temp1, ..., tempN
    */
    std::string
    gen_temp();

    /**
     * entry a new equation
    */
    void
    entry_euqation();

    /**
     * add a pattern condition
     * pattern statements will be generated before expressions
     *  to break or generate variable declarations
    */
    void
    add_pattern(const std::string &pattern);

    void
    add_pattern_cond(const std::string &cond);

    /**
     * add a statement for returning expression
    */
    void
    add_expr(const std::string &expr);

    /**
     * get generated statements for pattern
    */
    const std::vector<std::vector<std::string>>
    &patterns()
    const;

    /**
     * get generated statements for expression
    */
    const std::vector<std::vector<std::string>>
    &exprs()
    const;

  private:
    Code &code_;
    std::size_t indent_;

    std::string name_;

    // the first type is the result type
    std::vector<std::string> types_;
    std::map<std::string, std::size_t> template_mapping_;
    std::vector<std::string> template_args_;

    std::size_t temp_count_;
    std::vector<std::vector<std::string>> patterns_;
    std::vector<std::vector<std::string>> exprs_;
};
}
