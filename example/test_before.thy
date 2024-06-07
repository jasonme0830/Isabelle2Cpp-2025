theory binary_tree
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 


fun transtolist:: "'a tree =>'a list " where
"transtolist Tip=[] "|
(*"transtolist (Node Tip a Tip) = [a]  "|*)
"transtolist (Node left a right) =( a # (transtolist left)@(transtolist right)) "

end