theory merge_sort_ml_ud
imports Main
begin

fun merge_ml_ud :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge_ml_ud xs [] = xs" |
  "merge_ml_ud [] ys = ys" |
  "merge_ml_ud (x # xs) (y # ys) = If (x \<le> y) (x # (merge_ml_ud xs (y # ys))) (y # (merge_ml_ud (x # xs) ys))"

fun merge_sort_ml_ud :: "nat list \<Rightarrow> nat list" where
  "merge_sort_ml_ud [] = []" |
  "merge_sort_ml_ud [x] = [x]" |
  "merge_sort_ml_ud xs = merge_ml_ud (merge_sort_ml_ud (take ((length xs) div 2) xs)) (merge_sort_ml_ud (drop ((length xs) div 2) xs))"

end
