theory binary_search
imports Main
begin

fun bs :: "nat \<Rightarrow> nat list \<Rightarrow> nat option" where
  "bs x [] = None" |
  "bs x [y] = If (x = y) (Some x) None" |
  "bs x ys = (let m = (length ys) div 2 in let y = ys ! m in If (y = x) (Some x) (If (y < x) (bs x (drop (m + 1) ys)) (bs x (take m ys))))"

end
