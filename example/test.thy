theory test
  imports Main
begin


fun pair :: "'a \<Rightarrow> 'b \<Rightarrow> ('a * 'b)" where
  "pair f s = (f, s)"

end
