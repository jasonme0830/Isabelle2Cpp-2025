bench: a.out b.out c.out d.out
	time ./a.out > /dev/null
	time ./b.out > /dev/null
	time ./c.out > /dev/null
	time ./d.out > /dev/null

a.out: merge_sort.disable.cpp
	clang++ -std=c++17 -O0 merge_sort.disable.cpp -o a.out

b.out: merge_sort.enable.cpp
	clang++ -std=c++17 -O0 merge_sort.enable.cpp -o b.out

c.out: merge_sort.disable.cpp
	clang++ -std=c++17 -O2 merge_sort.disable.cpp -o c.out

d.out: merge_sort.disable.cpp
	clang++ -std=c++17 -O2 merge_sort.enable.cpp -o d.out
