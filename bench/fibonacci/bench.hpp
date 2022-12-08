#pragma once

#include "fib.hpp"

#include <cassert>
#include <benchmark/benchmark.h>

static std::uint64_t correct_fib(const std::uint64_t &x) {
    std::uint64_t a = 1;
    std::uint64_t b = 1;

    for (std::uint64_t i = 1; i < x; ++i) {
        auto c = a + b;
        a = b;
        b = c;
    }

    return b;
}

static void bench_fib(benchmark::State &state) {
    auto x = state.range(0);

    for (auto _ : state) {
        auto res = fib(x);

        state.PauseTiming();
        assert(res == correct_fib(x));
        state.ResumeTiming();
    }
}

static void bench_fib_mem(benchmark::State &state) {
    auto x = state.range(0);

    for (auto _ : state) {
        auto res = fib_mem(x);

        state.PauseTiming();
        assert(res == correct_fib(x));
        state.ResumeTiming();
    }
}

// bench the fibonacci function from input 1 to 20 incrementing by 1
BENCHMARK(bench_fib)->DenseRange(1, 25, 1);
// bench the fibonacci function from input 1 to 20 incrementing by 1
BENCHMARK(bench_fib_mem)->DenseRange(1, 25, 1);
