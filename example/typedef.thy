theory datatypes
imports Main
begin


datatype snat = sZero | sSucc snat
datatype 'a alsit = aNil | aCons 'a "'a alist"
datatype 'a slist = sNil | sCons 'a "'a slist"
datatype 'a atree = aLeaf | 
                    aNode1 "'a atree" 'a "'a atree"

fun add :: "snat \<Rightarrow> snat \<Rightarrow> snat" where
  "add sZero n = n" |
  "add (sSucc m) n = sSucc (add m n)"

fun app :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "app Nil ys = ys" |
  "app (Cons x xs) ys = Cons x (app xs ys)"

fun rev :: "'a list \<Rightarrow> 'a list" where
  "rev Nil = Nil" |
  "rev (Cons x xs) = app (rev xs) (Cons x Nil)"

fun snatofnat :: "nat \<Rightarrow> snat" where
  "snatofnat 0 = sZero" |
  "snatofnat (Suc n) = sSucc (snatofnat n)"

fun testset :: "'a list \<Rightarrow> 'a set" where
  "testset Nil = {}" |
  "testset (x # xs) = {x} \<inter> testset(xs)" |
  "testset (x # xs) = {x} \<union> testset(xs)"

fun fib :: "nat \<Rightarrow> nat" where
  "fib 0 = 1" |
  "fib (Suc 0) = 1" |
  "fib n = (fib (n - 1)) + (fib (n - 2))"
  
end