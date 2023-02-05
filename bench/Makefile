target = bench

hol2cpp = ../bin/hol2cpp
compare = ../../benchmark/tools/compare.py
format = console

thy = $(wildcard merge_sort/*.thy) $(wildcard binary_search/*.thy) $(wildcard fibonacci/*.thy) $(wildcard memoization/*.thy)
hpp = $(patsubst %.thy, %.hpp, $(thy))
cpp = $(patsubst %.thy, %.cpp, $(thy))

$(target): $(cpp) bench.cpp
	g++ -o $@ -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

%.ml.rc.cpp: %.ml.rc.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list --reduce-cond --predef ../bin/defs.pre

%.ml.ud.cpp: %.ml.ud.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list --use-deque --predef ../bin/defs.pre

%.ml.cpp: %.ml.thy $(hol2cpp)
	$(hol2cpp) $< -s --move-list --predef ../bin/defs.pre

%.ud.cpp: %.ud.thy $(hol2cpp)
	$(hol2cpp) $< -s --use-deque --predef ../bin/defs.pre

%.cpp: %.thy $(hol2cpp)
	$(hol2cpp) $< -s --predef ../bin/defs.pre

.PHONY: clean bm cmp cmp1 cmp2 cmp3 cmp4 cmp5 cmp6

bm: bench
	./bench

cmp: cmp1 cmp2 cmp3 cmp4 cmp5 cmp6 cmp7

cmp1: bench
	python3 $(compare) filters ./bench bench_merge_sort/ bench_merge_sort_ml/ --benchmark_format=$(format)

cmp2: bench
	python3 $(compare) filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_rc/ --benchmark_format=$(format)

cmp3: bench
	python3 $(compare) filters ./bench bench_merge_sort/ bench_merge_sort_ud/ --benchmark_format=$(format)

cmp4: bench
	python3 $(compare) filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_ud/ --benchmark_format=$(format)

cmp5: bench
	python3 $(compare) filters ./bench bench_binary_search/ bench_binary_search_ud/ --benchmark_format=$(format)

cmp6: bench
	python3 $(compare) filters ./bench bench_fib/ bench_fib_mem/ --benchmark_format=$(format)

cmp7: bench
	python3 $(compare) filters ./bench bench_find_fact/ bench_find_fact_mem/ --benchmark_format=$(format)

clean:
	rm -rf $(target) $(hpp) $(cpp)
