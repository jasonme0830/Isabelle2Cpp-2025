theory rule
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 


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

fun delRightest::"'a tree => 'a tree" where
"delRightest (Node left x Tip) = left" |
"delRightest (Node left x right) = Node left x (delRightest right)" |
"delRightest Tip = Tip"

fun rightestleft::"'a tree => 'a tree "where
"rightestleft Tip = Tip"|
"rightestleft (Node left x right) =(if right=Tip then left 
                                    else ( (rightestleft right)))  "

fun deltreeroot::"'a tree => 'a tree"where
"deltreeroot Tip = Tip "|
"deltreeroot (Node left x right) =(if right=Tip 
                                      then left 
                                      else if left=Tip 
                                              then right 
                                              else(Node (delRightest left)(rightest left)right ) ) "

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