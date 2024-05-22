#pragma once

#include "../codegen/codegen.hpp"
#include "../utility/config.hpp"

namespace hol2cpp{

struct Isomorphism final
{
public:
  Isomorphism(Theory &thy);
  ~Isomorphism();

  void print_theory();

  void find_isomorphism_datatype();
  void replace_iso_type_in_func();
  
  void analyse_func_recu_class();

private:
  Theory &thy;

};
}