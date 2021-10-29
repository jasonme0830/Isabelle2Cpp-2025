theory only_funs
  imports Main
begin

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
  "rev2 (x # xs) = (rev2 xs) @ (x # Nil)"

fun rev3 :: "'a list \<Rightarrow> 'a list" where
  "rev3 [] = []" |
  "rev3 (x # xs) = (rev3 xs) @ [x]"

fun testoption :: "'a option \<Rightarrow> ('a list) option" where
  "testoption None = Some Nil" |
  "testoption (Some x) = Some [x]"

fun testset :: "'a list \<Rightarrow> 'a set" where
  "testset Nil = {}" |
  "testset (x # xs) = {x} \<inter> testset(xs)" |
  "testset (x # xs) = {x} \<union> testset(xs)"

fun testifelse :: "'a list \<Rightarrow> 'a \<Rightarrow> 'a list" where
  "testifelse [] n = []" |
  "testifelse (x # xs) n = (if (x < n) then [x] else []) @ (testifelse xs n)"

fun pair :: "'a \<Rightarrow> 'b \<Rightarrow> ('a * 'b)" where
  "pair f s = (f, s)"

fun fst :: "('a * 'b) \<Rightarrow> 'a" where
  "fst (first, second) = first"

fun snd :: "('a * 'b) \<Rightarrow> 'b" where
  "snd (first, second) = second"

fun evn :: "nat \<Rightarrow> bool" where
  "evn 0 = True" |
  "evn (Suc 0) = False" |
  "evn (Suc (Suc n)) = evn n"

fun fib :: "nat \<Rightarrow> nat" where
  "fib 0 = 1" |
  "fib (Suc 0) = 1" |
  "fib n = (fib (n - 1)) + (fib (n - 2))"

fun altrue :: "nat \<Rightarrow> bool" where
  "altrue _ = True"

end
