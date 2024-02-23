#pragma once

#include "../codegen/codegen.hpp"

namespace hol2cpp{

struct Isomorphism final
{
public:
  Isomorphism(Theory &thy);
  ~Isomorphism();

  void find_isomorphism_datatype();
  void replace_iso_type_in_func();
  void find_rescusive_func();


private:
  Theory &thy;

};
}