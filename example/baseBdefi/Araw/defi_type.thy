theory defi_type
  imports Main
begin

datatype 'a slist = sNil | sCons 'a "'a slist"
datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 


fun AddListHead::"'a =>'a slist=>'a slist"where
"AddListHead a xs = sCons a xs "

fun AddListTail::"'a =>'a slist =>'a slist"where
"AddListTail a sNil =sCons a sNil "|(*!*)
"AddListTail a (sCons x xs) = sCons x (AddListTail a xs ) "

fun AddListI::"nat =>'a =>'a slist=>'a slist"where
"AddListI i a sNil = sCons a sNil "|
"AddListI 0 a (sCons x xs) = AddListHead a (sCons x xs)"|
"AddListI (Suc i) a (sCons x xs) = sCons x (AddListI i a xs) "

fun DelListHead::"'a slist=>'a slist "where
"DelListHead (sCons x xs) = xs"|
"DelListHead sNil = sNil "

fun DelListTail::"'a slist =>'a slist "where
"DelListTail sNil = sNil"|
"DelListTail (sCons a sNil) = sNil"|
"DelListTail (sCons x xs) =  sCons x (DelListTail xs) "

fun DelListI::"nat =>'a slist =>'a slist "where
"DelListI i sNil = sNil"|
"DelListI i (sCons a sNil) = sNil"|
"DelListI 0 (sCons x xs) = DelListHead (sCons x xs) "|
"DelListI (Suc i) (sCons x xs) = sCons x (DelListI i xs) "

fun SearchList::"'a =>'a slist =>bool "where
"SearchList a sNil = False"|
"SearchList a (sCons x xs) = (if a=x then True else (SearchList a xs))"

fun Modify1::"'a =>'a => 'a slist =>'a slist"where
"Modify1 a b sNil = sNil "|
"Modify1 a b (sCons x xs) = (if x=a then sCons b (Modify1 a b xs) else sCons x (Modify1 a b xs)) "

fun Modify2::"nat =>'a =>'a slist =>'a slist "where
"Modify2 n b sNil = sNil"|
"Modify2 0 b (sCons x xs) =(sCons b xs)"|
"Modify2 (Suc n) b (sCons x xs) = (sCons x (Modify2 n b xs))"

fun ffilter::"('a=>bool) =>'a slist=>'a slist "where
"ffilter f sNil = sNil "|
"ffilter f(sCons x xs) = (if (f x) then (sCons x (ffilter f xs))else(ffilter f xs ))"

fun app::  "'a slist =>'a slist => 'a slist " where 
"app sNil as =as"|
"app(sCons a as )bs= sCons a (app as bs)"

fun Reverse::  "'a slist =>'a slist " where
"Reverse sNil =sNil"|
"Reverse (sCons a as) = app(Reverse as)(sCons a sNil)"

fun Reverse2::"'a slist=>'a slist "where
"Reverse2 sNil = sNil"|
"Reverse2 (sCons a sNil) =sCons a sNil "|
"Reverse2 (sCons x xs) = app ( Reverse2 xs) (sCons x sNil) "

fun Insert::"'a::ord => 'a slist => 'a slist " where
"Insert a sNil =sCons a sNil "|
"Insert a (sCons x xs) =(if a \<le> x then sCons a (sCons x xs) else sCons x (Insert a xs)) "

fun InsertSortPart::"('a::ord) slist => 'a slist => 'a slist "where
"InsertSortPart sNil ys = ys "|
"InsertSortPart (sCons x xs) ys = InsertSortPart xs (Insert x ys) "

fun InsertSort::"('a::ord) slist=>'a slist " where
"InsertSort xs = InsertSortPart xs sNil"

fun ssize :: "'a slist => nat" where  
  "ssize sNil = 0" |  
  "ssize (sCons x sNil) = (Suc 0)" |
  "ssize (sCons x xs) = Suc (ssize xs)"

fun slength :: "'a slist => nat" where  
  "slength sNil = 0" |  
  "slength (sCons x sNil) = (Suc 0)" |
  "slength (sCons x xs) = (Suc (slength xs))"

fun stake :: "nat => 'a slist => 'a slist" where  
  "stake 0 xs = sNil" |  
  "stake (Suc n) sNil = sNil" | 
  "stake (Suc n) (sCons x xs) =(sCons x (stake n xs))"  

fun sdrop :: "nat => 'a slist => 'a slist" where  
  "sdrop 0 xs = xs" |  
  "sdrop (Suc n) sNil = sNil" |  
  "sdrop (Suc n) (sCons x xs) = (sdrop n xs)"

fun Merge::"('a::ord) slist => 'a slist => 'a slist "where
"Merge sNil xs = xs"|
"Merge xs sNil = xs"|
"Merge (sCons x xs)(sCons y ys) = (if x\<le>y then (sCons x (Merge xs (sCons y ys)) )
                                              else (sCons y (Merge (sCons x xs)ys))) "




fun searchtree1::"'a =>'a tree =>bool " where
"searchtree1 a Tip = False "|
"searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right)) "

fun searchtree2::"'a =>'a tree =>bool " where
"searchtree2 a Tip = False "|
"searchtree2 a (Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right)) "

fun searchtree3::"'a => 'a tree => bool " where
"searchtree3 a Tip = False "|
"searchtree3 a (Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) ) "

fun inserttree::"'a::ord => 'a tree => 'a tree " where
"inserttree a Tip = Node Tip a Tip"|
"inserttree a (Node left x right) = (if a\<le>x then (Node (inserttree a left) x right) 
                                     else (Node left x (inserttree a right) ) )"

fun transtolist:: "'a tree =>'a slist " where
"transtolist Tip = sNil "|
(*"transtolist (Node Tip a Tip) = [a]  "|*)
"transtolist (Node left a right) =(sCons a (app (transtolist left) (transtolist right))) "

fun rightest::"'a tree => 'a" where
"rightest (Node left x right) = (if right=Tip then x 
                                 else (rightest right)) "
(*"rightest Tip=  "*)

fun rightestleft::"'a tree => 'a tree "where
"rightestleft Tip = Tip"|
"rightestleft (Node left x right) =(if right=Tip then left 
                                    else ( (rightestleft right)))  "

fun deltreeroot::"'a tree => 'a tree"where
"deltreeroot Tip = Tip "|
"deltreeroot (Node left x right) =(if right=Tip then left else if left=Tip then right else(Node (rightestleft left)(rightest left)right ) ) "

fun deltree::"'a::ord => 'a tree => 'a tree"where
"deltree a Tip = Tip"|
"deltree a (Node left x right) =( if a=x then(deltreeroot(Node left x right))
                                  else if a<x then (Node (deltree a left )x right) 
                                  else (Node left x (deltree a right)) ) "

fun changetree::"('a::ord) => 'a::ord => 'a tree => 'a tree " where
"changetree a b Tip = Tip" |
"changetree a b (Node left x right) = ( if (a=x) then (Node left b right)
                                        else if (a<x) then (Node (changetree a b left) x right)
                                        else (Node left x (changetree a b right)))"



end