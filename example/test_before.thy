theory binary_tree
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun Merge::"('a::ord)list => 'a list => 'a list "where
"Merge [] xs=xs"|
"Merge xs [] = xs"|
"Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys)) "

fun MergeSort::"('a::ord)list=>'a list " where
"MergeSort [] = []"|
"MergeSort (Cons a []) = [a] "|
"MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs)) "

end