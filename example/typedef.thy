theory datatypes
imports Main
begin


datatype snat = sZero | sSucc snat
datatype 'a slist = sNil | sCons 'a "'a slist"
datatype sbool = sTrue | sFalse
datatype option = None | Some 'a

datatype 'b alist = sNil | sCons 'b "'b alist"

datatype 'a tree = Leaf | Node "'a tree" 'a "'a tree"

end