theory datatypes
imports Main
begin


datatype snat = sZero | sSucc snat
datatype 'a alsit = aNil | aCons 'a "'a alist"
datatype 'a slist = sNil | sCons 'a "'a slist"

fun app :: "'a slist \<Rightarrow> 'a slist \<Rightarrow> 'a slist" where
  "app sNil ys = ys" |
  "app (sCons x xs) ys = sCons x (app xs ys)"

fun len :: "'a slist \<Rightarrow> snat" where
  "len sNil = sZero" |
  "len (sCons x xs) = sSucc (len xs)"

fun rev :: "'a slist \<Rightarrow> 'a slist" where
  "rev sNil = sNil" |
  "rev (sCons x xs) = app (rev xs) (sCons x sNil)"



end