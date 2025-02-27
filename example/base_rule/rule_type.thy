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



fun two_fib::" nat => nat" where
"two_fib 0 = 1" |
"two_fib (Suc 0) = 1" |
"two_fib n = (two_fib (n-1)) + (two_fib (n-2))"

fun three_fib::" nat => nat" where
"three_fib 0 = 1" |
"three_fib (Suc 0) = 1" |
"three_fib (Suc (Suc 0)) = 1" |
"three_fib n = (three_fib (n-1) + three_fib (n-2) + three_fib(n-3))"

fun four_fib::" nat => nat" where
"four_fib 0 = 1" |
"four_fib (Suc 0) = 1" |
"four_fib (Suc (Suc 0)) = 1" |
"four_fib (Suc (Suc (Suc 0))) = 1" |
"four_fib n = (four_fib(n-1) + four_fib(n-2) + four_fib(n-3) + four_fib(n-4))"

fun Jacobsthal::" nat => nat" where
"Jacobsthal 0 = 2" |
"Jacobsthal (Suc 0) = 1" |
"Jacobsthal n = (Jacobsthal (n-1)) + 2*(Jacobsthal (n-2))"

fun Narayana::" nat => nat" where
"Narayana 0 = 1" |
"Narayana (Suc 0) = 1" |
"Narayana (Suc (Suc 0)) = 1" |
"Narayana n = Narayana(n-1) + Narayana(n-3)"

fun Leonardo::" nat => nat" where
"Leonardo 0 = 1" |
"Leonardo (Suc 0) = 1" |
"Leonardo n = Leonardo(n-1) + Leonardo(n-2) + 1"

fun Padua::" nat => nat" where
"Padua 0 = 1" |
"Padua (Suc 0) = 1" |
"Padua (Suc (Suc 0)) = 1" |
"Padua n = Padua(n-2) + Padua(n-3)"

fun Pell::" nat => nat" where
"Pell 0 = 0" |
"Pell (Suc 0) = 1" |
"Pell n = (Pell(n-1)*2)+Pell(n-2)"

fun Lucas::" nat => nat" where
"Lucas 0 = 2" |
"Lucas (Suc 0) = 1" |
"Lucas n = Lucas(n-1) + Lucas(n-2)"




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



fun copy_tree::"'a tree \<Rightarrow> 'a tree" where
"copy_tree (Node left x right) = Node (copy_tree left) x (copy_tree right)" |
"copy_tree Tip = Tip" 

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

fun transtolist:: "'a tree =>'a list " where
"transtolist Tip = [] "|
(*"transtolist (Node Tip a Tip) = [a]  "|*)
"transtolist (Node left a right) =( a # (transtolist left)@(transtolist right)) "

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

fun addtreeroot::"'a => 'a tree => 'a tree" where
"addtreeroot x Tip = Node Tip x Tip" |
"addtreeroot x (Node left m Tip) = Node left x (Node Tip m Tip)" |
"addtreeroot x (Node Tip m right) = Node (Node Tip m Tip) x right" |
"addtreeroot x (Node left m right) = (Node (Node left m right) x Tip)"


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

fun sorttree::"'a tree =>('a::ord) list "where
"sorttree Tip = [] "|
"sorttree (Node Tip a Tip) =[a] "|
"sorttree (Node left x right) =(MergeSort (transtolist (Node left x right)))"


end