theory binary_tree
  imports Main
begin

datatype snat = sZero | sSuc snat
datatype 'a slist = sNil | sCons 'a "'a slist"

fun Modify1::"'a=>'a=> 'a slist =>'a slist"where
"Modify1 a b sNil = sNil "|
"Modify1 a b (sCons x xs) = (if x=a then (sCons b (Modify1 a b xs)) else (sCons x (Modify1 a b xs))) "


end