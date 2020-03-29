#pragma once

#include <map>
#include <vector>
#include <string>

namespace hol2cpp {
class FuncEntity {
  public:
    FuncEntity() = default;

    const std::string &name() const {
      return name_;
    }

    void set_name(std::string name) {
      name_ = name;
    }

    void entry_type() {
      entry_type_ = types_.size();
      types_.emplace_back();
      anchor_ = 0;
    }

    void add_normal_type(std::string name) {
      auto &type = types_[entry_type_];
      type.insert(anchor_, name);
    }

    void add_argument_type(std::string name) {
      if (!template_mapping_.count(name)) {
        template_mapping_[name] = template_args_.size();
        template_args_.push_back("T" + std::to_string(template_args_.size()));
      }
      const auto &template_name = template_args_[template_mapping_[name]];
      auto &type = types_[entry_type_];
      type.insert(anchor_, template_name);
    }

    void add_template_type(std::string name) {
      auto &arg_type = types_[entry_type_];
      arg_type.insert(anchor_, name + "<>");
      anchor_ += name.size() + 1;
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

    std::size_t entry_type_;
    // the first type is the result type
    std::vector<std::string> types_;
    std::size_t anchor_;
    std::map<std::string, std::size_t> template_mapping_;
    std::vector<std::string> template_args_;

    std::size_t entry_equation_;
    bool is_pattern_;
    std::vector<std::string> patterns_;
    std::vector<std::string> exprs_;
};
}
