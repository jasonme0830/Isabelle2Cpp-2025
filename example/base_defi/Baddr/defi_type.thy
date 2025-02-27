theory snat_type
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
fun AddListI::"snat=>'a=>'a slist=>'a slist"where
"AddListI i a sNil = sCons a sNil "|
"AddListI sZero a (sCons x xs) = AddListHead a (sCons x xs)"|
"AddListI (sSuc i) a (sCons x xs) = sCons x  (AddListI i a xs) "
fun DelListTail::"'a slist=>'a slist "where
"DelListTail sNil = sNil"|
"DelListTail (sCons a sNil) = sNil"|
"DelListTail (sCons x xs) =  sCons x (DelListTail xs) "
fun DelListI::"snat=>'a slist=>'a slist "where
"DelListI i sNil = sNil "|
"DelListI i (sCons a sNil) = sNil "|
"DelListI sZero (sCons x xs) = DelListHead (sCons x xs) "|
"DelListI (sSuc i) (sCons x xs) = sCons x  (DelListI i xs) "
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
fun Insert::"'a::ord => 'a slist => 'a slist " where
"Insert a sNil =sCons a sNil "|
"Insert a (sCons x xs) =(if a \<le> x then (sCons a (sCons x xs)) else (sCons x (Insert a xs))) "
fun Reverse::  "'a slist =>'a slist " where
"Reverse sNil = sNil"|
"Reverse (sCons a as) = app(Reverse as)(sCons a sNil)"



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
  "supto i j = (if i \<ge> j then sNil else i # supto (i + 1) j)"
fun snth :: "'a slist => nat => 'a" where  
  "snth sNil n = 0" |  
  "snth (x#xs) 0 = x" |  
  "snth (x#xs) (Suc n) = snth xs n" 


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
fun addtreeroot::"'a => 'a tree => 'a tree" where
"addtreeroot x Tip = Node Tip x Tip" |
"addtreeroot x (Node left m Tip) = Node left x (Node Tip m Tip)" |
"addtreeroot x (Node Tip m right) = Node (Node Tip m Tip) x right" |
"addtreeroot x (Node left m right) = (Node (Node left m right) x Tip)"


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