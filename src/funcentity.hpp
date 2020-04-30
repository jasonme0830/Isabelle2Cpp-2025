#pragma once

#include <map>
#include <string>
#include <vector>

namespace hol2cpp
{
class Code;

class FuncEntity
{
  public:
    FuncEntity(Code &code);

    Code
    &code();

    void
    add_indent();
    void
    sub_indent();

    std::string
    &name();

    const std::string
    &name()
    const;

    void
    add_type(std::string type);

    std::string
    add_argument_type(std::string name);

    const std::string
    &result_type();

    const std::vector<std::string>
    &types()
    const;

    const std::vector<std::string>
    &template_args()
    const;

    std::string
    gen_temp();

    void
    entry_euation();

    void
    add_pattern(const std::string &pattern);

    void
    add_expr(const std::string &expr);

    const std::vector<std::vector<std::string>>
    &patterns()
    const;

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
