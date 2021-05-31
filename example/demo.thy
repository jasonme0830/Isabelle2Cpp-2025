theory demo
  imports Main
begin

datatype 'a slist = sNil | sCons 'a "'a slist"

fun app1 :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "app1 [] ys = ys" |
  "app1 (x # xs) ys = x # (app1 xs ys)"
  
fun app :: "'a slist \<Rightarrow> 'a slist \<Rightarrow> 'a slist" where
  "app sNil ys = ys" |
  "app (sCons x xs) ys = sCons x (app xs ys)"

end
