theory datatypedefs
  imports Main
begin

datatype snat = sZero | sSuc snat
datatype pnat = pZero | pSuc pnat
datatype tnat = tSuc tnat | tZero
datatype ynat = yZero | ySuc snat


datatype sbool = sTrue | sFalse
datatype pbool = pTrue | pFalse
datatype tbool = tFalse | tTrue


datatype 'a toption = tNone | tSome 'a
datatype 'a poption = pNode | pSome 'a
datatype 'a soption = sSome 'a | sNode
datatype 'a yoption = yNode | yOnce | ySome 'a
datatype 'a xoption = xSome 'a | xOnce | xNode
datatype 'a zoption = zSome 'a | zOnce 'a | zNode



datatype 'a slist = sNil | 
                    sCons 'a "'a slist"
datatype 'a tlist = tCons 'a "'a tlist" |
                    tNil
datatype 'a plist = pNil | 
                    pCons 'a "'a plist"
datatype 'a mlist = mNil |
                    mCons 'a "'a slist"
datatype 'a nlist = nNil |
                    nCons 'a "'a plist"
datatype 'a zlist = zNil |
                    zCons "'a zlist" 'a
datatype 'a xlist = xNil |
                    xCons 'a |
                    xConss "'a xlist" 'a




datatype 'a atree = aLeaf | 
                    aNode1 "'a atree" 'a "'a atree"
datatype 'a btree = bLeaf |
                    bNode1 'a |
                    bNode2 "'a btree" 'a "'a btree" 
datatype 'b ctree = cNode1 "'b ctree" 'b "'b ctree" | 
                    cNode2 'b |
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


datatype 'a newTree = newLeaf |
                      newNode "'a newTree" "'a slist" "'a newTree" |
                      nawNode "'a newTree" snat "'a newTree"
end


