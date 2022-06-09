theory test
  imports Main
begin

fun test :: "'a list \<Rightarrow> nat" where
  "test Nil = 0" |
  "test (Cons x xs) = length (If ((length xs) = 0) Nil xs) + 1"

end
