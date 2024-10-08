theory defi_type
  imports Main
begin

datatype snat = sZero | sSuc snat
datatype 'a slist = sNil | sCons 'a "'a slist"
datatype 'a stree = sTip | sNode "'a stree " 'a "'a stree " 

fun natofsnat :: "snat \<Rightarrow> nat" where
  "natofsnat sZero = 0" |
  "natofsnat (sSuc n) = (natofsnat n) + 1"

fun snatofnat :: "nat \<Rightarrow> snat" where
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
"MergeSort (Cons a sNil) = [a] "|
"MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs)) "

fun bs :: "snat \<Rightarrow> snat slist \<Rightarrow> snat option" where
  "bs x [] = None" |
  "bs x [y] = If (x = y) (Some sZero) None" |
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

fun fib :: "snat \<Rightarrow> snat" where
  "fib sZero = (Suc sZero)" |
  "fib (Suc sZero) = (Suc sZero)" |
  "fib n = (fib (n - (Suc sZero))) + (fib (n - (Suc(Suc sZero))))"


fun copy_tree::"'a stree \<Rightarrow> 'a stree" where
"copy_tree (sNode left x right) = sNode (copy_tree left) x (copy_tree right)" |
"copy_tree sTip = sTip" 

fun searchtree1::"'a \<Rightarrow> 'a stree \<Rightarrow> bool " where
"searchtree1 a sTip=False "|
"searchtree1 a ( sNode left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right)) "

fun searchtree2::"'a \<Rightarrow> 'a stree \<Rightarrow> bool " where
"searchtree2 a sTip=False "|
"searchtree2 a ( sNode left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right)) "

fun searchtree3::"'a \<Rightarrow> 'a stree \<Rightarrow> bool " where
"searchtree3 a sTip=False "|
"searchtree3 a ( sNode left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) ) "

fun inserttree::"'a::ord \<Rightarrow> 'a stree \<Rightarrow> 'a stree " where
"inserttree a sTip= sNode sTip a sTip"|
"inserttree a ( sNode left x right) = (if a\<le>x then ( sNode (inserttree a left) x right) 
                                     else ( sNode left x (inserttree a right) ) )"

fun rightest::"'a stree \<Rightarrow> 'a" where
"rightest ( sNode left x right) = (if right= sTip then x 
                                 else (rightest right)) "
(*"rightest sTip=  "*)

fun rightestleft::"'a stree \<Rightarrow> 'a stree "where
"rightestleft sTip= sTip"|
"rightestleft ( sNode left x right) =(if right= sTip then left 
                                    else ( (rightestleft right)))  "

fun deltreeroot::"'a stree \<Rightarrow> 'a stree"where
"deltreeroot sTip= sTip "|
"deltreeroot ( sNode left x right) =(if right= sTip then left else if left= sTip then right else( sNode (rightestleft left)(rightest left)right ) ) "

fun deltree::"'a::ord => 'a stree => 'a stree"where
"deltree a sTip = sTip"|
"deltree a ( sNode left x right) =( if a=x then(deltreeroot( sNode left x right))
                                  else if a<x then ( sNode (deltree a left )x right) 
                                  else ( sNode left x (deltree a right)) ) "

fun changetree::"('a::ord) \<Rightarrow> 'a::ord \<Rightarrow> 'a stree \<Rightarrow> 'a stree " where
"changetree a b sTip = sTip" |
"changetree a b ( sNode left x right) = ( if (a=x) then ( sNode left b right)
                                        else if (a<x) then ( sNode (changetree a b left) x right)
                                        else ( sNode left x (changetree a b right)))"


end