fun add :: "nat \<Rightarrow> nat \<Rightarrow> nat" where
  "add 0 n = n" |
  "add (Suc m) n = Suc (add m n)"

fun app :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "app Nil ys = ys" |
  "app (Cons x xs) ys = Cons x (app xs ys)"

fun rev :: "'a list \<Rightarrow> 'a list" where
  "rev Nil = Nil" |
  "rev (Cons x xs) = app (rev xs) (Cons x Nil)"

fun rev2 :: "'a list \<Rightarrow> 'a list" where
  "rev2 Nil = Nil" |
  "rev2 (x # xs) = (rev xs) @ (x # Nil)"

fun rev3 :: "'a list \<Rightarrow> 'a list" where
  "rev3 [] = []" |
  "rev3 (x # xs) = (rev xs) @ [x]"

fun testoption :: "'a option \<Rightarrow> ('a list) option" where
  "testoption None = Some Nil" |
  "testoption (Some x) = Some [x]"

fun testset :: "'a list \<Rightarrow> 'a set" where
  "testset Nil = {}" |
  "testset (x # xs) = {x} \<inter> testset(xs)" |
  "testset (x # xs) = {x} \<union> testset(xs)"
