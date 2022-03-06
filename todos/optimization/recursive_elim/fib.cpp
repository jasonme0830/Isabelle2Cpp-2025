#include <utility>
#include <stack>
#include <tuple>

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

std::uint64_t fact(const std::uint64_t &arg1) {
    // fact 0 = 1
    if (arg1 == 0) {
        return 1;
    }

    // fact n = n * (fact (n - 1))
    return arg1 * fact(arg1 - 1);
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
