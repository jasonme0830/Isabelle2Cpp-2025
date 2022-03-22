bench: bench.cpp move-list/merge_sort.enable.cpp \
				 move-list/merge_sort.disable.cpp
	g++ -o bench -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

move-list/merge_sort.disable.cpp: move-list/merge_sort.disable.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s

move-list/merge_sort.enable.cpp: move-list/merge_sort.enable.thy ../bin/hol2cpp
	../bin/hol2cpp $< -s --move-list

.PHONY: clean bm

bm: bench
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_disable_move_list bench_enable_move_list --benchmark_format=console

clean:
	rm bench
	rm move-list/*.hpp move-list/*.cpp
