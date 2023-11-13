theory datatypes
imports Main
begin


datatype snat = sZero | sSucc snat
datatype 'a slist = sNil | sCons 'a "'a slist"
datatype sbool = sTrue | sFalse
datatype 'a soption = None | Some 'a

datatype 'b alist = sNil | sCons 'b "'b alist"

datatype 'a tree = Leaf | Node "'a tree" 'a "'a tree"
datatype 'a stree = 
                  sbool |
                  nat |
                  tNode "'a stree" "'a stree" "'a stree" |
                  Node "'a stree" 'a "'a stree" |
                  nNode nat "'a stree" |
                  lNode sbool "'a stree" "'a slist" 


datatype ('a, 'b) my_type =  Constructor1   'a |
                             Constructor2 'b "'a list"

datatype ('a, 'b) ttree = aNode 'a |
                          bNode 'b |
                          asNode "('a, 'b) ttree" 'a "('a, 'b) ttree" |
                          sNode "'a stree" 'b "'a stree" |
                          lNode "'b stree" 'a 
                          
datatype ('c, 'd) ytree = aNode 'c |
                          bNode 'd |
                          asNode "('c, 'd) ttree" 'c "('c, 'd) ttree" |
                          sNode "'c stree" 'd "'c stree" |
                          lNode "'d stree" 'c 

datatype ('d, 'c) ptree = aNode 'd |
                          bNode 'c |
                          asNode "('c, 'd) ttree" 'c "('c, 'd) ttree" |
                          sNode "'c stree" 'd "'c stree" |
                          lNode "'d stree" 'c 



end