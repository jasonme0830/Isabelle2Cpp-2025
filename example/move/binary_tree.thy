theory binary_tree
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun searchtree1::"'a \<Rightarrow> 'a tree \<Rightarrow> bool " where
"searchtree1 a Tip=False "|
"searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right)) "

fun searchtree2::"'a \<Rightarrow> 'a tree \<Rightarrow> bool " where
"searchtree2 a Tip=False "|
"searchtree2 a (Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right)) "

fun searchtree3::"'a \<Rightarrow> 'a tree \<Rightarrow> bool " where
"searchtree3 a Tip=False "|
"searchtree3 a (Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) ) "

fun inserttree::"'a::ord \<Rightarrow>  'a tree  \<Rightarrow>  'a tree " where
"inserttree a Tip= Node Tip a Tip"|
"inserttree a (Node left x right) = (if a \<le> x then inserttree a left else inserttree a right ) "

fun transtolist:: "'a tree  \<Rightarrow> 'a list " where
"transtolist Tip=[] "|
(*"transtolist (Node Tip a Tip) = [a]  "|*)
"transtolist (Node left a right) =( a # (transtolist left)@(transtolist right)) "

fun rightest::"'a tree \<Rightarrow> 'a" where
"rightest (Node left x right) = (if right=Tip then x else (rightest right)) "
(*"rightest Tip=  "*)

fun rightestleft::"'a tree \<Rightarrow>  'a tree "where
"rightestleft Tip=Tip"|
"rightestleft (Node left x right) =(if right=Tip then left else (Node left x (rightestleft right)))  "

fun deltreeroot::"'a tree \<Rightarrow> 'a tree"where
"deltreeroot Tip=Tip "|
"deltreeroot (Node left x right) =(if right=Tip then left else if left=Tip then right else(Node (rightestleft left)(rightest left)right ) ) "

fun deltree::"'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a tree"where
"deltree a Tip = Tip"|
"deltree a (Node left x right) =( if a=x then(deltreeroot(Node left x right))else if a<x then (Node (deltree a left )x right) else (Node left x (deltree a right)) ) "

fun sorttree::"'a tree  \<Rightarrow> ('a::ord) list "where
"sorttree Tip = [] "|
"sorttree (Node Tip a Tip) =[a] "|
"sorttree (Node left x right) =(QuickSort (transtolist (Node left x right)))"


end