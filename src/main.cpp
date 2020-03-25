#include <iostream>
#include "parser.hpp"

using namespace std;
using namespace hol2cpp;

int main(int argc, char *argv[]) {
  Parser parser;
  auto decls = parser.pas_func_decls(R"src(
fun add :: "nat \<Rightarrow> nat \<Rightarrow> nat" where
  "add Nil n = n" |
  "add (Suc m) n = Suc (Add m n)"
)src");
  return 0;
}
