target = bench

thy = $(wildcard merge_sort/*.thy) $(wildcard binary_search/*.thy)
hpp = $(patsubst %.thy, %.hpp, $(thy))
cpp = $(patsubst %.thy, %.cpp, $(thy))

hol2cpp = ../bin/hol2cpp

$(target): $(cpp) bench.cpp
	g++ -o $@ -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

%.ml.rc.cpp: %.ml.rc.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list --reduce-cond

%.ml.cpp: %.ml.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list

%.cpp: %.thy $(hol2cpp)
	$(hol2cpp) $< -s

.PHONY: clean bm cmp cmp1 cmp2

compare = /Users/mu00/Repos/others/benchmark/tools/compare.py

bm_cmp1 = python3 $(compare) filters ./bench bench_merge_sort/ bench_merge_sort_ml/ --benchmark_format=console
bm_cmp2 = python3 $(compare) filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_rc/ --benchmark_format=console

bm: bench
	./bench

cmp: bench
	$(bm_cmp1)
	$(bm_cmp2)

cmp1: bench
	$(bm_cmp1)

cmp2: bench
	$(bm_cmp2)

clean:
	rm -rf $(target) $(hpp) $(cpp)
