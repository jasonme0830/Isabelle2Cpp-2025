#include <iostream>
#include "code.hpp"
#include "parser.hpp"

using namespace std;
using namespace hol2cpp;

int main(int argc, char *argv[]) {
  Parser parser;
  auto decls = parser.pas_func_decls(R"src(
fun add :: "nat \<Rightarrow> nat \<Rightarrow> nat" where
  "add Nil n = n" |
  "add (Suc m) n = Suc (Add m n)"

fun test :: "'a list \<Rightarrow> 'a list" where
  "test l = l"
)src");

  Code code;
  for (auto &decl : decls) {
    decl->codegen(code);
  }
  code.generate();

  return 0;
}
