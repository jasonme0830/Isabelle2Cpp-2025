theory no_argu
  imports Main
begin

datatype abool = aTrue | aFalse
datatype bbool = bTrue | bFalse | bMid
datatype cbool = cTrue | cFalse | cLeft | cright
datatype dbool = dTrue | dFalse
datatype ebool = eFalse | eTrue
datatype fbool = aTrue | aFalse
datatype gbool = gMid | gFalse | gTrue

datatype anat = aZero | aSuc anat
datatype bnat = bSuc bnat | bZero
datatype cnat = aZero | aSuc cnat
datatype dnat = dZero | dSuc dnat | dCon nat
datatype enat = eCon nat | eSuc enat | eZero
datatype fnat = fZero | fSuc fnat | fCon int

datatype nat_aoption = aNone | aSome nat
datatype int_boption = bNone | bSome int
datatype two_coption = cNone | cNat nat | cInt int
datatype nor_doption = dNone | dNat nat | dBool bool
datatype tem_eoption = eNone | eNat nat | eList "nat list"
datatype bol_foption = fNone | fBool bool
datatype cal_goption = gNone | gTwo two_coption | gBol bol_foption
datatype cal_hoption = hNone | hNat "nat option" 
datatype cal_ioption = iNone | iInt "int option"
datatype cal_joption = jNat "nat option" | jNone
datatype tem_koption = kNat nat | kList "nat list" | kNone
datatype nor_loption = dNone | dNat nat | dBool bool

datatype nat_alist = aNil | aCons1 nat nat_alist
datatype nat_blist = bNil | bCons1 nat
datatype nat_clist = cNil | cCons1 nat_clist
datatype nat_dlist = dNil | dCons1 "nat list"
datatype nat_elist = eNil | eCons1 nat | eCons2 nat_elist
datatype nat_flist = fNil | eCons1 nat_flist | eCons2 nat
datatype nat_glist = gCons1 nat | gCons2 nat_glist | gNil
datatype nat_hlist = hNil | hCons1 nat | hCons2 nat nat_hlist
datatype nat_ilist = iNil | iCons1 nat | iCons2 "nat list"
datatype nat_jlist = iNil | iCons1 nat | iCons2 "nat list"
datatype int_klist = kNil | kCons1 int | kCons2 "int list"
datatype two_llist = lNil | lCons1 nat | lCons2 int | lCons3 two_llist
datatype two_mlist = mNil | mCons1 nat int | mCons2 nat "int list"
datatype two_nlist = nNil | nCons1 int nat | nCons2 nat "int list"
datatype two_olist = oNil | oCons1 nat int | oCons2 int "nat list"
datatype two_plist = pNil | pCons1 int nat | pCons2 int "nat list"
datatype two_qlist = qNil | qCons1 int | qCons2 nat two_qlist
datatype two_rlist = rNil | rCons1 nat | rCons2 int two_rlist
datatype two_slist = sNil | sCons1 int | sCons2 nat "int list"
datatype two_tlist = tNil | tCons1 nat | tCons2 int "nat list"

datatype nat_atree = aLeaf | aNode nat
datatype nat_btree = bLeaf | bNode nat_btree nat 
datatype nat_ctree = cLeaf | cNode nat nat_ctree
datatype nat_dtree = dLeaf | dNode nat_dtree nat nat_dtree
datatype nat_etree = eLeaf | eNode nat nat_etree nat_etree
datatype nat_ftree = fLeaf | fNode nat_ftree nat_ftree nat
datatype nat_gtree = gLeaf | gNode1 nat | gNode2 nat nat_gtree
datatype nat_htree = hLeaf | hNode1 nat | hNode2 nat_htree nat nat_htree
datatype int_itree = iLeaf | iNode1 int | iNode2 int_itree int int_itree
datatype two_jtree = jLeaf | jNode1 int | jNode2 nat | jNode3 two_jtree int two_jtree
datatype two_ktree = kLeaf | kNode1 nat | kNode2 int | kNode3 two_ktree nat two_ktree
datatype two_ltree = lLeaf | lNode1 int | lNode2 nat | lNode3 two_ltree int nat two_ltree
datatype two_mtree = mLeaf | mNode1 two_mtree int | mNode2 nat two_mtree 
datatype two_ntree = nLeaf | nNode1 int two_ntree | nNode2 two_ntree nat
datatype tem_otree = oLeaf | oNode1 tem_otree "nat list" | oNode2 "int list" tem_otree
datatype tem_ptree = pLeaf | pNode1 tem_ptree "int list" | pNode2 "nat list" tem_ptree


end