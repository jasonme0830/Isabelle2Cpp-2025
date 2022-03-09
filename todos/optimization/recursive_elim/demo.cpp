#include <utility>
#include <stack>
#include <tuple>

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
std::uint64_t factorial_termial(std::uint64_t arg1) {
    if (arg1 <= 1) {
        return 1;
    } else if (arg1 == 2) {
        return 2;
    }

    if (arg1 % 2 == 0) {
        return arg1 * factorial_termial(arg1 - 2);
    }

    return arg1 + factorial_termial(arg1 - 2);
}

std::uint64_t factorial_termial2(std::uint64_t arg1, std::uint64_t acc1 = 0, std::uint64_t acc2 = 1) {
    if (arg1 <= 1) {
        return 1 + acc1; // this requires human reasoning
    } else if (arg1 == 2) {
        return 2 * acc2; // This requires human reasoning
    }

    if (arg1 % 2 == 1) {
        return arg1 + factorial_termial2(arg1 - 2);
    }

    return arg1 * factorial_termial2(arg1 - 2);
}

std::uint64_t fib(std::uint64_t arg1) {
    // fib 0 = 1
    if (arg1 == 0) {
        return 1;
    }

    // fib (Suc 0) = 1
    if (arg1 != 0) {
        if (arg1 - 1 == 0) {
            return 1;
        }
    }

    // fib n = (fib (n - 1)) + (fib (n - 2))
    return fib(arg1 - 1) + fib(arg1 - 2);
}

std::uint64_t fib2(std::uint64_t arg1) {
    std::stack<std::tuple<std::uint64_t>> args;
    args.push(std::make_tuple(arg1));

    std::stack<std::uint64_t> retvals;

    std::stack<std::uint64_t> actions;
    actions.push(0);

    while (true) {
        auto action = actions.top();
        actions.pop();

        switch (action) {
            case 0:
                auto [arg1] = args.top();
                args.pop();

                if (arg1 == 0) {
                    retvals.push(1);
                }

                if (arg1 != 0) {
                    if (arg1 - 1 == 0) {
                        retvals.push(1);
                    }
                }



            case 1:
        }
    }

    // fib n = (fib (n - 1)) + (fib (n - 2))
    return fib2(arg1 - 1) + fib2(arg1 - 2);
}
