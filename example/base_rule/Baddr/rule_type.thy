theory rule_type
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 


fun AddListHead::"'a =>'a list=>'a list"where
"AddListHead a xs =a # xs "

fun AddListTail::"'a =>'a list =>'a list"where
"AddListTail a [] =Cons a [] "|(*!*)
"AddListTail a (x#xs) = x # (AddListTail a xs ) "

fun AddListI::"nat =>'a =>'a list=>'a list"where
"AddListI i a [] = Cons a [] "|
"AddListI 0 a (x#xs) = AddListHead a (x#xs)"|
"AddListI (Suc i) a (x#xs) = x # (AddListI i a xs) "

fun DelListHead::"'a list=>'a list "where
"DelListHead (x#xs) = xs"|
"DelListHead [] = [] "

fun DelListTail::"'a list =>'a list "where
"DelListTail [] = []"|
"DelListTail (Cons a []) = []"|
"DelListTail (x#xs) =  Cons x (DelListTail xs) "

fun DelListI::"nat =>'a list =>'a list "where
"DelListI i [] = []"|
"DelListI i (Cons a []) = []"|
"DelListI 0 (x#xs) = DelListHead (x#xs) "|
"DelListI (Suc i) (x#xs) = x # (DelListI i xs) "

fun SearchList::"'a =>'a list =>bool "where
"SearchList a [] = False"|
"SearchList a (x#xs) = (if a=x then True else (SearchList a xs))"

fun ModifyValue::"'a =>'a => 'a list =>'a list"where
"ModifyValue a b [] = [] "|
"ModifyValue a b (x#xs) = (if x=a then b#(ModifyValue a b xs) else x#(ModifyValue a b xs)) "

fun ModifyIndex::"nat =>'a =>'a list =>'a list "where
"ModifyIndex n b [] = []"|
"ModifyIndex 0 b (x#xs) =(b#xs)"|
"ModifyIndex (Suc n) b (x#xs) = (x#(ModifyIndex n b xs))"

fun ffilter::"('a=>bool) =>'a list=>'a list "where
"ffilter f [] = [] "|
"ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))"

fun app::  "'a list =>'a list => 'a list " where 
"app Nil as =as"|
"app(Cons a as )bs= Cons a (app as bs)"

fun Reverse::  "'a list =>'a list " where
"Reverse Nil =Nil"|
"Reverse (Cons a as) = app(Reverse as)(Cons a Nil)"

fun Reverse2::"'a list=>'a list "where
"Reverse2 [] = []"|
"Reverse2 (Cons a []) =Cons a [] "|
"Reverse2 (x#xs) = ( Reverse2 xs)@ (Cons x Nil) "

fun Insert::"'a::ord => 'a list => 'a list " where
"Insert a [] =Cons a [] "|
"Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs)) "

fun InsertSortPart::"('a::ord)list => 'a list => 'a list "where
"InsertSortPart [] ys=ys "|
"InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys) "

fun InsertSort::"('a::ord)list=>'a list " where
"InsertSort xs = InsertSortPart xs Nil"


fun Merge::"('a::ord)list => 'a list => 'a list "where
"Merge [] xs=xs"|
"Merge xs [] = xs"|
"Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys)) "

fun MergeSort::"('a::ord)list=>'a list " where
"MergeSort [] = []"|
"MergeSort (Cons a []) = [a] "|
"MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs)) "

fun bs :: "nat \<Rightarrow> nat list \<Rightarrow> nat option" where
  "bs x [] = None" |
  "bs x [y] = If (x = y) (Some 0) None" |
  "bs x ys = (let m = (length ys) div 2 in
      let y = ys ! m in
        If (y = x)
          (Some m)
          (If (y < x)
            (case bs x (drop (m + 1) ys) of Some n \<Rightarrow> Some (m + n + 1) | None \<Rightarrow> None)
            (bs x (take m ys)
          )
      )
  )"

fun fib :: "nat \<Rightarrow> nat" where
  "fib 0 = 1" |
  "fib (Suc 0) = 1" |
  "fib n = (fib (n - 1)) + (fib (n - 2))"

fun slength :: "'a list => nat" where  
  "slength [] = 0" |  
  "slength (x # xs) = 1 + slength xs"

fun stake :: "nat => 'a list => 'a list" where  
  "stake 0 xs = []" |  
  "stake (Suc n) [] = []" |  
  "stake (Suc n) (x # xs) = x # stake n xs" 

fun sdrop :: "nat => 'a list => 'a list" where  
  "sdrop 0 xs = xs" |  
  "sdrop (Suc n) [] = []" |  
  "sdrop (Suc n) (x # xs) = sdrop n xs" 

fun supto :: "nat => nat => nat list" where  
  "supto i j = (if i \<ge> j then [] else i # supto (i + 1) j)"

fun snth :: "'a list => nat => 'a" where  
  "snth [] n = 0" |  
  "snth (x#xs) 0 = x" |  
  "snth (x#xs) (Suc n) = snth xs n" 




end