#pragma once

#include "../utility/format.hpp"

#include <map>
#include <string>
#include <vector>
#include <optional>
#include <functional>

namespace hol2cpp {
class Code;

struct TypeInfo {
    static std::function<std::string(const TypeInfo &)> as_arg;

  public:
    std::string name;
    std::vector<TypeInfo> arguments;

    TypeInfo();
    explicit TypeInfo(std::string name);
    explicit TypeInfo(std::string name, TypeInfo argument);

    TypeInfo replace_with(std::string name) const;

    std::string to_str() const;
    std::string to_str_as_arg() const;

    bool empty() const;
    bool is_function() const;
    const TypeInfo &result_typeinfo() const;
    std::size_t args_size() const;

    const TypeInfo &operator[](int i) const;
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

    // return the binded code
    Code &code();

    // add indent by the defined indent size
    FuncEntity &add_indent();
    // sub indent by the defined indent size
    FuncEntity &sub_indent();

    // set or get the name of this function
    std::string &name();

    // get the name of this function
    const std::string &name() const;

    // add the determined type, argument type or result type
    void add_typeinfo(TypeInfo);
    void adjust_args(std::size_t n); // for uncurry

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

    void decl_variable(const std::string &var, const std::string &expr);
    std::string get_variable(const std::string &var);
    TypeInfo get_var_typeinfo(const std::string &var);

    /**
     * generate a temporary variable name
     *  format as temp0, temp1, ..., tempN
    */
    std::string gen_temp();

    /**
     * entry a new equation
    */
    void entry_equation();
    void close_equation();
    void close_sub_equation(size_t n);

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

    void add_delay_statement(const std::string &decl);
    template<typename ...Args>
    void add_delay_statement(const FormatString &fs, Args &&...args) {
        add_delay_statement(fs.format(std::forward<Args>(args)...));
    }
    void close_pattern();

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

    std::size_t condition_count() const;
    void app_last_stmt(const std::string &app_stmt);
    const std::vector<std::vector<std::string>> &statements() const;

    const std::vector<std::string> &delay_declarations() const;

    void is_last_equation(bool is_last);
    bool is_last_equation() const;

  private:
    Code &code_;
    std::size_t indent_;

    std::string name_;
    std::vector<std::string> raw_equations_;

    // the first type is the result type
    std::vector<TypeInfo> typeinfos_;
    std::map<std::string, std::size_t> template_mapping_;
    std::vector<std::string> template_args_;
    std::map<std::string, std::string> var_mapping_;
    std::map<std::string, std::size_t> unused_var_count_;

    std::size_t temp_count_;
    std::size_t condition_count_;
    std::vector<std::vector<std::string>> statements_;

    std::size_t decl_base_;
    std::vector<std::string> delay_statements_;

    bool is_last_equation_; // for reduce-condition

    std::map<std::string, TypeInfo> var_typeinfos_;
};
} // namespace hol2cpp
