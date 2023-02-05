#pragma once

#include "fact.ml.ud.hpp"

#include <cassert>
#include <benchmark/benchmark.h>

static std::uint64_t correct_fact(const std::uint64_t &x) {
    std::uint64_t a = 1;

    for (std::uint64_t i = 1; i <= x; ++i) {
        a *= i;
    }

    return a;
}

// correct find_if function
static std::optional<std::uint64_t> correct_find_fact(const std::deque<std::uint64_t> &xs, const std::uint64_t &x) {
    for (const auto &i : xs) {
        if (correct_fact(i) == x) {
            return i;
        }
    }

    return std::nullopt;
}

static void bench_find_fact(benchmark::State &state) {
    auto x = state.range(0);

    std::deque<std::uint64_t> xs;
    for (std::uint64_t i = 1; i <= x; ++i) {
        xs.push_back(i);
    }

    auto y = x % 2 ? fact(x) : fact(x + 1);

    for (auto _ : state) {
        auto res = find_fact(xs, y);

        state.PauseTiming();
        assert(res == correct_find_fact(xs, y));
        state.ResumeTiming();
    }
}

static void bench_find_fact_mem(benchmark::State &state) {
    auto x = state.range(0);

    std::deque<std::uint64_t> xs;
    for (std::uint64_t i = 1; i <= x; ++i) {
        xs.push_back(i);
    }

    auto y = x % 2 ? fact(x) : fact(x + 1);

    for (auto _ : state) {
        auto res = find_fact_mem(xs, y);

        state.PauseTiming();
        assert(res == correct_find_fact(xs, y));
        state.ResumeTiming();
    }
}

BENCHMARK(bench_find_fact)->DenseRange(4, 12, 1);
BENCHMARK(bench_find_fact_mem)->DenseRange(4, 12, 1);
