bench: bench.cpp list-move/merge_sort.enable.cpp \
				 list-move/merge_sort.disable.cpp
	g++ -o bench -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

list-move/merge_sort.disable.cpp: list-move/merge_sort.disable.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s

list-move/merge_sort.enable.cpp: list-move/merge_sort.enable.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s --list-move

.PHONY: clean bm

bm: bench
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_disable_list_move bench_enable_list_move --benchmark_format=console

clean:
	rm bench
	rm list-move/*.hpp list-move/*.cpp
