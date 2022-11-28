theory memoize
  imports Main
begin

datatype snat = sZero | sSucc snat

fun (memoize) fib :: "nat => nat" where
  "fib 0 = 1" |
  "fib (Suc 0) = 1" |
  "fib n = (fib (n - 1)) + (fib (n - 2))"

fun (memoize) fact :: "nat => nat" where
  "fact 0 = 1" |
  "fact n = n * (fact (n - 1))"

fun add :: "snat => snat => snat" where
  "add sZero n = n" |
  "add (sSucc m) n = sSucc (add m n)"

fun (memoize) sfib :: "snat => snat" where
  "sfib sZero = sSucc sZero" |
  "sfib (sSucc sZero) = sSucc sZero" |
  "sfib (sSucc (sSucc m)) = add (sfib (sSucc m)) (sfib m)"

fun natofsnat :: "snat => nat" where
  "natofsnat sZero = 0" |
  "natofsnat (sSucc n) = (natofsnat n) + 1"

fun snatofnat :: "nat => snat" where
  "snatofnat 0 = sZero" |
  "snatofnat (Suc n) = sSucc (snatofnat n)"

end
