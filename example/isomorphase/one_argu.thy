theory one_argu
  imports Main
begin

datatype abool = aTrue | aFalse
datatype anat = aZero | aSuc anat

datatype 'a aoption = aNone | aSome 'a
datatype 'a boption = bNone | bOnce | bSome 'a
datatype 'a coption = cNone | cOnce nat | cSome 'a
datatype 'a doption = dNone | dOnce bool | dSome 'a | dAll 'a nat
datatype 'a eoption = eNone | eOnce "'a eoption" | eSome 'a
datatype 'b foption = fNone | fOnce "'b foption" | fSome 'b
datatype 'a goption = gNone | gOnce "'a goption" | gSome nat
datatype 'a hoption = hNone | hOnce "nat boption" | hSome 'a
datatype 'a ioption = iNone | iOnce "'a boption" | iSome 'a


datatype 'a nat_alist = aNil | aCons 'a
datatype 'a nat_blist = bNil | bCons 'a "'a nat_blist"
datatype 'a nat_clist = cNil | cCons nat "'a nat_clist"
datatype 'a nat_dlist = dNil | dCons 'a "'a list"
datatype 'a nat_elist = eNil | eCons nat
datatype 'b nat_flist = fNil | fCons 'b "'b nat_flist"
datatype 'b nat_glist = gNil | gCons 'b "'b list"
datatype 'a nat_hlist = hNil | hCons 'a "nat list"
datatype 'a nat_ilist = iNil | iCons 'a "'a nat_flist"
datatype 'a nat_jlist = jNil 'a "'a nat_jlist" | jCons
datatype 'a nat_klist = bNil | bCons 'a "'a nat_klist"
datatype 'a nat_llist = bNil | bCons 'a "'a nat_blist"
datatype 'a nat_mlist = mNil | mCons 'a 'a
datatype 'a nat_nlist = nNil | nCons "'a nat_nlist" "'a nat_nlist"

datatype 'a two_alist = atNil | atCons1 'a | atCons2 nat
datatype 'a two_blist = btNil | btCons1 'a | btCons2 int
datatype 'a two_clist = ctNil | ctCons1 'a | ctCons2 "'a list"
datatype 'a two_dlist = dtNil | dtCons1 'a | dtCons2 'a "'a list"
datatype 'a two_elist = etNil | etCons1 'a "'a list" | etCons2 "'a list"
datatype 'a two_flist = ftNil | ftCons1 'a | ftCons2 "'a two_flist"
datatype 'a two_glist = gtNil | gtCons1 'a | gtCons2 'a "'a two_glist"
datatype 'a two_hlist = htNil | htCons1 'a "'a two_hlist" | htCons2 "'a two_hlist"
datatype 'b two_ilist = itNil | itCons1 'b | itCons2 'b "'b two_ilist"
datatype 'b two_jlist = jtNil | jtCons1 'b "'b list" | jtCons2 'b
datatype 'a two_klist = ktNil | ktCons1 'a | ktCons2 nat "'a list"
datatype 'a two_llist = ltNil | ltCons1 nat | ltCons2 'a "'a list"
datatype 'a two_mlist = mtNil | mtCons1 'a | mtCons2 'a "nat list"
datatype 'a two_nlist = ntNil | ntCons1 'a | ntCons2 nat "'a two_nlist"
datatype 'a two_olist = otNil | otCons1 nat | otCons2 'a " 'a two_olist"
datatype 'a two_plist = ptNil | ptCons1 int | ptCons2 nat "'a two_plist"
datatype 'a two_qlist = qtNil | qtCons1 nat | qtCons2 int "'a two_qlist"
datatype 'a two_rlist = rtNil 'a | rtCons1 'a "'a two_rlist" | rtCons2

datatype 'a nor_atree = anLeaf | anNode 'a 
datatype 'a nor_btree = bnLeaf | anNode 'a "'a nor_btree"
datatype 'a nor_ctree = cnLeaf | cnNode "'a nor_ctree" 'a "'a nor_ctree"
datatype 'a nor_dtree = dnLeaf | dnNode "'a nor_dtree" 'a
datatype 'a nor_etree = enLeaf | enNode "'a nor_etree" "'a nor_etree" 'a
datatype 'a nor_ftree = fnLeaf | fnNode 'a "'a nor_etree" "'a nor_etree"
datatype 'a nor_gtree = gnLeaf | gnNode 'a 'a "'a nor_gtree"
datatype 'a nor_htree = hnLeaf | hnNode nat "'a nor_htree"
datatype 'a nor_itree = inLeaf | inNode nat 'a "'a nor_itree"
datatype 'a nor_jtree = jnLeaf | jnNode 'a nat "'a nor_jtree"
datatype 'a nor_ktree = knLeaf | knNode "'a nor_ktree" 'a nat "'a nor_ktree"
datatype 'a nor_ltree = lnLeaf | lnNode "'a nor_ctree" 'a "'a nor_ctree"
datatype 'a nor_mtree = mnLeaf | mnNode "'a nor_mtree" 'a "'a list"
datatype 'a nor_ntree = nnLeaf | nnNode "'a list" 'a "'a list"

datatype 'a two_atree = 
      atLeaf |
      atNode1 'a |
      atNode2 'a "'a two_atree" |
      atNode3 "'a two_atree" 'a "'a two_atree"

datatype 'a two_btree =
      btLeaf |
      btNode1 "'a two_btree" 'a |
      btNode2 'a |
      btNode3 "'a two_btree" 'a "'a two_btree"

datatype 'a two_ctree = 
      ctLeaf |
      ctNode1 'a "'a two_ctree" |
      ctNode2 'a |
      ctNode3 "'a two_ctree" 'a "'a two_ctree"

datatype 'a two_dtree = 
      dtLeaf |
      dtNode1 'a "'a list" |
      dtNode2 'a |
      dtNode3 "'a two_dtree" 'a "'a two_dtree"

datatype 'a two_etree = 
      etLeaf |
      etNode1 nat |
      etNode2 'a "'a two_etree" |
      etNode3 "'a two_etree" 'a "'a two_etree"

datatype 'b two_ftree = 
      ftLeaf |
      ftNode1 'b |
      ftNode2 'b "'b two_ftree" |
      ftNode3 "'b two_ftree" 'b "'b two_ftree"

datatype 'a two_gtree = 
      gtLeaf |
      gtNode1 'a |
      gtNode2 'a "'a two_atree" |
      gtNode3 "'a two_atree" 'a "'a two_atree"  




end