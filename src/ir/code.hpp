#pragma once

#include "datatype.hpp"
#include "funcentity.hpp"

#include <fstream>
#include <map>
#include <set>
#include <vector>

namespace hol2cpp {
extern std::size_t theIndentSize;

/**
 * Code contains all function entities and datatypes
 *  provides methods to generate C++ code
 */
class Code
{
public:
  Code();

  Datatype& entry_datatype(const std::string&);
  Datatype* find_datatype(const std::string&);

  void bind_cons(const std::string&, Datatype&);
  Datatype* find_datatype_by_cons(const std::string&);

  FuncEntity& entry_func_entity(const std::string&);
  FuncEntity* find_func_entity(const std::string&);

  void add_short_def(const std::string&, Ptr<ShortDef> short_def);
  const ShortDef* get_short_def(const std::string&) const;

  void add_header(const std::string& header);

  void pop_datatype();
  void pop_function();

  const std::set<std::string>& headers() const;
  std::vector<std::reference_wrapper<const Datatype>> datatypes() const;
  std::vector<std::reference_wrapper<const FuncEntity>> func_entities() const;

private:
  std::vector<std::string> names_of_datatypes_;
  std::map<std::string, Datatype> datatypes_;
  std::map<std::string, Datatype&> cons_types_;

  std::vector<std::string> names_of_func_entities_;
  std::map<std::string, FuncEntity> func_entities_;
  std::set<std::string> headers_;

  std::map<std::string, Ptr<ShortDef>> short_defs_;
};
} // namespace hol2cpp
