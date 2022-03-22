#include <benchmark/benchmark.h>
#include <cassert>
#include "move-list/merge_sort.enable.hpp"
#include "move-list/merge_sort.disable.hpp"

static void bench_disable_move_list(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = length; i > 0; --i) {
        list.push_back(i);
    }
    auto sorted_list = list;
    sorted_list.sort();

    for (auto _ : state) {
        auto nlist = merge_sort1(list);

        state.PauseTiming();
        assert(sorted_list == nlist);
        state.ResumeTiming();
    }
}

static void bench_enable_move_list(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = length; i > 0; --i) {
        list.push_back(i);
    }
    auto sorted_list = list;
    sorted_list.sort();

    for (auto _ : state) {
        auto nlist = merge_sort2(list);

        state.PauseTiming();
        assert(sorted_list == nlist);
        state.ResumeTiming();
    }
}

BENCHMARK(bench_disable_move_list)->RangeMultiplier(2)->Range(256, 16384);
BENCHMARK(bench_enable_move_list)->RangeMultiplier(2)->Range(256, 16384);
BENCHMARK_MAIN();
