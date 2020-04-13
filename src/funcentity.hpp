#pragma once

#include <map>
#include <string>
#include <vector>

namespace hol2cpp
{
class FuncEntity
{
  public:
    FuncEntity() = default;

    std::string
    &name() { return name_; }

    const std::string
    &name() const { return name_; }

    void add_type(std::string type)
    {
        types_.push_back(type);
    }

    std::string add_argument_type(std::string name)
    {
        if (!template_mapping_.count(name))
        {
            template_mapping_[name] = template_args_.size();
            template_args_.push_back("T" + std::to_string(template_args_.size()));
        }
        return template_args_[template_mapping_[name]];
    }

    const std::vector<std::string>
    &types() const { return types_; }

    const std::vector<std::string>
    &template_args() const { return template_args_; }

    std::string gen_temp()
    {
        return "temp" + std::to_string(temp_count_++);
    }

    void entry_euation()
    {
        temp_count_ = 0;
        patterns_.emplace_back();
        exprs_.emplace_back();
    }

    void add_pattern(std::string pattern)
    {
        patterns_.back().push_back(pattern);
    }

    void add_expr(std::string expr)
    {
        exprs_.back().push_back(expr);
    }

    const std::vector<std::vector<std::string>>
    &patterns() const { return patterns_; }

    const std::vector<std::vector<std::string>>
    &exprs() const { return exprs_; }

  private:
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
