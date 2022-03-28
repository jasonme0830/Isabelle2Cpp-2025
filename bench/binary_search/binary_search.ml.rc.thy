theory binary_search_ml_rc
imports Main
begin

fun bs_ml_rc :: "nat \<Rightarrow> nat list \<Rightarrow> nat option" where
  "bs_ml_rc x [] = None" |
  "bs_ml_rc x [y] = If (x = y) (Some 0) None" |
  "bs_ml_rc x ys = (let m = (length ys) div 2 in
      let y = ys ! m in
        If (y = x)
          (Some m)
          (If (y < x)
            (case bs_ml_rc x (drop (m + 1) ys) of Some n \<Rightarrow> Some (m + n + 1) | None \<Rightarrow> None)
            (bs_ml_rc x (take m ys)
          )
      )
  )"

end
