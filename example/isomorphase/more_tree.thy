theory more_tree
  imports Main
begin

datatype abool = aTrue | aFalse
datatype anat = aZero | aSuc anat
datatype 'a aoption = aNone | aSome 'a
datatype 'a nat_blist = bNil | bCons 'a "'a nat_blist"
datatype 'a tree = Leaf | Node "'a tree" 'a "'a tree"
datatype ('a,'b) onechoose = None | aOne 'a | aTwo 'b

datatype ('a,'b,'c) thr_atree = atLeaf | atNode 'a 'b 'c
datatype ('a,'b,'c) thr_btree =  btLeaf | btNode 'a 'a 'a
datatype ('a,'b,'c) thr_ctree =  ctLeaf |  ctNode 'b 'b 'b
datatype ('a,'b,'c) thr_dtree = dtLeaf | dtNode 'c 'b 'a
datatype ('a,'b,'c) thr_etree = etLeaf | etNode 'a nat int
datatype ('a,'b,'c) thr_ftree = ftLeaf | ftNode 'b 'c 'a
datatype ('a,'b,'c) thr_gtree = gtLeaf | gtNode1 'a | gtNode2 'b 'c
datatype ('a,'b,'c) thr_htree = htLeaf | htNode1 'b | htNode2 'a 'c
datatype ('a,'b,'c) thr_itree = itLeaf | itNode1 'a 'a | itNode2 'b nat
datatype ('a,'b,'c) thr_jtree = jtLeaf | jtNode1 'a 'a | jtNode2 'c int
datatype ('a,'b,'c) thr_ktree = ktLeaf | ktNode1 'a 'a | ktNode2 'b int
datatype ('a,'b,'c) thr_ltree = ltLeaf | ltNode1 'a 'a | ltNode2 'c nat
datatype ('a,'b,'c) thr_mtree = mtLeaf | mtNode1 'a 'a | mtNode2 'b 'b 'c
datatype ('a,'b,'c) thr_ntree = ntLeaf | ntNode1 'a 'a | ntNode2 'b 'c 'c
datatype ('a,'b,'c) thr_otree = otLeaf | otNode1 'a 'c | otNode2 'c 'b 'b
datatype ('a,'b,'c) thr_ptree = ptLeaf | ptNode1 'c 'a | ptNode2 'c 'b 'b
datatype ('a,'b,'c) thr_qtree = qtLeaf | qtNode1 'b 'c | qtNode2 'a
datatype ('a,'b,'c) thr_rtree = rtLeaf | rtNode1 'a | rtNode2 'b 'c 'a
datatype ('a,'b,'c) thr_stree = stLeaf | stNode1 'c 'a 'b | stNode2 'b
datatype ('a,'b,'c) thr_ttree = ttLeaf | ttNode1 'b 'c 'a | ttNode2 'a
datatype ('a,'b,'c) thr_utree = utLeaf | utNode1 'a | utNode2 'b 'c | utNode3 'a 'b 'c
datatype ('a,'b,'c) thr_vtree = vtLeaf | vtNode1 'b | vtNode2 'c 'a | vtNode3 'a 'b 'c

datatype ('a,'b,'c) rec_atree = 
  arLeaf |
  arNode1 'a 'b 'c |
  arNode2 'a "('a,'b,'c) rec_atree" |
  arNode3 "('a,'b,'c) rec_atree" 'a "('a,'b,'c) rec_atree"

datatype ('a,'b,'c) rec_btree = 
  brLeaf |
  brNode1 'b 'c 'a |
  brNode2 'b "('a,'b,'c) rec_btree" |
  brNode3 "('a,'b,'c) rec_btree" 'b "('a,'b,'c) rec_btree"

datatype ('a,'b,'c) rec_ctree =
  crLeaf |
  crNode1 'a 'b 'c |
  crNode2 'a "('a,'b,'c) rec_atree" |
  crNode3 "('a,'b,'c) rec_atree" 'a "('a,'b,'c) rec_atree"

datatype ('a,'b,'c) rec_dtree = 
  drLeaf |
  drNode1 'b 'c 'a |
  drNode2 'b "('a,'b,'c) rec_atree" |
  drNode3 "('a,'b,'c) rec_atree" 'b "('a,'b,'c) rec_atree"

datatype ('a,'b,'c) rec_etree = 
  erLeaf |
  erNode1 'a |
  erNode2 'b |
  erNode3 "('a,'b,'c) rec_etree" 'c "('a,'b,'c) rec_etree"

datatype ('d,'e,'f) rec_ftree = 
  frLeaf |
  frNode1 'd |
  frNode2 'e |
  frNode3 "('d,'e,'f) rec_ftree" 'f "('d,'e,'f) rec_ftree"

datatype ('a,'b,'c) rec_gtree = 
  grLeaf |
  grNode1 'b |
  grNode2 'c |
  grNode3 "('a,'b,'c) rec_gtree" 'a "('a,'b,'c) rec_gtree"

datatype ('a,'b,'c) rec_htree =
  hrLeaf |
  hrNode1  "('a,'b,'c) rec_gtree" 'a "('a,'b,'c) rec_gtree" |
  hrNode2 'c |
  hrNode3 'b

datatype ('a,'b,'c) rec_itree =
  irLeaf |
  irNode1 'c |
  irNode2 nat |
  irNode3 "'a tree" 'b "'a tree"

datatype ('a,'b,'c) rec_jtree =
  jrLeaf |
  jrNode1 nat |
  jrNode2 'b |
  jrNode3 "'c tree" 'a "'c tree"

datatype ('a,'b,'c) rec_ktree =
  krLeaf |
  krNode1 "nat tree" 'a "nat tree" |
  krNode2 'c |
  krNode3 'a int

datatype ('d,'e,'f) rec_ltree =
  lrLeaf |
  lrNode1 "nat tree" 'd "nat tree" |
  lrNode2 'e |
  lrNode3 'f int

datatype ('a,'b,'c) rec_mtree = 
  mrLeaf |
  mrNode1 'a 'b |
  mrNode2 'b 'c |
  mrNode3 "'a aoption" 'b "('a,'c) onechoose"

datatype ('a,'b,'c) rec_ntree =
  nrLeaf |
  nrNode1 'a 'c |
  nrNode2 'c 'b |
  nrNode3 "'a aoption" 'c "('a,'b) onechoose"


end