#include <cstdint>

std::uint64_t fib_raw(std::uint64_t n) {
    return n < 2 ? 1 : fib_raw(n - 1) + fib_raw(n - 2);
}

std::uint64_t fib_raw_quick(std::uint64_t n) {
    std::uint64_t a = 1;
    std::uint64_t b = 1;

    for (std::uint64_t i = 1; i < n; ++i) {
        auto c = a + b;
        a = b;
        b = c;
    }

    return b;
}
