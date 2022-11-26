theory memoize
  imports Main
begin

fun (memoize) fib :: "nat \<Rightarrow> nat" where
  "fib 0 = 1" |
  "fib (Suc 0) = 1" |
  "fib n = (fib (n - 1)) + (fib (n - 2))"

fun (memoize) fact :: "nat \<Rightarrow> nat" where
  "fact 0 = 1" |
  "fact n = n * (fact (n - 1))"

end
