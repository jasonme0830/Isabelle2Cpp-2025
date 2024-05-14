theory funcs
imports Main
begin


datatype 'b tree = Tip | Node "'b tree" 'b "'b tree"

fun applist::"'a list  \<Rightarrow> 'a list  \<Rightarrow> 'a list" where
    "applist Nil ys = ys"|
    "applist (Cons x xs) ys = Cons x(applist xs ys)"

fun addlistH::"('a::ord)  \<Rightarrow> 'a list  \<Rightarrow> 'a list" where
    "addlistH a Nil = (Cons a Nil)" |
    "addlistH a xs = (Cons a xs)"
fun addlistE::" 'a  \<Rightarrow> 'a list  \<Rightarrow> 'a list" where
    "addlistE a Nil = Cons a Nil" |
    "addlistE a (Cons x xs) = Cons x (addlistE a xs) "
fun addlistN::" 'a  \<Rightarrow> 'a list  \<Rightarrow> nat  \<Rightarrow> 'a list" where
    "addlistN a Nil n = Cons a Nil" |
    "addlistN a xs 0 = Cons a xs" |
    "addlistN a (Cons x xs) (Suc n) = (Cons x (addlistN a xs n)) "

fun dellistH::" 'a list  \<Rightarrow> 'a list" where
    "dellistH Nil = Nil" |
    "dellistH (Cons x xs) = xs" 
fun dellistE::" 'a list  \<Rightarrow> 'a list" where
    "dellistE Nil = Nil" |
    "dellistE (Cons x Nil) = Nil" |
    "dellistE (Cons x xs) = Cons x (dellistE xs)" 
fun dellistN::" 'a list  \<Rightarrow> nat  \<Rightarrow> 'a list" where
    "dellistN Nil n = Nil" |
    "dellistN (Cons x xs) 0 = xs" |
    "dellistN (Cons x xs) (Suc n) = (Cons x (dellistN xs n)) " 
fun dellistF::" 'a  \<Rightarrow> 'a list  \<Rightarrow> 'a list" where
    "dellistF a Nil = Nil" |
    "dellistF a (Cons x xs) = (if a=x then xs 
                              else (Cons x (dellistF a xs))) "

fun changelistF::" 'a  \<Rightarrow> 'a list  \<Rightarrow> 'a   \<Rightarrow> 'a list" where
    "changelistF a Nil b = Nil" |
    "changelistF a (Cons x xs) b = (if a=x then (Cons b xs)
                                  else (Cons x (changelistF a xs b)))" 
fun changelistN::" 'a list  \<Rightarrow> nat  \<Rightarrow> 'a  \<Rightarrow> 'a list" where 
    "changelistN Nil n a = Nil" |
    "changelistN (Cons x xs) 0 a = Cons a xs" |
    "changelistN (Cons x xs) (Suc n) a = Cons x (changelistN xs n a)" 

fun searchlist::" 'a  \<Rightarrow> 'a list  \<Rightarrow> bool" where
    "searchlist a Nil = False" |
    "searchlist a (Cons x xs) = (if a=x then True else (searchlist a xs))"
 
fun revlist::" 'a list  \<Rightarrow> 'a list  \<Rightarrow> 'a list" where
    "revlist Nil ys = ys" |
    "revlist (Cons x xs) ys = revlist xs (Cons x ys)" 

fun insertsort::" ('a::ord)  \<Rightarrow> 'a list  \<Rightarrow> 'a list" where
    "insertsort a Nil = Cons a Nil" |
    "insertsort a (Cons x xs) = (if a<x then (Cons a (Cons x xs))
                                 else (Cons x (insertsort a xs)))"


fun addtree::" ('b::ord)  \<Rightarrow> 'b tree  \<Rightarrow> 'b tree" where
    "addtree b Tip = (Node Tip b Tip)" |
    "addtree b (Node left x right) = (if b=x then (Node left x right)
                                      else if b<x then (addtree b left)
                                      else addtree b right)"

fun smallest::" 'b tree  \<Rightarrow> 'b" where
    "smallest Tip = undefined" | (*no necessary*)
    "smallest (Node left x right) = (if left=Tip then x 
                                    else smallest left)" 
fun deltree::" 'b tree  \<Rightarrow> ('b::ord)  \<Rightarrow> 'b tree" where
    "deltree Tip n = Tip" |
    "deltree (Node Tip x Tip) n = (if x=n then Tip
                                  else (Node Tip x Tip))"|
    "deltree (Node left x Tip) n = (if x=n then left 
                                    else (Node left x Tip))" |
    "deltree (Node left x right) n = (if n<x then deltree left n
                                      else if n>x then deltree right n
             else (Node left (smallest right) (deltree right (smallest right))))"

fun changetree::" ('b::ord)  \<Rightarrow> 'b tree  \<Rightarrow> ('b::ord)  \<Rightarrow> 'b tree" where
    "changetree b Tip n = Tip" |
    "changetree b (Node left x right) n = (if b=x then (Node left n right)
                                           else if b<x then changetree b left n
                                           else changetree b right n)" 

fun searchtreeF::" ('b::ord)  \<Rightarrow> 'b tree  \<Rightarrow> bool" where
    "searchtreeF b Tip = False" |
    "searchtreeF b (Node left x right) = (if b=x then True
                                       else if (searchtreeF b left) =True then True
                                       else if (searchtreeF b right) =True then True
                                       else False)"
fun searchtreeM::" ('b::ord)  \<Rightarrow> 'b tree  \<Rightarrow> bool" where
    "searchtreeM b Tip = False" |
    "searchtreeM b (Node left x right) = (if (searchtreeM b left) =True then True
                                       else if b=x then True
                                       else if (searchtreeM b right) =True then True
                                       else False) "
fun searchtreeL::" ('b::ord)  \<Rightarrow> 'b tree  \<Rightarrow> bool" where
    "searchtreeL b Tip = False" |
    "searchtreeL b (Node left x right) = (if (searchtreeL b left)=True then True
                                       else if (searchtreeL b right)=True then True
                                       else if b=x then True
                                       else False)" 

fun mergetree::" 'b tree  \<Rightarrow> 'b tree" where
    "mergetree Tip = Tip" |
    "mergetree (Node left x right) = (Node (mergetree right) x (mergetree left))"

fun looktreeF::" 'b tree  \<Rightarrow> 'b list" where
    "looktreeF Tip = Nil" |
    "looktreeF (Node left x right) = (applist (Cons x (looktreeF left)) 
                                              (looktreeF right)) "
fun looktreeM::" 'b tree  \<Rightarrow> 'b list" where
    "looktreeM Tip = Nil" |
    "looktreeM (Node left x right) = (applist (addlistE x (looktreeM left))
                                             (looktreeM right))"
fun looktreeL::" 'b tree  \<Rightarrow> 'b list" where 
    "looktreeL Tip = Nil" |
    "looktreeL (Node left x right) = (addlistE x 
                                              (applist (looktreeL left) 
                                                       (looktreeL right)) )"

fun Merge::" ('a::ord) list  \<Rightarrow> 'a list  \<Rightarrow> 'a list" where
    "Merge [] xs = xs" |
    "Merge xs [] = xs" |
    "Merge (x#xs)(y#ys) = (if x<y then x#(Merge xs (y#ys))
                          else y#(Merge (x#xs)ys))"
fun MergeSort::" ('a::ord) list  \<Rightarrow> 'a list" where
    "MergeSort [] = []" |
    "MergeSort (Cons a []) = [a]" |
    "MergeSort xs = Merge(MergeSort(take((size xs)div 2)xs))
                         (MergeSort(drop((size xs)div 2)xs))"

fun transtolist::" 'a tree  \<Rightarrow> 'a list" where
    "transtolist Tip = Nil" |
    "transtolist (Node left x right) = x #(transtolist left)
                                        @(transtolist right)"

end