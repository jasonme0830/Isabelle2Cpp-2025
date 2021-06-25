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

end
