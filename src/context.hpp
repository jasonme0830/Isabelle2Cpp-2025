#pragma once

#include <map>
#include <vector>
#include <string>

namespace hol2cpp {
class Context {
  public:
    Context() = default;

    void entry_result_type() {
      entry_result_type_ = true;
      anchor_ = 0;
    }

    void entry_argument_type(std::size_t index) {
      entry_result_type_ = false;
      entry_argument_type_ = index;
      anchor_ = 0;
      argument_types_.emplace_back();
    }

    void add_normal_type(std::string name) {
      if (entry_result_type_) {
        result_type_.insert(anchor_, name);
      } else {
        auto &arg_type = argument_types_[entry_argument_type_];
        arg_type.insert(anchor_, name);
      }
    }

    void add_argument_type(std::string name) {
      if (!template_mapping_.count(name)) {
        template_args_.push_back(
          template_mapping_[name] =
            "T" + std::to_string(template_args_.size()));
      }
      const auto &template_name = template_mapping_[name];
      if (entry_result_type_) {
        result_type_.insert(anchor_, template_name);
      } else {
        auto &arg_type = argument_types_[entry_argument_type_];
        arg_type.insert(anchor_, template_name);
      }
    }

    void add_template_type(std::string name) {
      if (entry_result_type_) {
        result_type_.insert(anchor_, name + "<>");
        anchor_ += name.size() + 1;
      } else {
        auto &arg_type = argument_types_[entry_argument_type_];
        arg_type.insert(anchor_, name + "<>");
        anchor_ += name.size() + 1;
      }
    }

  private:
    bool entry_result_type_;
    std::size_t entry_argument_type_;

    std::vector<std::string> argument_types_;
    std::size_t anchor_;
    std::string result_type_;

    std::map<std::string, std::string> template_mapping_;
    std::vector<std::string> template_args_;
};
}
