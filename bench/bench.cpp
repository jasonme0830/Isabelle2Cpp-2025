#include <benchmark/benchmark.h>
#include <cassert>
#include "enable-list-move/merge_sort.enable.hpp"
#include "enable-list-move/merge_sort.disable.hpp"

static void bench_disable_list_move(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = length; i > 0; --i) {
        list.push_back(i);
    }
    auto sorted_list = list;
    sorted_list.sort();

    for (auto _ : state) {
        state.PauseTiming();
        auto nlist = list;
        state.ResumeTiming();

        auto sorted_nlist = merge_sort1(nlist);

        state.PauseTiming();
        assert(sorted_list == sorted_nlist);
        state.ResumeTiming();
    }
}

static void bench_enable_list_move(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = length; i > 0; --i) {
        list.push_back(i);
    }
    auto sorted_list = list;
    sorted_list.sort();

    for (auto _ : state) {
        state.PauseTiming();
        auto nlist = list;
        state.ResumeTiming();

        auto sorted_nlist = merge_sort2(nlist);

        state.PauseTiming();
        assert(sorted_list == sorted_nlist);
        state.ResumeTiming();
    }
}

BENCHMARK(bench_disable_list_move)->RangeMultiplier(10)->Range(10, 10000);
BENCHMARK(bench_enable_list_move)->RangeMultiplier(10)->Range(10, 10000);
BENCHMARK_MAIN();
