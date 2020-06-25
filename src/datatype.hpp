#pragma once

#include <vector>
#include <string>
#include "ast.hpp"

namespace hol2cpp
{
class DataType
{
  public:
    bool &is_recuisive()
    {
        return is_recuisive_;
    }
    std::string &name()
    {
        return name_;
    }

    std::vector<std::string> &template_args()
    {
        return template_args_;
    }

    std::size_t
    find_argument_type(const std::string &name)
    {
        return template_mapping_.at(name);
    }

    std::string
    add_argument_type(std::string name)
    {
        if (!template_mapping_.count(name))
        {
            template_mapping_[name] = template_args_.size();
            template_args_.push_back("T" + std::to_string(template_args_.size()));
        }
        return template_args_[template_mapping_[name]];
    }

    std::size_t
    pos_of_cons(const std::string &cons)
    {
        return constructor_mapping_[cons];
    }
    void
    add_constructor(const std::string &cons)
    {
        constructor_mapping_[cons] = constructors_.size();
        constructors_.push_back(cons);
    }
    const std::vector<std::string>
    &constructors() const
    {
        return constructors_;
    }

    void
    entry_component()
    {
        components_.emplace_back();
    }
    void
    add_field_type(const std::string &type)
    {
        components_.back().push_back(type);
    }
    const std::vector<std::vector<std::string>>
    &components() const
    {
        return components_;
    }

    std::vector<std::vector<Ptr<Type>>> &abstracts()
    {
        return abstracts_;
    }

  private:
    bool is_recuisive_;
    std::string name_;

    std::vector<std::string> types_;
    std::map<std::string, std::size_t> template_mapping_;
    std::vector<std::string> template_args_;

    std::map<std::string, std::size_t> constructor_mapping_;
    std::vector<std::string> constructors_;
    std::vector<std::vector<std::string>> components_;

    std::vector<std::vector<Ptr<Type>>> abstracts_;
};
}
