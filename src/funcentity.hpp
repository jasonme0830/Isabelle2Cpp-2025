#pragma once

#include "format.hpp"

#include <map>
#include <string>
#include <vector>

namespace hol2cpp {
class Code;

struct TypeInfo {
    std::string name;
    std::vector<TypeInfo> arguments;

    TypeInfo() = default;
    explicit TypeInfo(std::string name);

    std::string to_str() const;
    std::string to_str_with(const std::string &name) const;

    bool empty() const;
    bool is_function() const;
    const TypeInfo &result_typeinfo();
    std::size_t args_size() const;
};

/**
 * FuncEntity contains types occurs in the parsed function
 *  and generated statements
*/
class FuncEntity {
  public:
    /**
     * @code: bind the code then could get the code
     *  and call its method add_header to add header by needed
    */
    FuncEntity(Code &code);

    /**
     * return the binded code
    */
    Code &code();

    /**
     * add indent by the defined indent size
    */
    FuncEntity &add_indent();
    /**
     * sub indent by the defined indent size
    */
    FuncEntity &sub_indent();

    /**
     * set or get the name of this function
    */
    std::string &name();

    /**
     * get the name of this function
    */
    const std::string &name() const;

    /**
     * add the determined type
     *  argument type or result type
    */
    void add_typeinfo(TypeInfo);

    /**
     * mapping the given name to the argument type in C++
     * @name: the raw name of the argument in isabelle/hol
     *  for example, a from 'a
     * return the mapped argument type in C++
    */
    std::string add_argument_type(std::string name);

    /**
     * return the result type
    */
    const TypeInfo &result_typeinfo();

    /**
     * return the types of this function
     * note: the types contain the result type, which is the last
    */
    const std::vector<TypeInfo> &typeinfos() const;
    std::size_t args_size() const;

    /**
     * return the template argument types
     *  used for generate code for template functions
    */
    const std::vector<std::string> &template_args() const;

    std::map<std::string, std::string> &varrm_mapping();

    /**
     * generate a temporary variable name
     *  format as temp0, temp1, ..., tempN
    */
    std::string gen_temp();

    /**
     * entry a new equation
    */
    void entry_euqation();

    /**
     * add a pattern condition
     * pattern statements will be generated before expressions
     *  to break or generate variable declarations
     *
     * ';' will be added to the end automatically
    */
    void add_pattern(const std::string &pattern);
    template<typename ...Args>
    void add_pattern(const FormatString &fs, Args &&...args) {
        add_pattern(fs.format(std::forward<Args>(args)...));
    }

    void add_pattern_cond(const std::string &cond);
    template<typename ...Args>
    void add_pattern_cond(const FormatString &fs, Args &&...args) {
        add_pattern_cond(fs.format(std::forward<Args>(args)...));
    }

    /**
     * add a statement for returning expression
     *
     * ';' won't be added to the end automatically,
     *  so the expr should be with ';' if it needs
    */
    FuncEntity &add_expr(const std::string &expr);
    template<typename ...Args>
    FuncEntity &add_expr(const FormatString &fs, Args &&...args) {
        return add_expr(fs.format(std::forward<Args>(args)...));
    }

    const std::vector<std::vector<std::string>> &statements() const;

  private:
    Code &code_;
    std::size_t indent_;

    std::string name_;

    // the first type is the result type
    std::vector<TypeInfo> typeinfos_;
    std::map<std::string, std::size_t> template_mapping_;
    std::vector<std::string> template_args_;
    std::map<std::string, std::string> varrm_mapping_;

    std::size_t temp_count_;
    std::vector<std::vector<std::string>> statements_;
};
} // namespace hol2cpp
