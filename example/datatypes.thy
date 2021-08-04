datatype snat = sZero | sSucc snat;
datatype 'a slist = sNil | sCons 'a "'a slist";
datatype sbool = sTrue | sFalse;
datatype option = None | Some 'a;

fun add :: "snat \<Rightarrow> snat \<Rightarrow> snat" where
  "add sZero n = n" |
  "add (sSucc m) n = sSucc (add m n)"

fun fib :: "snat \<Rightarrow> snat" where
  "fib sZero = sSucc sZero" |
  "fib (sSucc sZero) = sSucc sZero" |
  "fib (sSucc (sSucc m)) = add (fib (sSucc m)) (fib m)"

fun natofsnat :: "snat \<Rightarrow> nat" where
  "natofsnat sZero = 0" |
  "natofsnat (sSucc n) = (natofsnat n) + 1"

fun snatofnat :: "nat \<Rightarrow> snat" where
  "snatofnat 0 = sZero" |
  "snatofnat (Suc n) = sSucc (snatofnat n)"

fun app :: "'a slist \<Rightarrow> 'a slist \<Rightarrow> 'a slist" where
  "app sNil ys = ys" |
  "app (sCons x xs) ys = sCons x (app xs ys)"

fun rev :: "'a slist \<Rightarrow> 'a slist" where
  "rev sNil = sNil" |
  "rev (sCons x xs) = app (rev xs) (sCons x sNil)"

fun snat2slist :: "snat \<Rightarrow> snat slist" where
  "snat2slist n = sCons n sNil"

fun len :: "'a slist \<Rightarrow> snat" where
  "len sNil = sZero" |
  "len (sCons x xs) = sSucc (len xs)"

fun listwithlen :: "nat \<Rightarrow> nat slist" where
  "listwithlen 0 = sNil" |
  "listwithlen (Suc n) = sCons 1 (listwithlen n)"

fun leninnat :: "'a slist \<Rightarrow> nat" where
  "leninnat sNil = 0" |
  "leninnat (sCons x xs) = (leninnat xs) + 1"

fun dblist :: "'a slist \<Rightarrow> 'a slist" where
  "dblist sNil = sNil" |
  "dblist (sCons x sNil) = sCons x (sCons x sNil)" |
  "dblist (sCons x xs) = app (dblist (sCons x sNil)) (dblist xs)"

fun snot :: "sbool \<Rightarrow> sbool" where
  "snot sTrue = sFalse" |
  "snot sFlase = sTrue"
