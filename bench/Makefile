bench: bench.cpp merge_sort/merge_sort.cpp \
				 merge_sort/merge_sort.ml.cpp \
				 merge_sort/merge_sort.ml.rc.cpp
	g++ -o bench -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

merge_sort/merge_sort.cpp: merge_sort/merge_sort.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s

merge_sort/merge_sort.ml.cpp: merge_sort/merge_sort.ml.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s --move-list

merge_sort/merge_sort.ml.rc.cpp: merge_sort/merge_sort.ml.rc.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s --move-list --reduce-cond

.PHONY: clean bm b1 b2

bm: bench
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_merge_sort/ bench_merge_sort_ml/ --benchmark_format=console
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_rc/ --benchmark_format=console

b1: bench
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_merge_sort/ bench_merge_sort_ml/ --benchmark_format=console

b2: bench
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_merge_sort_ml/ bench_merge_sort_ml_rc/ --benchmark_format=console

clean:
	rm bench
	rm merge_sort/*.hpp merge_sort/*.cpp
