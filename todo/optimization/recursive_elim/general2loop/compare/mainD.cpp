#include <functional>
#include <cstddef>
#include <iostream>

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

int main() {
    for (std::uint64_t i = 0; i < 10000000; ++i) {
        std::cout << ft3_loop(99);
    }
    std::cout << std::endl;
}
