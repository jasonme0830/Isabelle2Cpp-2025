theory merge_sort
imports Main
begin

datatype 'a slist = sNil | sCons 'a "'a slist"

fun merge :: "nat list \<Rightarrow> nat list \<Rightarrow> nat list" where
  "merge xs [] = xs" |
  "merge [] ys = ys" |
  "merge (x # xs) (y # ys) = If (x \<le> y) (x # (merge xs (y # ys))) (y # (merge (x # xs) ys))"

fun merge_sort :: "nat list \<Rightarrow> nat list" where
  "merge_sort [] = []" |
  "merge_sort [x] = [x]" |
  "merge_sort xs = merge (merge_sort (take ((length xs) div 2) xs)) (merge_sort (drop ((length xs) div 2) xs))"

fun slength :: "'a slist \<Rightarrow> nat" where
  "slength sNil = 0" |
  "slength (sCons x xs) = Suc (slength xs)"

fun stake :: "nat \<Rightarrow> 'a slist \<Rightarrow> 'a slist" where
  "stake n sNil = sNil" |
  "stake 0 _ = sNil" |
  "stake (Suc m) (sCons x xs) = sCons x (stake m xs)"

fun sdrop :: "nat \<Rightarrow> 'a slist \<Rightarrow> 'a slist" where
  "sdrop n sNil = sNil" |
  "sdrop 0 xs = xs" |
  "sdrop (Suc m) (sCons x xs) = sdrop m xs"

fun smerge :: "nat slist \<Rightarrow> nat slist \<Rightarrow> nat slist" where
  "smerge xs sNil = xs" |
  "smerge sNil ys = ys" |
  "smerge (sCons x xs) (sCons y ys) = If (x \<le> y) (sCons x (smerge xs (sCons y ys))) (sCons y (smerge (sCons x xs) ys))"

fun smerge_sort :: "nat slist \<Rightarrow> nat slist" where
  "smerge_sort sNil = sNil" |
  "smerge_sort (sCons x sNil) = sCons x sNil" |
  "smerge_sort xs = smerge (smerge_sort (stake ((slength xs) div 2) xs)) (smerge_sort (sdrop ((slength xs) div 2) xs))"

end
