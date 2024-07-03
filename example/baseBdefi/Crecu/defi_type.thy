theory defi_type
  imports Main
begin

datatype 'a slist = sNil | sCons 'a "'a slist"
datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun AddListHead::"'a=>'a slist=>'a slist"where
"AddListHead a xs = sCons a  xs "

fun AddListTail::"'a=>'a slist =>'a slist"where
"AddListTail a sNil =sCons a sNil "|
"AddListTail a (sCons x xs) = sCons x  (AddListTail a xs ) "

fun AddListI::"nat=>'a=>'a slist=>'a slist"where
"AddListI i a sNil = sCons a sNil "|
"AddListI Zero a (sCons x xs) = AddListHead a (sCons x xs)"|
"AddListI (Suc i) a (sCons x xs) = sCons x  (AddListI i a xs) "

fun DelListHead::"'a slist=>'a slist "where
"DelListHead (sCons x xs) = xs"|
"DelListHead sNil = sNil "

fun DelListTail::"'a slist=>'a slist "where
"DelListTail sNil = sNil"|
"DelListTail (sCons a sNil) = sNil"|
"DelListTail (sCons x xs) =  sCons x (DelListTail xs) "

fun DelListI::"nat=>'a slist=>'a slist "where
"DelListI i sNil = sNil "|
"DelListI i (sCons a sNil) = sNil "|
"DelListI Zero (sCons x xs) = DelListHead (sCons x xs) "|
"DelListI (Suc i) (sCons x xs) = sCons x  (DelListI i xs) "

fun Modify1::"'a=>'a=> 'a slist =>'a slist"where
"Modify1 a b sNil = sNil "|
"Modify1 a b (sCons x xs) = (if x=a then (sCons b (Modify1 a b xs)) else (sCons x (Modify1 a b xs))) "

fun Modify2::"nat=>'a=>'a slist=>'a slist "where
"Modify2 n b sNil = sNil"|
"Modify2 Zero b (sCons x xs) =(sCons b xs)"|
"Modify2 (Suc n) b (sCons x xs) = (sCons x (Modify2 n b xs))"

fun SearchList::"'a=>'a slist=>bool "where
"SearchList a sNil = False"|
"SearchList a (sCons x xs) = (if a=x then True else (SearchList a xs))"

fun app::  "'a slist =>'a slist => 'a slist " where 
"app sNil as =as"|
"app(sCons a as ) bs= sCons a (app as bs)"

fun Reverse::  "'a slist =>'a slist " where
"Reverse sNil = sNil"|
"Reverse (sCons a as) = app(Reverse as)(sCons a sNil)"


fun searchtree1::"'a \<Rightarrow> 'a tree \<Rightarrow> bool " where
"searchtree1 a Tip=False "|
"searchtree1 a ( Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right)) "

fun searchtree2::"'a \<Rightarrow> 'a tree \<Rightarrow> bool " where
"searchtree2 a Tip=False "|
"searchtree2 a ( Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right)) "

fun searchtree3::"'a \<Rightarrow> 'a tree \<Rightarrow> bool " where
"searchtree3 a Tip=False "|
"searchtree3 a ( Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) ) "

fun inserttree::"'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a tree " where
"inserttree a Tip= Node Tip a Tip"|
"inserttree a ( Node left x right) = (if a\<le>x then ( Node (inserttree a left) x right) 
                                     else ( Node left x (inserttree a right) ) )"

fun rightest::"'a tree \<Rightarrow> 'a" where
"rightest ( Node left x right) = (if right= Tip then x 
                                 else (rightest right)) "
(*"rightest Tip=  "*)

fun rightestleft::"'a tree \<Rightarrow> 'a tree "where
"rightestleft Tip= Tip"|
"rightestleft ( Node left x right) =(if right= Tip then left 
                                    else ( (rightestleft right)))  "

fun deltreeroot::"'a tree \<Rightarrow> 'a tree"where
"deltreeroot Tip= Tip "|
"deltreeroot ( Node left x right) =(if right= Tip then left else if left= Tip then right else( Node (rightestleft left)(rightest left)right ) ) "

fun deltree::"'a::ord => 'a tree => 'a tree"where
"deltree a Tip = Tip"|
"deltree a ( Node left x right) =( if a=x then(deltreeroot( Node left x right))
                                  else if a<x then ( Node (deltree a left )x right) 
                                  else ( Node left x (deltree a right)) ) "

fun changetree::"('a::ord) \<Rightarrow> 'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a tree " where
"changetree a b Tip = Tip" |
"changetree a b ( Node left x right) = ( if (a=x) then ( Node left b right)
                                        else if (a<x) then ( Node (changetree a b left) x right)
                                        else ( Node left x (changetree a b right)))"


end