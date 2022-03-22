theory merge_sort
imports Main
begin

fun merge :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge xs [] = xs" |
  "merge [] ys = ys" |
  "merge (x # xs) (y # ys) = If (x \<le> y) (x # (merge xs (y # ys))) (y # (merge (x # xs) ys))"

fun merge_sort :: "nat list \<Rightarrow> nat list" where
  "merge_sort [] = []" |
  "merge_sort [x] = [x]" |
  "merge_sort xs = merge (merge_sort (take ((length xs) div 2) xs)) (merge_sort (drop ((length xs) div 2) xs))"

end
