theory more_argu
  imports Main
begin

datatype abool = aTrue | aFalse
datatype anat = aZero | aSuc anat
datatype 'a aoption = aNone | aSome 'a
datatype 'a nat_blist = bNil | bCons 'a "'a nat_blist"
datatype 'a nor_ctree = cnLeaf | cnNode "'a nor_ctree" 'a "'a nor_ctree"

datatype ('a, 'b) boption = bNone | bSome1 'a | bSome2 'b
datatype ('a, 'b) coption = cNone | cSome1 'b | cSome2 'a
datatype ('a, 'b) doption = dNone | dSome1 nat | dSome2 'a
datatype ('a, 'b) eoption = eNone | eSome1 'b
datatype ('a, 'b) foption = fNone | fSome1 'a
datatype ('a, 'b) goption = gNone 'a | gSome 'b
datatype ('a, 'b) hoption = hNone | hSome1 int | hSome2 'b
datatype ('a, 'b) ioption = iNone nat | iSome1 'a | iSome2
datatype ('a, 'b) joption = jNone | jSome1 'a 'b 
datatype ('a, 'b) koption = kNone | kSome1 nat 'a
datatype ('a, 'b) loption = lNone | lSome1 'b 'a
datatype ('a, 'b) moption = mNone | mSome1 'a nat
datatype ('a, 'b) noption = nNone | nSome1 int nat
datatype ('a, 'b) ooption = oNone | oSome1 'a int | oSome2 'b nat
datatype ('a, 'b) poption = pNone | pSome1 'b int | pSome2 'a nat

datatype ('a, 'b) nor_alist = anNil | anCons1 'a | anCons2 'b
datatype ('a, 'b) nor_blist = bnNil | bnCons1 'b | bnCons2 'a
datatype ('a, 'b) nor_clist = cnNil | cnCons1 'a 'b | cnCons2 'a "('a,'b) nor_clist"
datatype ('a, 'b) nor_dlist = dnNil | dnCons1 'a 'b | dnCons2 'b "('a,'b) nor_dlist"
datatype ('a, 'b) nor_elist = enNil | enCons1 'b 'a | enCons2 'a "('a,'b) nor_elist"
datatype ('a, 'b) nor_flist = fnNil | fnCons1 'b 'a | fnCons2 'b "('a,'b) nor_flist"
datatype ('a, 'b) nor_glist = gnNil | gnCons1 'a 'b | gnCons2 'a
datatype ('a, 'b) nor_hlist = hnNil | hnCons1 'a 'b | hnCons2 'b
datatype ('a, 'b) nor_ilist = inNil | inCons1 'a 'b | inCons2 "('a,'b) nor_ilist"
datatype ('a, 'b) nor_jlist = jnNil | jnCons1 'a | jnCons2 'a "('a,'b) nor_jlist"
datatype ('a, 'b) nor_klist = knNil | knCons1 'b | knCons2 'a "('a,'b) nor_klist"
datatype ('a, 'b) nor_llist = lnNil | lnCons1 'b | lnCons2 'b "('a,'b) nor_llist"
datatype ('a, 'b) nor_mlist = mnNil | mnCons1 'a | mnCons2 "('a,'b) nor_mlist"
datatype ('a, 'b) nor_nlist = nnNil | nnCons1 'a | nnCons2 "('a,'b) nor_clist"
datatype ('a, 'b) nor_olist = onNil | onCons1 nat | onCons2 'a "('a,'b) nor_olist"
datatype ('c, 'd) nor_plist = pnNil | pnCons1 'c 'd | pnCons2 'c "('c,'d) nor_plist"
datatype ('c, 'd) nor_qlist = qnNil | qnCons1 'c 'd | qnCons2 'd "('c,'d) nor_qlist"
datatype ('a, 'c) nor_rlist = rnNil | rnCons1 'a 'c | rnCons2 'c "('a,'c) nor_rlist"
datatype ('a, 'b) nor_slist = snNil | snCons1 'a | snCons2 'b "'a list"
datatype ('a, 'b) nor_tlist = tnNil | tnCons1 'b | tnCons2 'a "'b list"
datatype ('a, 'b) nor_ulist = unNil | unCons1 int | unCons2 'a "'a list"
datatype ('a, 'b) nor_vlist = vnNil | vnCons1 int | vnCons2 'b "'b list"
datatype ('a, 'b) nor_wlist = wnNil | wnCons1 'a | wnCons2 int "'b list"
datatype ('a, 'b) nor_xlist = xnNil | xnCons1 'a | xnCons2 'b "int list"
datatype ('a, 'b) nor_ylist = ynNil | ynCons1 'a | ynCons2 'a "int list"
datatype ('a, 'b) nor_zlist = znNil | znCons1 'b | znCons2 'a "int list"




end