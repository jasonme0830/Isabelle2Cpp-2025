#include <benchmark/benchmark.h>
#include <cassert>
#include "merge_sort/merge_sort.hpp"
#include "merge_sort/merge_sort.ml.hpp"
#include "merge_sort/merge_sort.ml.rc.hpp"

static void bench_merge_sort(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = length; i > 0; --i) {
        list.push_back(i);
    }
    auto sorted_list = list;
    sorted_list.sort();

    for (auto _ : state) {
        auto nlist = merge_sort(list);

        state.PauseTiming();
        assert(sorted_list == nlist);
        state.ResumeTiming();
    }
}

static void bench_merge_sort_ml(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = length; i > 0; --i) {
        list.push_back(i);
    }
    auto sorted_list = list;
    sorted_list.sort();

    for (auto _ : state) {
        auto nlist = merge_sort_ml(list);

        state.PauseTiming();
        assert(sorted_list == nlist);
        state.ResumeTiming();
    }
}

static void bench_merge_sort_ml_rc(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = length; i > 0; --i) {
        list.push_back(i);
    }
    auto sorted_list = list;
    sorted_list.sort();

    for (auto _ : state) {
        auto nlist = merge_sort_ml_rc(list);

        state.PauseTiming();
        assert(sorted_list == nlist);
        state.ResumeTiming();
    }
}

BENCHMARK(bench_merge_sort)->RangeMultiplier(2)->Range(256, 16384);
BENCHMARK(bench_merge_sort_ml)->RangeMultiplier(2)->Range(256, 16384);
BENCHMARK(bench_merge_sort_ml_rc)->RangeMultiplier(2)->Range(256, 16384);
BENCHMARK_MAIN();
