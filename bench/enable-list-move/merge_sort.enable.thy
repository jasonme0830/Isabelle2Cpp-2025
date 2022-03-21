theory merge_sort
imports Main
begin

fun merge2 :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge2 xs [] = xs" |
  "merge2 [] ys = ys" |
  "merge2 (x # xs) (y # ys) = If (x \<le> y) (x # (merge2 xs (y # ys))) (y # (merge2 (x # xs) ys))"

fun merge_sort2 :: "nat list \<Rightarrow> nat list" where
  "merge_sort2 [] = []" |
  "merge_sort2 [x] = [x]" |
  "merge_sort2 xs = merge2 (merge_sort2 (take ((length xs) div 2) xs)) (merge_sort2 (drop ((length xs) div 2) xs))"

end
