#include <utility>
#include <tuple>

std::uint64_t factorial_termial2(std::uint64_t arg1, std::uint64_t acc1 = 0, std::uint64_t acc2 = 1) {
    if (arg1 <= 1) {
        return 1 + acc1;
    } else if (arg1 == 2) {
        return 2 * acc2;
    }

    if (arg1 % 2 == 1) {
        return factorial_termial2(arg1 - 2, arg1 + acc1, acc2);
    }

    return factorial_termial2(arg1 - 2, acc1, arg1 * acc2);
}

// tail to loop
std::uint64_t factorial_termial3(std::uint64_t arg1) {
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
