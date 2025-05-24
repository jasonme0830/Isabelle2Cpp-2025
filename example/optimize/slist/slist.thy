theory defi
  imports Main
begin

datatype snat = sZero | sSuc snat
datatype 'a slist = sNil | sCons 'a "'a slist"
datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun natofsnat :: "snat \<Rightarrow> nat" where
  "natofsnat sZero = 0" |
  "natofsnat (sSuc n) = (natofsnat n) + 1"

fun snatofnat :: "nat \<Rightarrow> snat" where
  "snatofnat 0 = sZero" |
  "snatofnat (Suc n) = sSuc (snatofnat n)"



fun AddListHead::"'a=>'a slist=>'a slist"where
"AddListHead a xs = sCons a  xs "
fun DelListHead::"'a slist=>'a slist "where
"DelListHead (sCons x xs) = xs"|
"DelListHead sNil = sNil "

fun AddListTail::"'a=>'a slist =>'a slist"where
"AddListTail a sNil =sCons a sNil "|
"AddListTail a (sCons x xs) = sCons x  (AddListTail a xs ) "
fun DelListTail::"'a slist=>'a slist "where
"DelListTail sNil = sNil"|
"DelListTail (sCons a sNil) = sNil"|
"DelListTail (sCons x xs) =  sCons x (DelListTail xs) "

fun AddListIs::"snat=>'a=>'a slist=>'a slist"where
"AddListIs i a sNil = sCons a sNil "|
"AddListIs sZero a (sCons x xs) = AddListHead a (sCons x xs)"|
"AddListIs (sSuc i) a (sCons x xs) = sCons x  (AddListIs i a xs) "
fun DelListIs::"snat=>'a slist=>'a slist "where
"DelListIs i sNil = sNil "|
"DelListIs i (sCons a sNil) = sNil "|
"DelListIs sZero (sCons x xs) = DelListHead (sCons x xs) "|
"DelListIs (sSuc i) (sCons x xs) = sCons x  (DelListIs i xs) "

fun AddListI::"nat=>'a=>'a slist=>'a slist"where
"AddListI i a sNil = sCons a sNil "|
"AddListI Zero a (sCons x xs) = AddListHead a (sCons x xs)"|
"AddListI (Suc i) a (sCons x xs) = sCons x  (AddListI i a xs) "
fun DelListI::"nat=>'a slist=>'a slist "where
"DelListI i sNil = sNil "|
"DelListI i (sCons a sNil) = sNil "|
"DelListI Zero (sCons x xs) = DelListHead (sCons x xs) "|
"DelListI (Suc i) (sCons x xs) = sCons x  (DelListI i xs) "

fun ModifyValue::"'a=>'a=> 'a slist =>'a slist"where
"ModifyValue a b sNil = sNil "|
"ModifyValue a b (sCons x xs) = (if x=a then (sCons b (ModifyValue a b xs)) else (sCons x (ModifyValue a b xs))) "
fun ModifyIndex::"snat=>'a=>'a slist=>'a slist "where
"ModifyIndex n b sNil = sNil"|
"ModifyIndex sZero b (sCons x xs) =(sCons b xs)"|
"ModifyIndex (sSuc n) b (sCons x xs) = (sCons x (ModifyIndex n b xs))"
fun SearchList::"'a=>'a slist=>bool "where
"SearchList a sNil = False"|
"SearchList a (sCons x xs) = (if a=x then True else (SearchList a xs))"

fun app::  "'a slist =>'a slist => 'a slist " where 
"app sNil as =as"|
"app(sCons a as ) bs= sCons a (app as bs)"
fun Reverse::  "'a slist =>'a slist " where
"Reverse sNil = sNil"|
"Reverse (sCons a as) = app(Reverse as)(sCons a sNil)"


fun Insert::"'a::ord => 'a slist => 'a slist " where
"Insert a sNil =sCons a sNil "|
"Insert a (sCons x xs) =(if a \<le> x then (sCons a (sCons x xs)) else (sCons x (Insert a xs))) "
fun InsertSortPart::"('a::ord)slist => 'a slist => 'a slist "where
"InsertSortPart sNil ys=ys "|
"InsertSortPart (sCons x xs) ys=InsertSortPart xs (Insert x ys) "
fun InsertSort::"('a::ord)slist=>'a slist " where
"InsertSort xs = InsertSortPart xs sNil"

fun Merge::"('a::ord)list => 'a slist => 'a slist "where
"Merge sNil xs=xs"|
"Merge xs sNil = xs"|
"Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys)) "
fun MergeSort::"('a::ord)list=>'a slist " where
"MergeSort sNil = sNil "|
"MergeSort  (sCons a sNil) = [a] "|
"MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs)) "


fun ssize :: "'a slist => nat" where  
  "ssize sNil = 0" |  
  "ssize  (sCons x sNil) = (Suc 0)" |
  "ssize  (sCons x xs) = Suc (ssize xs)"
fun slength :: "'a slist => nat" where  
  "slength sNil = 0" |  
  "slength  (sCons x sNil) = (Suc 0)" |
  "slength  (sCons x xs) = (Suc (slength xs))"
fun stake :: "nat => 'a slist => 'a slist" where  
  "stake 0 xs = sNil" |  
  "stake (Suc n) sNil = sNil" | 
  "stake (Suc n)  (sCons x xs) = (sCons x (stake n xs))"  
fun sdrop :: "nat => 'a slist => 'a slist" where  
  "sdrop 0 xs = xs" |  
  "sdrop (Suc n) sNil = sNil" |  
  "sdrop (Suc n)  (sCons x xs) = (sdrop n xs)"
fun supto :: "nat => nat => nat slist" where  
  "supto i j = (if i \<ge> j then sNil else (sCons i  (supto (i + 1) j)))"
fun snth :: "'a slist => nat => 'a" where  
  "snth sNil n = 0" |  
  "snth (x#xs) 0 = x" |  
  "snth (x#xs) (Suc n) = snth xs n" 




end