#include "datatype.hpp"

using namespace std;

namespace hol2cpp {

Datatype::Datatype(Code& code)
:code_(code)
{

}
Code&
Datatype::code()
{
  return code_;
}
void
Datatype::is_predef(bool value)
{
  is_predef_ = value;
}

bool
Datatype::is_predef() const
{
  return is_predef_;
}

void
Datatype::is_recuisive(bool value)
{
  is_recuisive_ = value;
}

bool
Datatype::is_recuisive() const
{
  return is_recuisive_;
}

bool
Datatype::is_normal_type() const
{
  return template_args_.empty();
}

const string&
Datatype::self() const
{
  return self_;
}

const string&
Datatype::name() const
{
  return name_;
}

void
Datatype::set_self(string self)
{
  self_ = move(self);
}

void
Datatype::set_name(string name)
{
  name_ = move(name);
}

const vector<string>&
Datatype::template_args() const
{
  return template_args_;
}

size_t
Datatype::find_argument_type(const string& name)
{
  return template_mapping_.at(name);
}

string
Datatype::add_argument_type(string name)
{
  if (!template_mapping_.count(name)) {
    template_mapping_[name] = template_args_.size();
    template_args_.push_back("T" + to_string(template_args_.size() + 1));
  }
  return template_args_[template_mapping_[name]];
}

size_t
Datatype::pos_of_cons(const string& cons)
{
  return constructor_mapping_[cons];
}

void
Datatype::add_constructor(const string& cons)
{
  constructor_mapping_[cons] = constructors_.size();
  constructors_.push_back(cons);
}

const vector<string>&
Datatype::constructors() const
{
  return constructors_;
}

void
Datatype::entry_component()
{
  components_.emplace_back();
}

void
Datatype::add_field_type(const string& type)
{
  components_.back().push_back(type);
}

const vector<vector<string>>&
Datatype::components() const
{
  return components_;
}

vector<vector<Ptr<Type>>>&
Datatype::abstracts()
{
  return abstracts_;
}
} // namespace hol2cpp
