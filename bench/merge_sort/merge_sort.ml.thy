theory merge_sort_ml
imports Main
begin

fun merge_ml :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge_ml xs [] = xs" |
  "merge_ml [] ys = ys" |
  "merge_ml (x # xs) (y # ys) = If (x \<le> y) (x # (merge_ml xs (y # ys))) (y # (merge_ml (x # xs) ys))"

fun merge_sort_ml :: "nat list \<Rightarrow> nat list" where
  "merge_sort_ml [] = []" |
  "merge_sort_ml [x] = [x]" |
  "merge_sort_ml xs = merge_ml (merge_sort_ml (take ((length xs) div 2) xs)) (merge_sort_ml (drop ((length xs) div 2) xs))"

end
