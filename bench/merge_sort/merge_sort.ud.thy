theory merge_sort_ud
imports Main
begin

fun merge_ud :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge_ud xs [] = xs" |
  "merge_ud [] ys = ys" |
  "merge_ud (x # xs) (y # ys) = If (x \<le> y) (x # (merge_ud xs (y # ys))) (y # (merge_ud (x # xs) ys))"

fun merge_sort_ud :: "nat list \<Rightarrow> nat list" where
  "merge_sort_ud [] = []" |
  "merge_sort_ud [x] = [x]" |
  "merge_sort_ud xs = merge_ud (merge_sort_ud (take ((length xs) div 2) xs)) (merge_sort_ud (drop ((length xs) div 2) xs))"

end
