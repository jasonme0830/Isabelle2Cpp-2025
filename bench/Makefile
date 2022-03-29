target = bench

thy = $(wildcard merge_sort/*.thy) $(wildcard binary_search/*.thy)
hpp = $(patsubst %.thy, %.hpp, $(thy))
cpp = $(patsubst %.thy, %.cpp, $(thy))

hol2cpp = ../bin/hol2cpp

$(target): $(cpp) bench.cpp
	g++ -o $@ -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

%.ml.rc.cpp: %.ml.rc.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list --reduce-cond

%.ml.ud.cpp: %.ml.ud.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list --use-deque

%.ml.cpp: %.ml.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list

%.ud.cpp: %.ud.thy $(hol2cpp)
	$(hol2cpp) $< -s --use-deque

%.cpp: %.thy $(hol2cpp)
	$(hol2cpp) $< -s

.PHONY: clean bm cmp cmp1 cmp2

compare = /Users/mu00/Repos/others/benchmark/tools/compare.py

bm_cmp1 = python3 $(compare) filters ./bench bench_merge_sort/ bench_merge_sort_ml/ --benchmark_format=console
bm_cmp2 = python3 $(compare) filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_rc/ --benchmark_format=console
bm_cmp3 = python3 $(compare) filters ./bench bench_merge_sort/ bench_merge_sort_ud/ --benchmark_format=console
bm_cmp4 = python3 $(compare) filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_ud/ --benchmark_format=console

bm: bench
	./bench

cmp: bench
	$(bm_cmp1)
	$(bm_cmp2)
	$(bm_cmp3)
	$(bm_cmp4)

cmp1: bench
	$(bm_cmp1)

cmp2: bench
	$(bm_cmp2)

cmp3: bench
	$(bm_cmp3)

cmp4: bench
	$(bm_cmp4)

clean:
	rm -rf $(target) $(hpp) $(cpp)
