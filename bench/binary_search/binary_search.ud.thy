theory binary_search
imports Main
begin

fun bs_ud :: "nat \<Rightarrow> nat list \<Rightarrow> nat option" where
  "bs_ud x [] = None" |
  "bs_ud x [y] = If (x = y) (Some 0) None" |
  "bs_ud x ys = (let m = (length ys) div 2 in
      let y = ys ! m in
        If (y = x)
          (Some m)
          (If (y < x)
            (case bs_ud x (drop (m + 1) ys) of Some n \<Rightarrow> Some (m + n + 1) | None \<Rightarrow> None)
            (bs_ud x (take m ys)
          )
      )
  )"

end
