#include <iostream>
#include "code.hpp"
#include "parser.hpp"

using namespace std;
using namespace hol2cpp;

int main(int argc, char* argv[])
{
    Parser parser;
    auto decls = parser.pas_func_decls(R"src(
fun add :: "nat \<Rightarrow> nat \<Rightarrow> nat" where
  "add 0 n = n" |
  "add (Suc m) n = Suc (add m n)"

fun app :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "app Nil ys = ys" |
  "app (Cons x xs) ys = Cons x (app xs ys)"

fun rev :: "'a list \<Rightarrow> 'a list" where
  "rev Nil = Nil" |
  "rev (Cons x xs) = app (rev xs) (Cons x Nil)"

fun test :: "'a list \<Rightarrow> ('a list \<Rightarrow> 'a list)" where
  "test Nil = fun"
)src");

    Code code;
    for (auto &decl : decls)
    {
        code.entry_func();
        decl->build_entity(code.current_entity());
    }
    code.generate();

    return 0;
}
