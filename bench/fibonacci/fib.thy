theory fib
imports Main
begin

fun fib :: "nat => nat" where
  "fib 0 = 1" |
  "fib (Suc 0) = 1" |
  "fib n = (fib (n - 1)) + (fib (n - 2))"

fun (memoize) fib_mem :: "nat => nat" where
  "fib_mem 0 = 1" |
  "fib_mem (Suc 0) = 1" |
  "fib_mem n = (fib_mem (n - 1)) + (fib_mem (n - 2))"

end
