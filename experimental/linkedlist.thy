theory linkedlist
  imports Main
begin

type_alias addr = nat

datatype ref = Null | Ref addr

datatype llist_node = LList "addr \<Rightarrow> nat" "addr \<Rightarrow> ref"
(**
 * nat, llist_node*
*)

fun app :: "((addr set \<Rightarrow> addr) * addr set) \<Rightarrow> llist_node \<Rightarrow> ref \<Rightarrow> ref \<Rightarrow> (llist_node * ref)" where
(**
 * ALLOCATION: 1;
 * TYPED_INFO: (2, 3, 4);
 * REAL_ARGS: 3, 4;
 * RETURN: 5_2;
*)
  "app (new, alloc) ll Null Null = (ll, Null)" |
  "app (new, alloc) ll (Ref a) Null = (ll, Ref a)" |
  "app (new, alloc) ll Null (Ref b) = (ll, Ref b)" |
  "app (new, alloc) (LList val next) (Ref a) (Ref b) = (LList val (next(a:= snd (app (new, alloc) (LList val next) (next a) (Ref b)))), Ref a)"

end
