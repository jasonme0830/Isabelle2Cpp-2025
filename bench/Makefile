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

.PHONY: clean bm b1 b2

compare = /Users/mu00/Repos/others/benchmark/tools/compare.py

bm_msml = python3 $(compare) filters ./bench bench_merge_sort/ bench_merge_sort_ml/ --benchmark_format=console
bm_msrc = python3 $(compare) filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_rc/ --benchmark_format=console

bm: bench
	$(bm_msml)
	$(bm_msrc)

b1: bench
	$(bm_msml)

b2: bench
	$(bm_msrc)

clean:
	rm -rf $(target) $(hpp) $(cpp)
