theory merge_sort_ml_rc
imports Main
begin

fun merge_ml_rc :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge_ml_rc xs [] = xs" |
  "merge_ml_rc [] ys = ys" |
  "merge_ml_rc (x # xs) (y # ys) = If (x \<le> y) (x # (merge_ml_rc xs (y # ys))) (y # (merge_ml_rc (x # xs) ys))"

fun merge_sort_ml_rc :: "nat list \<Rightarrow> nat list" where
  "merge_sort_ml_rc [] = []" |
  "merge_sort_ml_rc [x] = [x]" |
  "merge_sort_ml_rc xs = merge_ml_rc (merge_sort_ml_rc (take ((length xs) div 2) xs)) (merge_sort_ml_rc (drop ((length xs) div 2) xs))"

end
