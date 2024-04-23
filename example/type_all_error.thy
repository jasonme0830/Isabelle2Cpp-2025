theory datatypedefs
  imports Main
begin

datatype ybool = yTrue | yFalse | yMid


datatype 'a newTree = newLeaf |
                      newNode "'a newTree" "'a slist" "'a newTree" 

end