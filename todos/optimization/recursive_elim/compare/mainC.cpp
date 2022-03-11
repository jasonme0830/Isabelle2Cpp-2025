#include <functional>
#include <cstddef>
#include <iostream>

std::uint64_t ft3(std::uint64_t arg1,
    std::function<std::uint64_t(std::uint64_t)> cont = [](std::uint64_t ret) { return ret; }
) {
    if (arg1 <= 1) {
        return cont(1);
    } else if (arg1 == 2) {
        return cont(2);
    }

    if (arg1 % 2 == 1) {
        return ft3(arg1 - 2, [=] (std::uint64_t ret) { return cont(arg1 + ret); });
    }

    return ft3(arg1 - 2, [=](std::uint64_t ret) { return cont(arg1 * ret); });
}

int main() {
    for (std::uint64_t i = 0; i < 10000000; ++i) {
        std::cout << ft3(99);
    }
    std::cout << std::endl;
}
