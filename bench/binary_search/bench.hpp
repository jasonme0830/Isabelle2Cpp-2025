#pragma once

#include "binary_search.hpp"
#include "binary_search.ud.hpp"
#include "binary_search.ml.hpp"

#include <cassert>
#include <benchmark/benchmark.h>

static void bench_binary_search(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = 1; i <= length; ++i) {
        list.push_back(i);
    }

    for (auto _ : state) {
        for (std::uint64_t i = 1; i <= length; i += 2) {
            auto res = bs(i, list);

            state.PauseTiming();
            assert(res.value() == i - 1);
            state.ResumeTiming();
        }

        for (std::uint64_t i = length + 1; i <= length * 2; i += 2) {
            auto res = bs(i, list);

            state.PauseTiming();
            assert(!res.has_value());
            state.ResumeTiming();
        }
    }
}

static void bench_binary_search_ud(benchmark::State &state) {
    auto length = state.range(0);

    std::deque<std::uint64_t> list;
    for (std::uint64_t i = 1; i <= length; ++i) {
        list.push_back(i);
    }

    for (auto _ : state) {
        for (std::uint64_t i = 1; i <= length; i += 2) {
            auto res = bs_ud(i, list);

            state.PauseTiming();
            assert(res.value() == i - 1);
            state.ResumeTiming();
        }

        for (std::uint64_t i = length + 1; i <= length * 2; i += 2) {
            auto res = bs_ud(i, list);

            state.PauseTiming();
            assert(!res.has_value());
            state.ResumeTiming();
        }
    }
}

static void bench_binary_search_ml(benchmark::State &state) {
    auto length = state.range(0);

    std::list<std::uint64_t> list;
    for (std::uint64_t i = 1; i <= length; ++i) {
        list.push_back(i);
    }

    for (auto _ : state) {
        for (std::uint64_t i = 1; i <= length; i += 2) {
            auto res = bs_ml(i, list);

            state.PauseTiming();
            assert(res.value() == i - 1);
            state.ResumeTiming();
        }

        for (std::uint64_t i = length + 1; i <= length * 2; i += 2) {
            auto res = bs_ml(i, list);

            state.PauseTiming();
            assert(!res.has_value());
            state.ResumeTiming();
        }
    }
}

BENCHMARK(bench_binary_search)->RangeMultiplier(2)->Range(256, 16384);
BENCHMARK(bench_binary_search_ud)->RangeMultiplier(2)->Range(256, 16384);
BENCHMARK(bench_binary_search_ml)->RangeMultiplier(2)->Range(256, 16384);
