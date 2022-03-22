#include <utility>
#include <stack>
#include <functional>

std::uint64_t factorial(const std::uint64_t &arg1) {
    // factorial 0 = 1
    if (arg1 == 0) {
        return 1;
    }

    // factorial n = n * (fact (n - 1))
    return arg1 * factorial(arg1 - 1);
}

std::uint64_t factorial2(std::uint64_t arg1, std::uint64_t acc = 1) {
    // factorial2 0 = 1
    if (arg1 == 0) {
        return acc;
    }

    // factorial2 n = n * (fact (n - 1))
    return factorial2(arg1 - 1, arg1 * acc);
}

std::uint64_t termial(std::uint64_t arg1) {
    if (arg1 == 0) {
        return 0;
    }

    return arg1 + termial(arg1 - 1);
}

std::uint64_t termial2(std::uint64_t arg1, std::uint64_t acc = 0) {
    if (arg1 == 0) {
        return acc;
    }

    return termial2(arg1 - 1, arg1 + acc);
}

// mf 1 = 1
// mf 2 = 2
// mf 3 = 3
// mf 4 = 4 * 2 = 6
// mf 5 = 5 * 3 = 15
std::uint64_t mult_factorial(const std::uint64_t &arg1) {
    if (arg1 <= 1) {
        return 1;
    } else if (arg1 == 2) {
        return 2;
    } else if (arg1 == 3) {
        return 3;
    }

    return arg1 * mult_factorial(arg1 - 2);
}

std::uint64_t mult_factorial2(std::uint64_t arg1, std::uint64_t acc = 1) {
    if (arg1 <= 1) {
        return 1 * acc;
    } else if (arg1 == 2) {
        return 2 * acc;
    } else if (arg1 == 3) {
        return 3 * acc;
    }

    return mult_factorial2(arg1 - 2, arg1 * acc);
}

// mt 1 = 1
// mt 2 = 2
// mt 3 = 3 + 1 = 4
std::uint64_t mult_termial(std::uint64_t arg1) {
    if (arg1 == 0) {
        return 0;
    } else if (arg1 == 1) {
        return 1;
    }

    return arg1 + mult_termial(arg1 - 2);
}

std::uint64_t mult_termial2(std::uint64_t arg1, std::uint64_t acc = 0) {
    if (arg1 == 0) {
        return 0 + acc;
    } else if (arg1 == 1) {
        return 1 + acc;
    }

    return mult_termial2(arg1 - 2, arg1 + acc);
}

/**
 * above cases need finding identity-element,
 * but the following cases need human reasoning
*/

// ft 0 = 1
// ft 1 = 1
// ft 2 = 2
// ft 3 = 3 + 1 = 4
// ft 4 = 4 * 2 = 8
// ft 5 = 5 + 3 + 1 = 9
// ft 6 = 6 * 4 * 2 = 48
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

std::uint64_t ft2(std::uint64_t arg1, std::uint64_t acc1 = 0, std::uint64_t acc2 = 1) {
    if (arg1 <= 1) {
        return 1 + acc1; // this requires human reasoning
    } else if (arg1 == 2) {
        return 2 * acc2; // This requires human reasoning
    }

    if (arg1 % 2 == 1) {
        return ft2(arg1 - 2, arg1 + acc1, acc2);
    }

    return ft2(arg1 - 2, acc1, arg1 * acc2);
}

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
