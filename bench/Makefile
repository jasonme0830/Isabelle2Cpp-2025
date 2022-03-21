bench: bench.cpp enable-list-move/*.cpp
	g++ -o bench -Wall -O2 -std=c++17 $^ -pthread -lbenchmark

.PHONY: clean bm

bm: bench
	python3 /Users/mu00/Repos/others/benchmark/tools/compare.py filters ./bench bench_disable_list_move bench_enable_list_move --benchmark_format=console

clean: ./bench
	rm bench
