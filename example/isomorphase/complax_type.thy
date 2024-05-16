theory complax_type
  imports Main
begin

datatype abool = aTrue | aFalse
datatype anat = aZero | aSuc anat
datatype 'a option = None | Some 'a
datatype 'a blist = bNil | bCons 'a "'a blist"
datatype ('a, 'b) pair = Pair 'a 'b
datatype 'a atree = aLeaf | aNode "'a atree" 'a "'a atree"

datatype 'b btree = bLeaf | bNode "'b atree" 'b "'b atree"
datatype 'a ctree = cLeaf | cNone "'a ctree" 'a "'a ctree"
datatype 'b dtree = dLeaf | dNode "'b ctree" 'b "'b ctree"

datatype even_nat = Even_Zero | Even_Succ odd_nat
and odd_nat = Odd_Succ even_nat

datatype ('a, 'b) exp =
Term "('a, 'b) trm" | Sum "('a, 'b) trm" "('a, 'b) exp"
and ('a, 'b) trm =
Factor "('a, 'b) fct" | Prod "('a, 'b) fct" "('a, 'b) trm"
and ('a, 'b) fct =
Const 'a | Var 'b | Expr "('a, 'b) exp"


datatype ('a,'b) clist = cNil | cCons 'a "('a,'b) clist"
datatype ('a,'b) dlist = dNil | dCons 'b "('a,'b) dlist"
(*datatype ('a,'b) elist = eNil | eCons 'a "('b,'a) elist"*)
datatype ('a,'b) flist = fNil | fCons 'a "('b,'a) clist"
datatype ('a,'b) glist = gNil | gCons 'a "('a,'b) clist"
datatype ('a,'b) hlist = hNil | hCons 'b "('b,'a) clist"
datatype ('a,'b) ilist = iNil | iCons 'a "('a blist,'b) clist"
(*datatype ('a,'b) jlist = jNil | jCons 'a "('a blist,'b) jlist"*)
datatype ('a,'b) klist = kNil | kCons 'a "(('a,'b)klist,'a) clist"


datatype 'a etree = eLeaf | eNode 'a "'a etree option" "'a etree option"

datatype ('a, 'b) fun_copy = Fun "'a \<Rightarrow> 'b"

(*datatype 'a also_wrong = W1 | W2 "(('a also_wrong), 'a) fun_copy"*)

datatype 'a ftree = FTLeaf 'a | FTNode "'a \<Rightarrow> 'a ftree"

(*datatype 'a pow_list = PNil 'a | PCons "('a ∗ 'a) pow_list"*)







end