#include <utility>

// same as the function in recursive2tail.cpp, which has parameters with default values
std::uint64_t factorial_termial2(std::uint64_t arg1) {
    return factorial_termial2_impl(arg1, 0, 1);
}

std::uint64_t factorial_termial2_impl(std::uint64_t arg1, std::uint64_t acc1, std::uint64_t acc2) {
    if (arg1 <= 1) {
        return 1 + acc1;
    } else if (arg1 == 2) {
        return 2 * acc2;
    }

    if (arg1 % 2 == 1) {
        return factorial_termial2_impl(arg1 - 2, arg1 + acc1, acc2);
    }

    return factorial_termial2_impl(arg1 - 2, acc1, arg1 * acc2);
}

// convert to loop
std::uint64_t factorial_termial2_impl2(std::uint64_t arg1, std::uint64_t acc1, std::uint64_t acc2) {
    while (true) {
        if (arg1 <= 1) {
            return 1 + acc1;
        } else if (arg1 == 2) {
            return 2 * acc2;
        }
    }

    if (arg1 <= 1) {
        return 1 + acc1;
    } else if (arg1 == 2) {
        return 2 * acc2;
    }

    if (arg1 % 2 == 1) {
        return factorial_termial2_impl2(arg1 - 2, arg1 + acc1, acc2);
    }

    return factorial_termial2_impl2(arg1 - 2, acc1, arg1 * acc2);
}
