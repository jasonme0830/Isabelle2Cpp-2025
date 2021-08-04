theory demo
  imports Main
begin

datatype 'a slist = sNil | sCons 'a "'a slist"

fun app :: "'a slist \<Rightarrow> 'a slist \<Rightarrow> 'a slist" where
  "app sNil ys = ys" |
  "app (sCons x xs) ys = sCons x (app xs ys)"

fun rev :: "'a slist \<Rightarrow> 'a slist" where
  "rev sNil = sNil" |
  "rev (sCons x xs) = app (rev xs) (sCons x sNil)"

fun dblist :: "'a slist \<Rightarrow> 'a slist" where
  "dblist sNil = sNil" |
  "dblist (sCons x sNil) = sCons x (sCons x sNil)" |
  "dblist (sCons x xs) = app (dblist (sCons x sNil)) (dblist xs)"

fun fib :: "nat \<Rightarrow> nat" where
  "fib 0 = 1" |
  "fib (Suc 0) = 1" |
  "fib n = (fib (n - 1)) + (fib (n - 2))"

fun rev3 :: "'a list \<Rightarrow> 'a list" where
  "rev3 [] = []" |
  "rev3 (x # xs) = (rev3 xs) @ [x]"

export_code rev3 in SML module_name demo

end
