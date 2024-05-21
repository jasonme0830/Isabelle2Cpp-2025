theory binary_tree
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun searchtree1::"'a=>'a tree=>bool " where
"searchtree1 a Tip=False "|
"searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right)) "

fun searchtree2::"'a=>'a tree=>bool " where
"searchtree2 a Tip=False "|
"searchtree2 a (Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right)) "

fun searchtree3::"'a=>'a tree=>bool " where
"searchtree3 a Tip=False "|
"searchtree3 a (Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) ) "

fun inserttree::"'a::ord=> 'a tree => 'a tree " where
"inserttree a (Node left x right) = (if a\<le>x then (Node (inserttree a left) x right) 
                                     else (Node left x (inserttree a right) ) )" |
"inserttree a Tip= Node Tip a Tip"

fun transtolist:: "'a tree =>'a list " where
"transtolist Tip=[] "|
(*"transtolist (Node Tip a Tip) = [a]  "|*)
"transtolist (Node left a right) =( a # (transtolist left)@(transtolist right)) "

fun rightest::"'a tree=>'a" where
"rightest (Node left x right) = (if right=Tip then x 
                                 else (rightest right)) "
(*"rightest Tip=  "*)

fun rightestleft::"'a tree=> 'a tree "where
"rightestleft Tip=Tip"|
"rightestleft (Node left x right) =(if right=Tip then left 
                                    else ( (rightestleft right)))  "

fun deltreeroot::"'a tree=>'a tree"where
"deltreeroot Tip=Tip "|
"deltreeroot (Node left x right) =(if right=Tip then left else if left=Tip then right else(Node (rightestleft left)(rightest left)right ) ) "

fun deltree::"'a::ord=>'a tree=>'a tree"where
"deltree a Tip = Tip"|
"deltree a (Node left x right) =( if a=x then(deltreeroot(Node left x right))
                                  else if a<x then (Node (deltree a left )x right) 
                                  else (Node left x (deltree a right)) ) "

fun changetree::"('a::ord) => 'a::ord => 'a tree => 'a tree " where
"changetree a b Tip = Tip" |
"changetree a b (Node left x right) = ( if (a=x) then (Node left b right)
                                        else if (a<x) then (Node (changetree a b left) x right)
                                        else (Node left x (changetree a b right)))"

fun Merge::"('a::ord)list => 'a list => 'a list "where
"Merge [] xs=xs"|
"Merge xs [] = xs"|
"Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys)) "

fun MergeSort::"('a::ord)list=>'a list " where
"MergeSort [] = []"|
"MergeSort (Cons a []) = [a] "|
"MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs)) "

fun sorttree::"'a tree =>('a::ord) list "where
"sorttree Tip = [] "|
"sorttree (Node Tip a Tip) =[a] "|
"sorttree (Node left x right) =(MergeSort (transtolist (Node left x right)))"


end