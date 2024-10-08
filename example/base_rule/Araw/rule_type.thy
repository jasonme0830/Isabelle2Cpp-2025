theory rule_tree
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 


fun supto :: "int => int => int list" where  
  "supto i j = (if i \<ge> j then [] else i # supto (i + 1) j)"


end