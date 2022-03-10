#include <utility>
#include <tuple>
#include <functional>
#include <stack>

// the definition of ft2 is in recursive2tail.cpp
std::uint64_t ft2(std::uint64_t arg1, std::uint64_t acc1 = 0, std::uint64_t acc2 = 1);

// ft2 to loop
std::uint64_t ft2_loop(std::uint64_t arg1) {
    std::uint64_t acc1 = 0;
    std::uint64_t acc2 = 1;

    while (true) {
        if (arg1 <= 1) {
            return 1 + acc1;
        } else if (arg1 == 2) {
            return 2 * acc2;
        }

        if (arg1 % 2 == 1) {
            std::tie(arg1, acc1, acc2) = std::forward_as_tuple(
                arg1 - 2, arg1 + acc1, acc2
            );
            continue;
        }

        std::tie(arg1, acc1, acc2) = std::forward_as_tuple(
            arg1 - 2, acc1, arg1 * acc2
        );
        continue;
    }
}

// the definition of ft3 is in recursive2tail.cpp
std::uint64_t ft3(std::uint64_t arg1,
    std::function<std::uint64_t(std::uint64_t)> cont = [](std::uint64_t ret) { return ret; }
);

std::uint64_t ft3_loop(std::uint64_t arg1) {
    std::function<std::uint64_t(std::uint64_t)> cont = [](std::uint64_t ret) { return ret; };

    while (true) {
        if (arg1 <= 1) {
            return cont(1);
        } else if (arg1 == 2) {
            return cont(2);
        }

        if (arg1 % 2 == 1) {
            std::tie(arg1, cont) = std::forward_as_tuple(
                arg1 - 2, [=] (std::uint64_t ret) { return cont(arg1 + ret); }
            );
            continue;
        }

        std::tie(arg1, cont) = std::forward_as_tuple(
            arg1 - 2, [=](std::uint64_t ret) { return cont(arg1 * ret); }
        );
        continue;
    }
}
