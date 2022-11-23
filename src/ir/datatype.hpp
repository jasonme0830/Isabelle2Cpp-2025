#pragma once

#include "ast.hpp"

#include <string>
#include <vector>

namespace hol2cpp {
class Datatype
{
public:
  void is_predef(bool value);
  bool is_predef() const;

  void is_recuisive(bool value);
  bool is_recuisive() const;

  bool is_normal_type() const;

  const std::string& self() const;
  const std::string& name() const;

  void set_self(std::string self);
  void set_name(std::string name);

  const std::vector<std::string>& template_args() const;

  std::size_t find_argument_type(const std::string& name);
  std::string add_argument_type(std::string name);

  std::size_t pos_of_cons(const std::string& cons);
  void add_constructor(const std::string& cons);
  const std::vector<std::string>& constructors() const;

  void entry_component();
  void add_field_type(const std::string& type);
  const std::vector<std::vector<std::string>>& components() const;

  std::vector<std::vector<Ptr<Type>>>& abstracts();

private:
  bool is_predef_;
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
