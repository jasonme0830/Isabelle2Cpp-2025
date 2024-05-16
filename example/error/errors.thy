theory errors
    imports Main
begin
datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 

fun ffilter::"('a=>bool) =>'a list=>'a list "where
"ffilter f [] = [] "|
"ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))"

fun QuickSort::"'a list =>('a::ord) list "where
"QuickSort [] =  []"|
"QuickSort (x#xs) = (QuickSort (ffilter(\<lambda> y . y\<le>x) xs))@x #(QuickSort (ffilter(\<lambda> y . y>x)xs)) "

fun rightest::"'a tree=>'a" where
"rightest Tip = undefined" |
"rightest (Node left x right) = (if right=Tip then x 
                                 else (rightest right)) "
                                 
end