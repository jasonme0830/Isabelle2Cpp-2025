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

fun sfib :: "snat => snat" where
  "sfib sZero = sSucc sZero" |
  "sfib (sSucc sZero) = sSucc sZero" |
  "sfib (sSucc (sSucc m)) = add (sfib (sSucc m)) (sfib m)"


fun merge :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge xs [] = xs" |
  "merge [] ys = ys" |
  "merge (x # xs) (y # ys) = If (x \<le> y) (x # (merge xs (y # ys))) (y # (merge (x # xs) ys))"

fun merge_sort :: "nat list \<Rightarrow> nat list" where
  "merge_sort [] = []" |
  "merge_sort [x] = [x]" |
  "merge_sort xs = merge (merge_sort (take ((length xs) div 2) xs)) (merge_sort (drop ((length xs) div 2) xs))"
end