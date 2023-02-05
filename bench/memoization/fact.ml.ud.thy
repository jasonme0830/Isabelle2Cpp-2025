theory fact
imports Main
begin

fun fact :: "nat => nat" where
  "fact 0 = 0" |
  "fact (Suc 0) = 1" |
  "fact n = n * (fact (n - 1))"

fun (memoize) fact_mem :: "nat => nat" where
  "fact_mem 0 = 0" |
  "fact_mem (Suc 0) = 1" |
  "fact_mem n = n * (fact (n - 1))"

fun find_if :: "nat list => (nat => bool) => nat option" where
  "find_if [] pred = None" |
  "find_if [x] pred = If (pred x) (Some x) None" |
  "find_if (x # xs) pred = If (pred x) (Some x) (find_if xs pred)"

fun find_fact :: "nat list => nat => nat option" where
  "find_fact xs x = find_if xs (% y. ((fact y) = x))"

fun find_fact_mem :: "nat list => nat => nat option" where
  "find_fact_mem xs x = find_if xs (% y. ((fact_mem y) = x))"

end
