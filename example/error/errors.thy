theory errors
    imports Main
begin

fun ffilter::"('a => bool)  => 'a list => 'a list "where
"ffilter f [] = [] "|
"ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))"

fun QuickSort::"'a list  => ('a::ord) list "where
"QuickSort [] =  []"|
"QuickSort (x#xs) = (QuickSort (filter(\<lambda> y . y\<le>x) xs))@x #(QuickSort (filter(\<lambda> y . y>x)xs)) "

fun modifytree::"'a::ord \<Rightarrow> 'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a tree "where
"modifytree a b Tip =Tip"|
"modifytree a b (Node left x right) =( if a=x then (Node left b right) else if a<x then (Node (modifytree a b left )x right)  else (Node left x (modifytree a b right)) ) "


end