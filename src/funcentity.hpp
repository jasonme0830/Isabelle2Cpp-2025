#pragma once

#include <map>
#include <vector>
#include <string>

namespace hol2cpp {
class FuncEntity {
  public:
    FuncEntity() = default;

    std::string &name() {
      return name_;
    }

    const std::string &name() const {
      return name_;
    }

    void add_type(std::string type) {
      types_.push_back(type);
    }

    std::string add_argument_type(std::string name) {
      if (!template_mapping_.count(name)) {
        template_mapping_[name] = template_args_.size();
        template_args_.push_back("T" + std::to_string(template_args_.size()));
      }
      return template_args_[template_mapping_[name]];
    }

    const std::vector<std::string> &
    types() const {
      return types_;
    }

    const std::vector<std::string> &
    template_args() const {
      return template_args_;
    }

    void entry_equation() {
      entry_equation_ = patterns_.size();
      is_pattern_ = true;
      patterns_.emplace_back();
      exprs_.emplace_back();
    }

    bool is_pattern() const {
      return is_pattern_;
    }

    void entry_expr() {
      is_pattern_ = false;
    }

  private:
    std::string name_;

    // the first type is the result type
    std::vector<std::string> types_;
    std::map<std::string, std::size_t> template_mapping_;
    std::vector<std::string> template_args_;

    std::size_t entry_equation_;
    bool is_pattern_;
    std::vector<std::string> patterns_;
    std::vector<std::string> exprs_;
};
}
