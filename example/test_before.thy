theory binary_tree
  imports Main
begin

datatype 'a slist = sNil | sCons 'a "'a slist"
datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun AddListHead::"'a =>'a list=>'a list"where
"AddListHead a xs =a # xs "

fun bs :: "nat \<Rightarrow> nat slist \<Rightarrow> nat option" where
  "bs x [] = None" |
  "bs x [y] = If (x = y) (Some Zero) None" |
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


end