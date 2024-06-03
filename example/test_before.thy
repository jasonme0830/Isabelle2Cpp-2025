theory binary_tree
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun inserttree::"'b list => 'b list tree => 'b list tree " where
"inserttree [] Tip = Tip" |
"inserttree (Cons x xs) Tip = (Node Tip (Cons x xs) Tip)" |
"inserttree [] (Node left a right) = (Node left a right)" |
"inserttree xs (Node left as right) = (if (size xs) < (size as) then (Node(inserttree xs left) as right)
                                      else (Node  left as (inserttree xs right)))"

end