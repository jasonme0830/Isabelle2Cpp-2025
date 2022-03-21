theory merge_sort
imports Main
begin

fun merge1 :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge1 xs [] = xs" |
  "merge1 [] ys = ys" |
  "merge1 (x # xs) (y # ys) = If (x \<le> y) (x # (merge1 xs (y # ys))) (y # (merge1 (x # xs) ys))"

fun merge_sort1 :: "nat list \<Rightarrow> nat list" where
  "merge_sort1 [] = []" |
  "merge_sort1 [x] = [x]" |
  "merge_sort1 xs = merge1 (merge_sort1 (take ((length xs) div 2) xs)) (merge_sort1 (drop ((length xs) div 2) xs))"

end
