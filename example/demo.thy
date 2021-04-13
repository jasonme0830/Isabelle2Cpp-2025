fun app :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "app [] ys = ys" |
  "app (x # xs) ys = x # (app xs ys)"

datatype 'a slist = sNil | sCons 'a "'a slist";

fun app2 :: "'a slist \<Rightarrow> 'a slist \<Rightarrow> 'a slist" where
  "app2 sNil ys = ys" |
  "app2 (sCons x xs) ys = sCons x (app xs ys)"
