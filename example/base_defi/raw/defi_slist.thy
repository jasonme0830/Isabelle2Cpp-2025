theory defi_slist
  imports Main
begin

datatype snat = sZero | sSuc snat
datatype 'a slist = sNil | sCons 'a "'a slist"

fun natofsnat :: "snat => nat" where
  "natofsnat sZero = 0" |
  "natofsnat (sSuc n) = (natofsnat n) + 1"

fun snatofnat :: "nat => snat" where
  "snatofnat 0 = sZero" |
  "snatofnat (Suc n) = sSuc (snatofnat n)"

fun AddListHead::"'a=>'a slist=>'a slist"where
"AddListHead a xs = sCons a  xs "

fun AddListTail::"'a=>'a slist =>'a slist"where
"AddListTail a sNil =sCons a sNil "|
"AddListTail a (sCons x xs) = sCons x  (AddListTail a xs ) "

fun AddListI::"snat=>'a=>'a slist=>'a slist"where
"AddListI i a sNil = sCons a sNil "|
"AddListI sZero a (sCons x xs) = AddListHead a (sCons x xs)"|
"AddListI (sSuc i) a (sCons x xs) = sCons x  (AddListI i a xs) "

fun DelListHead::"'a slist=>'a slist "where
"DelListHead (sCons x xs) = xs"|
"DelListHead sNil = sNil "

fun DelListTail::"'a slist=>'a slist "where
"DelListTail sNil = sNil"|
"DelListTail (sCons a sNil) = sNil"|
"DelListTail (sCons x xs) =  sCons x (DelListTail xs) "

fun DelListI::"snat=>'a slist=>'a slist "where
"DelListI i sNil = sNil "|
"DelListI i (sCons a sNil) = sNil "|
"DelListI sZero (sCons x xs) = DelListHead (sCons x xs) "|
"DelListI (sSuc i) (sCons x xs) = sCons x  (DelListI i xs) "

fun Modify1::"'a=>'a=> 'a slist =>'a slist"where
"Modify1 a b sNil = sNil "|
"Modify1 a b (sCons x xs) = (if x=a then (sCons b (Modify1 a b xs)) else (sCons x (Modify1 a b xs))) "

fun Modify2::"snat=>'a=>'a slist=>'a slist "where
"Modify2 n b sNil = sNil"|
"Modify2 sZero b (sCons x xs) =(sCons b xs)"|
"Modify2 (sSuc n) b (sCons x xs) = (sCons x (Modify2 n b xs))"

fun SearchList::"'a=>'a slist=>bool "where
"SearchList a sNil = False"|
"SearchList a (sCons x xs) = (if a=x then True else (SearchList a xs))"

fun app::  "'a slist =>'a slist => 'a slist " where 
"app sNil as =as"|
"app(sCons a as ) bs= sCons a (app as bs)"

fun Reverse::  "'a slist =>'a slist " where
"Reverse sNil = sNil"|
"Reverse (sCons a as) = app(Reverse as)(sCons a sNil)"


end