theory List
imports Sledgehammer Code_Numeral Lifting_Set
begin

datatype (set: 'a) list =
    Nil  ("[]")
  | Cons (hd: 'a) (tl: "'a list")  (infixr "#" 65)
for
  map: map
  rel: list_all2
  pred: list_all
where
  "tl [] = []"

primrec (nonexhaustive) last :: "'a list \<Rightarrow> 'a" where
"last (x # xs) = (if xs = [] then x else last xs)"

primrec butlast :: "'a list \<Rightarrow> 'a list" where
"butlast [] = []" |
"butlast (x # xs) = (if xs = [] then [] else x # butlast xs)"

definition coset :: "'a list \<Rightarrow> 'a set" where
[simp]: "coset xs = - set xs"

primrec append :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" (infixr "@" 65) where
append_Nil: "[] @ ys = ys" |
append_Cons: "(x#xs) @ ys = x # xs @ ys"

primrec rev :: "'a list \<Rightarrow> 'a list" where
"rev [] = []" |
"rev (x # xs) = rev xs @ [x]"

primrec filter:: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"filter P [] = []" |
"filter P (x # xs) = (if P x then x # filter P xs else filter P xs)"

primrec fold :: "('a \<Rightarrow> 'b \<Rightarrow> 'b) \<Rightarrow> 'a list \<Rightarrow> 'b \<Rightarrow> 'b" where
fold_Nil:  "fold f [] = id" |
fold_Cons: "fold f (x # xs) = fold f xs \<circ> f x"

primrec foldr :: "('a \<Rightarrow> 'b \<Rightarrow> 'b) \<Rightarrow> 'a list \<Rightarrow> 'b \<Rightarrow> 'b" where
foldr_Nil:  "foldr f [] = id" |
foldr_Cons: "foldr f (x # xs) = f x \<circ> foldr f xs"

primrec foldl :: "('b \<Rightarrow> 'a \<Rightarrow> 'b) \<Rightarrow> 'b \<Rightarrow> 'a list \<Rightarrow> 'b" where
foldl_Nil:  "foldl f a [] = a" |
foldl_Cons: "foldl f a (x # xs) = foldl f (f a x) xs"

primrec concat:: "'a list list \<Rightarrow> 'a list" where
"concat [] = []" |
"concat (x # xs) = x @ concat xs"

primrec drop:: "nat \<Rightarrow> 'a list \<Rightarrow> 'a list" where
drop_Nil: "drop n [] = []" |
drop_Cons: "drop n (x # xs) = (case n of 0 \<Rightarrow> x # xs | Suc m \<Rightarrow> drop m xs)"

primrec take:: "nat \<Rightarrow> 'a list \<Rightarrow> 'a list" where
take_Nil:"take n [] = []" |
take_Cons: "take n (x # xs) = (case n of 0 \<Rightarrow> [] | Suc m \<Rightarrow> x # take m xs)"

primrec (nonexhaustive) nth :: "'a list => nat => 'a" (infixl "!" 100) where
nth_Cons: "(x # xs) ! n = (case n of 0 \<Rightarrow> x | Suc k \<Rightarrow> xs ! k)"

primrec list_update :: "'a list \<Rightarrow> nat \<Rightarrow> 'a \<Rightarrow> 'a list" where
"list_update [] i v = []" |
"list_update (x # xs) i v =
  (case i of 0 \<Rightarrow> v # xs | Suc j \<Rightarrow> x # list_update xs j v)"

primrec takeWhile :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"takeWhile P [] = []" |
"takeWhile P (x # xs) = (if P x then x # takeWhile P xs else [])"

primrec dropWhile :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"dropWhile P [] = []" |
"dropWhile P (x # xs) = (if P x then dropWhile P xs else x # xs)"

primrec zip :: "'a list \<Rightarrow> 'b list \<Rightarrow> ('a \<times> 'b) list" where
"zip xs [] = []" |
zip_Cons: "zip xs (y # ys) =
  (case xs of [] \<Rightarrow> [] | z # zs \<Rightarrow> (z, y) # zip zs ys)"

abbreviation map2 :: "('a \<Rightarrow> 'b \<Rightarrow> 'c) \<Rightarrow> 'a list \<Rightarrow> 'b list \<Rightarrow> 'c list" where
"map2 f xs ys \<equiv> map (\<lambda>(x,y). f x y) (zip xs ys)"

primrec product :: "'a list \<Rightarrow> 'b list \<Rightarrow> ('a \<times> 'b) list" where
"product [] _ = []" |
"product (x#xs) ys = map (Pair x) ys @ product xs ys"

primrec product_lists :: "'a list list \<Rightarrow> 'a list list" where
"product_lists [] = [[]]" |
"product_lists (xs # xss) = concat (map (\<lambda>x. map (Cons x) (product_lists xss)) xs)"

primrec upt :: "nat \<Rightarrow> nat \<Rightarrow> nat list" ("(1[_..</_'])") where
upt_0: "[i..<0] = []" |
upt_Suc: "[i..<(Suc j)] = (if i \<le> j then [i..<j] @ [j] else [])"

definition insert :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"insert x xs = (if x \<in> set xs then xs else x # xs)"

definition union :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"union = fold insert"

primrec find :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a option" where
"find _ [] = None" |
"find P (x#xs) = (if P x then Some x else find P xs)"

definition
   "extract" :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> ('a list * 'a * 'a list) option"
where "extract P xs =
  (case dropWhile (Not \<circ> P) xs of
     [] \<Rightarrow> None |
     y#ys \<Rightarrow> Some(takeWhile (Not \<circ> P) xs, y, ys))"

primrec those :: "'a option list \<Rightarrow> 'a list option"
where
"those [] = Some []" |
"those (x # xs) = (case x of
  None \<Rightarrow> None
| Some y \<Rightarrow> map_option (Cons y) (those xs))"

primrec remove1 :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"remove1 x [] = []" |
"remove1 x (y # xs) = (if x = y then xs else y # remove1 x xs)"

primrec removeAll :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"removeAll x [] = []" |
"removeAll x (y # xs) = (if x = y then removeAll x xs else y # removeAll x xs)"

primrec distinct :: "'a list \<Rightarrow> bool" where
"distinct [] \<longleftrightarrow> True" |
"distinct (x # xs) \<longleftrightarrow> x \<notin> set xs \<and> distinct xs"

primrec remdups :: "'a list \<Rightarrow> 'a list" where
"remdups [] = []" |
"remdups (x # xs) = (if x \<in> set xs then remdups xs else x # remdups xs)"

fun remdups_adj :: "'a list \<Rightarrow> 'a list" where
"remdups_adj [] = []" |
"remdups_adj [x] = [x]" |
"remdups_adj (x # y # xs) = (if x = y then remdups_adj (x # xs) else x # remdups_adj (y # xs))"

primrec replicate :: "nat \<Rightarrow> 'a \<Rightarrow> 'a list" where
replicate_0: "replicate 0 x = []" |
replicate_Suc: "replicate (Suc n) x = x # replicate n x"

abbreviation length :: "'a list \<Rightarrow> nat" where
"length \<equiv> size"

definition enumerate :: "nat \<Rightarrow> 'a list \<Rightarrow> (nat \<times> 'a) list" where
enumerate_eq_zip: "enumerate n xs = zip [n..<n + length xs] xs"

primrec rotate1 :: "'a list \<Rightarrow> 'a list" where
"rotate1 [] = []" |
"rotate1 (x # xs) = xs @ [x]"

definition rotate :: "nat \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"rotate n = rotate1 ^^ n"

definition nths :: "'a list => nat set => 'a list" where
"nths xs A = map fst (filter (\<lambda>p. snd p \<in> A) (zip xs [0..<size xs]))"

primrec subseqs :: "'a list \<Rightarrow> 'a list list" where
"subseqs [] = [[]]" |
"subseqs (x#xs) = (let xss = subseqs xs in map (Cons x) xss @ xss)"

primrec n_lists :: "nat \<Rightarrow> 'a list \<Rightarrow> 'a list list" where
"n_lists 0 xs = [[]]" |
"n_lists (Suc n) xs = concat (map (\<lambda>ys. map (\<lambda>y. y # ys) xs) (n_lists n xs))"

function splice :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
"splice [] ys = ys" |
"splice (x#xs) ys = x # splice ys xs"
by pat_completeness auto

function shuffles where
  "shuffles [] ys = {ys}"
| "shuffles xs [] = {xs}"
| "shuffles (x # xs) (y # ys) = (#) x ` shuffles xs (y # ys) \<union> (#) y ` shuffles (x # xs) ys"

fun sorted_wrt :: "('a \<Rightarrow> 'a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> bool" where
"sorted_wrt P [] = True" |
"sorted_wrt P (x # ys) = ((\<forall>y \<in> set ys. P x y) \<and> sorted_wrt P ys)"

fun sorted :: "'a list \<Rightarrow> bool" where
"sorted [] = True" |
"sorted (x # ys) = ((\<forall>y \<in> set ys. x \<le> y) \<and> sorted ys)"

fun strict_sorted :: "'a list \<Rightarrow> bool" where
"strict_sorted [] = True" |
"strict_sorted (x # ys) = ((\<forall>y \<in> List.set ys. x < y) \<and> strict_sorted ys)"

primrec insort_key :: "('b \<Rightarrow> 'a) \<Rightarrow> 'b \<Rightarrow> 'b list \<Rightarrow> 'b list" where
"insort_key f x [] = [x]" |
"insort_key f x (y#ys) =
  (if f x \<le> f y then (x#y#ys) else y#(insort_key f x ys))"

definition sort_key :: "('b \<Rightarrow> 'a) \<Rightarrow> 'b list \<Rightarrow> 'b list" where
"sort_key f xs = foldr (insort_key f) xs []"

definition insort_insert_key :: "('b \<Rightarrow> 'a) \<Rightarrow> 'b \<Rightarrow> 'b list \<Rightarrow> 'b list" where
"insort_insert_key f x xs =
  (if f x \<in> f ` set xs then xs else insort_key f x xs)"

abbreviation "sort \<equiv> sort_key (\<lambda>x. x)"
abbreviation "insort \<equiv> insort_key (\<lambda>x. x)"
abbreviation "insort_insert \<equiv> insort_insert_key (\<lambda>x. x)"

definition stable_sort_key :: "(('b \<Rightarrow> 'a) \<Rightarrow> 'b list \<Rightarrow> 'b list) \<Rightarrow> bool" where
"stable_sort_key sk =
   (\<forall>f xs k. filter (\<lambda>y. f y = k) (sk f xs) = filter (\<lambda>y. f y = k) xs)"

end

primrec partition :: "('a \<Rightarrow> bool) \<Rightarrow>'a list \<Rightarrow> 'a list \<times> 'a list" where
  "partition P [] = ([], [])" |
  "partition P (x # xs) =
  (let (yes, no) = partition P xs
   in if P x then (x # yes, no) else (yes, x # no))"

function upto :: "int \<Rightarrow> int \<Rightarrow> int list" ("(1[_../_])") where
  "upto i j = (if i \<le> j then i # [i+1..j] else [])"

definition upto_aux :: "int \<Rightarrow> int \<Rightarrow> int list \<Rightarrow> int list" where
  "upto_aux i j js = [i..j] @ js"

function transpose where
"transpose []             = []" |
"transpose ([]     # xss) = transpose xss" |
"transpose ((x#xs) # xss) =
  (x # [h. (h#t) \<leftarrow> xss]) # transpose (xs # [t. (h#t) \<leftarrow> xss])"

definition sorted_list_of_set :: "'a set \<Rightarrow> 'a list" where
  "sorted_list_of_set = folding.F insort []"

inductive_set
  lists :: "'a set => 'a list set"
  for A :: "'a set"
where
    Nil [intro!, simp]: "[] \<in> lists A"
  | Cons [intro!, simp]: "\<lbrakk>a \<in> A; l \<in> lists A\<rbrakk> \<Longrightarrow> a#l \<in> lists A"

inductive_cases listsE [elim!]: "x#l \<in> lists A"
inductive_cases listspE [elim!]: "listsp A (x # l)"

inductive_simps listsp_simps[code]:
  "listsp A []"
  "listsp A (x # xs)"

inductive ListMem :: "'a \<Rightarrow> 'a list \<Rightarrow> bool"
where
    elem:  "ListMem x (x # xs)"
  | insert:  "ListMem x xs \<Longrightarrow> ListMem x (y # xs)"

definition set_Cons :: "'a set \<Rightarrow> 'a list set \<Rightarrow> 'a list set" where
"set_Cons A XS = {z. \<exists>x xs. z = x # xs \<and> x \<in> A \<and> xs \<in> XS}"

primrec listset :: "'a set list \<Rightarrow> 'a list set" where
"listset [] = {[]}" |
"listset (A # As) = set_Cons A (listset As)"

primrec
definition lex :: "('a \<times> 'a) set \<Rightarrow> ('a list \<times> 'a list) set" where
"lex r = (\<Union>n. lexn r n)" \<comment> \<open>Holds only between lists of the same length\<close>

definition lenlex :: "('a \<times> 'a) set => ('a list \<times> 'a list) set" where
"lenlex r = inv_image (less_than <*lex*> lex r) (\<lambda>xs. (length xs, xs))"
        \<comment> \<open>Compares lists by their length and then lexicographically\<close>

definition lexord :: "('a \<times> 'a) set \<Rightarrow> ('a list \<times> 'a list) set" where
"lexord r = {(x,y). \<exists> a v. y = x @ a # v \<or>
            (\<exists> u a b v w. (a,b) \<in> r \<and> x = u @ (a # v) \<and> y = u @ (b # w))}"

inductive lexordp :: "'a list \<Rightarrow> 'a list \<Rightarrow> bool"
where
  Nil: "lexordp [] (y # ys)"
| Cons: "x < y \<Longrightarrow> lexordp (x # xs) (y # ys)"
| Cons_eq:
  "\<lbrakk> \<not> x < y; \<not> y < x; lexordp xs ys \<rbrakk> \<Longrightarrow> lexordp (x # xs) (y # ys)"

inductive lexordp_eq :: "'a list \<Rightarrow> 'a list \<Rightarrow> bool" where
  Nil: "lexordp_eq [] ys"
| Cons: "x < y \<Longrightarrow> lexordp_eq (x # xs) (y # ys)"
| Cons_eq: "\<lbrakk> \<not> x < y; \<not> y < x; lexordp_eq xs ys \<rbrakk> \<Longrightarrow> lexordp_eq (x # xs) (y # ys)"

definition "measures fs = inv_image (lex less_than) (%a. map (%f. f a) fs)"

definition listrel1 :: "('a \<times> 'a) set \<Rightarrow> ('a list \<times> 'a list) set" where
"listrel1 r = {(xs,ys).
   \<exists>us z z' vs. xs = us @ z # vs \<and> (z,z') \<in> r \<and> ys = us @ z' # vs}"

inductive_set
  listrel :: "('a \<times> 'b) set \<Rightarrow> ('a list \<times> 'b list) set"
  for r :: "('a \<times> 'b) set"
where
    Nil:  "([],[]) \<in> listrel r"
  | Cons: "[| (x,y) \<in> r; (xs,ys) \<in> listrel r |] ==> (x#xs, y#ys) \<in> listrel r"

inductive_cases listrel_Nil1 [elim!]: "([],xs) \<in> listrel r"
inductive_cases listrel_Nil2 [elim!]: "(xs,[]) \<in> listrel r"
inductive_cases listrel_Cons1 [elim!]: "(y#ys,xs) \<in> listrel r"
inductive_cases listrel_Cons2 [elim!]: "(xs,y#ys) \<in> listrel r"

definition bind :: "'a list \<Rightarrow> ('a \<Rightarrow> 'b list) \<Rightarrow> 'b list" where
"bind xs f = concat (map f xs)"

fun map_tailrec_rev ::  "('a \<Rightarrow> 'b) \<Rightarrow> 'a list \<Rightarrow> 'b list \<Rightarrow> 'b list" where
"map_tailrec_rev f [] bs = bs" |
"map_tailrec_rev f (a#as) bs = map_tailrec_rev f as (f a # bs)"

definition map_tailrec :: "('a \<Rightarrow> 'b) \<Rightarrow> 'a list \<Rightarrow> 'b list" where
"map_tailrec f as = rev (map_tailrec_rev f as [])"

definition member :: "'a list \<Rightarrow> 'a \<Rightarrow> bool" where
[code_abbrev]: "member xs x \<longleftrightarrow> x \<in> set xs"

definition list_ex :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> bool" where
list_ex_iff [code_abbrev]: "list_ex P xs \<longleftrightarrow> Bex (set xs) P"

definition list_ex1 :: "('a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> bool" where
list_ex1_iff [code_abbrev]: "list_ex1 P xs \<longleftrightarrow> (\<exists>! x. x \<in> set xs \<and> P x)"

definition can_select :: "('a \<Rightarrow> bool) \<Rightarrow> 'a set \<Rightarrow> bool" where
[code_abbrev]: "can_select P A = (\<exists>!x\<in>A. P x)"

definition listrel1p :: "('a \<Rightarrow> 'a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a list \<Rightarrow> bool" where
"listrel1p r xs ys = ((xs, ys) \<in> listrel1 {(x, y). r x y})"

definition
  lexordp :: "('a \<Rightarrow> 'a \<Rightarrow> bool) \<Rightarrow> 'a list \<Rightarrow> 'a list \<Rightarrow> bool" where
  "lexordp r xs ys = ((xs, ys) \<in> lexord {(x, y). r x y})"

definition "Bleast S P = (LEAST x. x \<in> S \<and> P x)"

definition "abort_Bleast S P = (LEAST x. x \<in> S \<and> P x)"

definition null :: "'a list \<Rightarrow> bool" where
  [code_abbrev]: "null xs \<longleftrightarrow> xs = []"

definition maps :: "('a \<Rightarrow> 'b list) \<Rightarrow> 'a list \<Rightarrow> 'b list" where
  [code_abbrev]: "maps f xs = concat (map f xs)"

definition map_filter :: "('a \<Rightarrow> 'b option) \<Rightarrow> 'a list \<Rightarrow> 'b list" where
  [code_post]: "map_filter f xs = map (the \<circ> f) (filter (\<lambda>x. f x \<noteq> None) xs)"

definition all_interval_nat :: "(nat \<Rightarrow> bool) \<Rightarrow> nat \<Rightarrow> nat \<Rightarrow> bool" where
  "all_interval_nat P i j \<longleftrightarrow> (\<forall>n \<in> {i..<j}. P n)"

definition all_interval_int :: "(int \<Rightarrow> bool) \<Rightarrow> int \<Rightarrow> int \<Rightarrow> bool" where
  "all_interval_int P i j \<longleftrightarrow> (\<forall>k \<in> {i..j}. P k)"

definition gen_length :: "nat \<Rightarrow> 'a list \<Rightarrow> nat"
where "gen_length n xs = n + length xs"

definition map_project :: "('a \<Rightarrow> 'b option) \<Rightarrow> 'a set \<Rightarrow> 'b set" where
  "map_project f A = {b. \<exists> a \<in> A. f a = Some b}"

end
