theory datatypes
imports Main
begin


datatype 'a atree = aLeaf | 
                    aNode1 "'a atree" 'a "'a atree"
datatype 'a btree = bLeaf |
                    bNode1 'a |
                    bNode2 "'a btree" 'a "'a btree" 
datatype 'a ctree = cNode1 "'a ctree" 'a "'a ctree" | 
                    cNode2 'a |
                    cLeaf 

datatype 'a dtree = dLeaf |
                    dNode1 'a |
                    dNode2 'a "'a dtree" 
datatype 'a etree = eLeaf |
                    eNode1 'a "'a etree" |
                    eNode2 "'a etree" 'a "'a etree"
datatype 'a ftree = fLeaf |
                    fNode1 'a "'a ftree" |
                    fNode2 "'a ftree" "'a ftree" 'a 
datatype 'a gtree = gNode1 'a "'a gtree" "'a gtree" |
                    gNode2 "'a gtree" 'a |
                    gLeaf
datatype 'a htree = hNode1 "'a htree" "'a htree" 'a |
                    hNode2 'a "'a htree" |
                    hLeaf



end