(* --- set - 20 --- *)
(*1*)
definition coset :: "'a list \<Rightarrow> 'a set" where
[simp]: "coset xs = - set xs"

(*2*)
fun sorted_wrt :: "('a \<Rightarrow> 'a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> bool" where
"sorted_wrt P [] = True" |
"sorted_wrt P (x # ys) = ((\<forall>y \<in> set ys. P x y) \<and> sorted_wrt P ys)"

(*3*)
fun sorted :: "'a list \<Rightarrow> bool" where
"sorted [] = True" |
"sorted (x # ys) = ((\<forall>y \<in> set ys. x \<le> y) \<and> sorted ys)"

(*4*)
fun strict_sorted :: "'a list \<Rightarrow> bool" where
"strict_sorted [] = True" |
"strict_sorted (x # ys) = ((\<forall>y \<in> List.set ys. x < y) \<and> strict_sorted ys)"

(*5*)
definition stable_sort_key :: "(('b \<Rightarrow> 'a) \<Rightarrow> 'b list \<Rightarrow> 'b list) \<Rightarrow> bool" where
"stable_sort_key sk =
   (\<forall>f xs k. filter (\<lambda>y. f y = k) (sk f xs) = filter (\<lambda>y. f y = k) xs)"

(*6*)
definition lex :: "('a \<times> 'a) set \<Rightarrow> ('a list \<times> 'a list) set" where
"lex r = (\<Union>n. lexn r n)"

(*7*)
definition lenlex :: "('a \<times> 'a) set => ('a list \<times> 'a list) set" where
"lenlex r = inv_image (less_than <*lex*> lex r) (\<lambda>xs. (length xs, xs))"

(*8*)
definition lexord :: "('a \<times> 'a) set \<Rightarrow> ('a list \<times> 'a list) set" where
"lexord r = {(x,y). \<exists> a v. y = x @ a # v \<or>
            (\<exists> u a b v w. (a,b) \<in> r \<and> x = u @ (a # v) \<and> y = u @ (b # w))}"

(*9*)
definition listrel1 :: "('a \<times> 'a) set \<Rightarrow> ('a list \<times> 'a list) set" where
"listrel1 r = {(xs,ys).
   \<exists>us z z' vs. xs = us @ z # vs \<and> (z,z') \<in> r \<and> ys = us @ z' # vs}"

(*10*)
definition list_ex1 :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> bool" where
list_ex1_iff [code_abbrev]: "list_ex1 P xs \<longleftrightarrow> (\<exists>! x. x \<in> set xs \<and> P x)"

(*11*)
definition can_select :: "('a \<Rightarrow> bool) \<Rightarrow> 'a set \<Rightarrow> bool" where
[code_abbrev]: "can_select P A = (\<exists>!x\<in>A. P x)"

(*12*)
definition listrel1p :: "('a \<Rightarrow> 'a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a list \<Rightarrow> bool" where
"listrel1p r xs ys = ((xs, ys) \<in> listrel1 {(x, y). r x y})"

(*13*)
definition
  lexordp :: "('a \<Rightarrow> 'a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a list \<Rightarrow> bool" where
  "lexordp r xs ys = ((xs, ys) \<in> lexord {(x, y). r x y})"

(*--14*)
definition "Bleast S P = (LEAST x. x \<in> S \<and> P x)"

(*--15*)
definition "abort_Bleast S P = (LEAST x. x \<in> S \<and> P x)"

(*16*)
definition set_Cons :: "'a set \<Rightarrow> 'a list set \<Rightarrow> 'a list set" where
"set_Cons A XS = {z. \<exists>x xs. z = x # xs \<and> x \<in> A \<and> xs \<in> XS}"

(*17*)
primrec \<comment> \<open>The lexicographic ordering for lists of the specified length\<close>
  lexn :: "('a \<times> 'a) set \<Rightarrow> nat \<Rightarrow> ('a list \<times> 'a list) set" where
"lexn r 0 = {}" |
"lexn r (Suc n) =
  (map_prod (%(x, xs). x#xs) (%(x, xs). x#xs) ` (r <*lex*> lexn r n)) Int
  {(xs, ys). length xs = Suc n \<and> length ys = Suc n}"

(*18*)
definition all_interval_nat :: "(nat \<Rightarrow> bool) \<Rightarrow> nat \<Rightarrow> nat \<Rightarrow> bool" where
  "all_interval_nat P i j \<longleftrightarrow> (\<forall>n \<in> {i..<j}. P n)"

(*19*)
definition all_interval_int :: "(int \<Rightarrow> bool) \<Rightarrow> int \<Rightarrow> int \<Rightarrow> bool" where
  "all_interval_int P i j \<longleftrightarrow> (\<forall>k \<in> {i..j}. P k)"

(*20*)
definition map_project :: "('a \<Rightarrow> 'b option) \<Rightarrow> 'a set \<Rightarrow> 'b set" where
  "map_project f A = {b. \<exists> a \<in> A. f a = Some b}"

(* --- functional && lambda without typeinfo--- *)
(* 可以做 *)
(*21*)
definition rotate :: "nat \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"rotate n = rotate1 ^^ n"

(*22*)
primrec fold :: "('a \<Rightarrow> 'b \<Rightarrow> 'b) \<Rightarrow> 'a list \<Rightarrow> 'b \<Rightarrow> 'b" where
fold_Nil:  "fold f [] = id" |
fold_Cons: "fold f (x # xs) = fold f xs \<circ> f x"

(*23*)
primrec foldr :: "('a \<Rightarrow> 'b \<Rightarrow> 'b) \<Rightarrow> 'a list \<Rightarrow> 'b \<Rightarrow> 'b" where
foldr_Nil:  "foldr f [] = id" |
foldr_Cons: "foldr f (x # xs) = f x \<circ> foldr f xs"

(*24*)
definition sort_key :: "('b \<Rightarrow> 'a) \<Rightarrow> 'b list \<Rightarrow> 'b list" where
"sort_key f xs = foldr (insort_key f) xs []"

(*25*)
primrec subseqs :: "'a list \<Rightarrow> 'a list list" where
"subseqs [] = [[]]" |
"subseqs (x#xs) = (let xss = subseqs xs in map (Cons x) xss @ xss)"

(*26*)
primrec those :: "'a option list \<Rightarrow> 'a list option"
where
"those [] = Some []" |
"those (x # xs) = (case x of
  None \<Rightarrow> None
| Some y \<Rightarrow> map_option (Cons y) (those xs))"

(*27*)
primrec product :: "'a list \<Rightarrow> 'b list \<Rightarrow> ('a \<times> 'b) list" where
"product [] _ = []" |
"product (x#xs) ys = map (Pair x) ys @ product xs ys"

(*28*)
definition union :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"union = fold insert"

(*29*)
primrec n_lists :: "nat \<Rightarrow> 'a list \<Rightarrow> 'a list list" where
"n_lists 0 xs = [[]]" |
"n_lists (Suc n) xs = concat (map (\<lambda>ys. map (\<lambda>y. y # ys) xs) (n_lists n xs))"

(*30*)
definition nths :: "'a list => nat set => 'a list" where
"nths xs A = map fst (filter (\<lambda>p. snd p \<in> A) (zip xs [0..<size xs]))"

(*31*)
definition insort_insert_key :: "('b \<Rightarrow> 'a) \<Rightarrow> 'b \<Rightarrow> 'b list \<Rightarrow> 'b list" where
"insort_insert_key f x xs =
  (if f x \<in> f ` set xs then xs else insort_key f x xs)"

(*32*)
definition map_filter :: "('a \<Rightarrow> 'b option) \<Rightarrow> 'a list \<Rightarrow> 'b list" where
  [code_post]: "map_filter f xs = map (the \<circ> f) (filter (\<lambda>x. f x \<noteq> None) xs)"

(*33*)
abbreviation map2 :: "('a \<Rightarrow> 'b \<Rightarrow> 'c) \<Rightarrow> 'a list \<Rightarrow> 'b list \<Rightarrow> 'c list" where
"map2 f xs ys \<equiv> map (\<lambda>(x,y). f x y) (zip xs ys)"

(*34*)
primrec product_lists :: "'a list list \<Rightarrow> 'a list list" where
"product_lists [] = [[]]" |
"product_lists (xs # xss) = concat (map (\<lambda>x. map (Cons x) (product_lists xss)) xs)"

(*35*)
definition
   "extract" :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> ('a list * 'a * 'a list) option"
where "extract P xs =
  (case dropWhile (Not \<circ> P) xs of
     [] \<Rightarrow> None |
     y#ys \<Rightarrow> Some(takeWhile (Not \<circ> P) xs, y, ys))"

(* --- no function typeinfo --- *)
(* 可以做 *)
(*--36*)
function shuffles where
  "shuffles [] ys = {ys}"
| "shuffles xs [] = {xs}"
| "shuffles (x # xs) (y # ys) = (#) x ` shuffles xs (y # ys) \<union> (#) y ` shuffles (x # xs) ys"
  by pat_completeness simp_all
termination by lexicographic_order

(*--37*)
function transpose where
"transpose []             = []" |
"transpose ([]     # xss) = transpose xss" |
"transpose ((x#xs) # xss) =
  (x # [h. (h#t) \<leftarrow> xss]) # transpose (xs # [t. (h#t) \<leftarrow> xss])"
by pat_completeness auto

(* --- folding.F --- *)
(*38*)
definition sorted_list_of_set :: "'a set \<Rightarrow> 'a list" where
  "sorted_list_of_set = folding.F insort []"
