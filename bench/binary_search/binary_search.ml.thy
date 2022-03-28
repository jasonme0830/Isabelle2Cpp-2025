theory binary_search_ml
imports Main
begin

fun bs_ml :: "nat \<Rightarrow> nat list \<Rightarrow> nat option" where
  "bs_ml x [] = None" |
  "bs_ml x [y] = If (x = y) (Some x) None" |
  "bs_ml x ys = (let m = (length ys) div 2 in let y = ys ! m in If (y = x) (Some x) (If (y < x) (bs_ml x (drop (m + 1) ys)) (bs_ml x (take m ys))))"

end
