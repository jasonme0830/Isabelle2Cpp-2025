theory binary_tree
  imports Main
begin

datatype 'a slist = sNil | sCons 'a "'a slist"
datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun bs :: "nat \<Rightarrow> nat slist \<Rightarrow> nat option" where
  "bs x [] = None" |
  "bs x [y] = If (x = y) (Some sZero) None" |
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

end