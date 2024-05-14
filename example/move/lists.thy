theory lists
  imports Main
begin

fun AddListHead::"'a => 'a list => 'a list"where
"AddListHead a xs =a # xs "

fun AddListTail::"'a => 'a list  => 'a list"where
"AddListTail a [] =Cons a [] "|(*!*)
"AddListTail a (x#xs) = x # (AddListTail a xs ) "

fun AddListI::"nat => 'a => 'a list => 'a list"where
"AddListI i a [] = Cons a [] "|
"AddListI 0 a (x#xs) = AddListHead a (x#xs)"|
"AddListI (Suc i) a (x#xs) = x # (AddListI i a xs) "

fun DelListHead::"'a list => 'a list "where
"DelListHead (x#xs) = xs"|
"DelListHead [] = [] "

fun DelListTail::"'a list => 'a list "where
"DelListTail [] = []"|
"DelListTail (Cons a []) = []"|
"DelListTail (x#xs) = DelListTail xs "

fun DelListI::"nat => 'a list => 'a list "where
"DelListI i [] = []"|
"DelListI i (Cons a []) = []"|
"DelListI 0 (x#xs) = DelListHead (x#xs) "|
"DelListI (Suc i) (x#xs) = x # (DelListI i xs) "

fun SearchList::"'a => 'a list => bool "where
"SearchList a [] = False"|
"SearchList a (x#xs) = (if a=x then True else (SearchList a xs))"

fun Modify1::"'a => 'a =>  'a list  => 'a list"where
"Modify1 a b [] = [] "|
"Modify1 a b (x#xs) = (if x=a then b#(Modify1 a b xs) else x#(Modify1 a b xs)) "

fun Modify2::"nat => 'a => 'a list => 'a list "where
"Modify2 n b [] = []"|
"Modify2 0 b (x#xs) =(b#xs)"|
"Modify2 (Suc n) b (x#xs) = (x#(Modify2 n b xs))"

fun ffilter::"('a => bool)  => 'a list => 'a list "where
"ffilter f [] = [] "|
"ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))"

(*fun Reverse::"'a list => 'a list "where
"Reverse [] = []"|
"Reverse (Cons a []) =Cons a [] "|
"Reverse (xs#x) = x #( Reverse xs) "*)

fun app::  "'a list  => 'a list  =>  'a list " where 
"app Nil as =as"|
"app(Cons a as )bs= Cons a (app as bs)"

fun Reverse::  "'a list  => 'a list " where
"Reverse Nil =Nil"|
"Reverse (Cons a as) = app(Reverse as)(Cons a Nil)"

fun Insert::"'a::ord  =>  'a list  =>  'a list " where
"Insert a [] =Cons a [] "|
"Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs)) "

fun InsertSortPart::"('a::ord)list  =>  'a list  =>  'a list "where
"InsertSortPart [] ys=ys "|
"InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys) "

fun InsertSort::"('a::ord)list => 'a list " where
"InsertSort xs = InsertSortPart xs Nil"

fun Merge::"('a::ord)list  =>  'a list  =>  'a list "where
"Merge [] xs=xs"|
"Merge xs [] = xs"|
"Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys)) "

fun MergeSort::"('a::ord)list => 'a list " where
"MergeSort [] = []"|
"MergeSort (Cons a []) = [a] "|
"MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs)) "

end