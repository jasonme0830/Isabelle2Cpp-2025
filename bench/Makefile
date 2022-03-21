bench: bench.cpp enable-list-move/merge_sort.enable.cpp \
				 enable-list-move/merge_sort.disable.cpp
	g++ -o bench -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

enable-list-move/merge_sort.disable.cpp: enable-list-move/merge_sort.disable.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s

enable-list-move/merge_sort.enable.cpp: enable-list-move/merge_sort.enable.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s --enable-list-move

.PHONY: clean bm

bm: bench
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_disable_list_move bench_enable_list_move --benchmark_format=console

clean: ./bench
	rm bench
	rm enable-list-move/*.hpp enable-list-move/*.cpp
