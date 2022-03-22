#include <iostream>
#include <cstddef>

std::uint64_t factorial_termial(std::uint64_t arg1) {
    if (arg1 <= 1) {
        return 1;
    } else if (arg1 == 2) {
        return 2;
    }

    if (arg1 % 2 == 1) {
        return arg1 + factorial_termial(arg1 - 2);
    }

    return arg1 * factorial_termial(arg1 - 2);
}

int main() {
    for (std::uint64_t i = 0; i < 10000000; ++i) {
        std::cout << factorial_termial(99);
    }
    std::cout << std::endl;
}
