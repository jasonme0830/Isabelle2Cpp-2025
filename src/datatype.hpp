#pragma once

#include "ast.hpp"

#include <vector>
#include <string>

namespace hol2cpp {
class DataType {
  public:
    bool &is_recuisive();
    std::string &self();

    bool is_normal_type();

    std::string &name();

    std::vector<std::string> &template_args();

    std::size_t find_argument_type(const std::string &name);
    std::string add_argument_type(std::string name);

    std::size_t pos_of_cons(const std::string &cons);
    void add_constructor(const std::string &cons);
    const std::vector<std::string> &constructors() const;

    void entry_component();
    void add_field_type(const std::string &type);
    const std::vector<std::vector<std::string>> &components() const;

    std::vector<std::vector<Ptr<Type>>> &abstracts();

  private:
    bool is_recuisive_;
    std::string self_;

    std::string name_;

    std::vector<std::string> types_;
    std::map<std::string, std::size_t> template_mapping_;
    std::vector<std::string> template_args_;

    std::map<std::string, std::size_t> constructor_mapping_;
    std::vector<std::string> constructors_;
    std::vector<std::vector<std::string>> components_;

    std::vector<std::vector<Ptr<Type>>> abstracts_;
};
} // namespace hol2cpp
