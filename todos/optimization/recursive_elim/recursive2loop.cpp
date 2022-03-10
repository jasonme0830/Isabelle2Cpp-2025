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

// the definition of factorial_termial is in recursive2tail.cpp
std::uint64_t factorial_termial(std::uint64_t arg1);

std::uint64_t ft4_loop(std::uint64_t arg1) {
    std::stack<std::uint64_t> states;
    states.push(0);

    std::stack<std::uint64_t> rets;

    std::stack<std::tuple<std::uint64_t>> args1;
    std::stack<std::tuple<std::uint64_t>> args2;

    while (!states.empty()) {
        auto state = states.top(); states.pop();
        switch (state) {
case 0:
        if (arg1 <= 1) {
            rets.push(1);
            continue;
        } else if (arg1 == 2) {
            rets.push(2);
            continue;
        }

        if (arg1 % 2 == 1) {
            args1.push(std::make_tuple(arg1)); // store the variable
            states.push(1);

            std::tie(arg1) = std::forward_as_tuple(arg1 - 2);
            states.push(0);
            continue;
        }

        args2.push(std::make_tuple(arg1));
        states.push(2);

        std::tie(arg1) = std::forward_as_tuple(arg1 - 2);
        states.push(0);
        continue;

case 1: {
        auto [arg1] = args1.top(); args1.pop();
        auto ret1 = rets.top(); rets.pop();
        rets.push(arg1 * ret1);
        continue;
}
case 2: {
        auto [arg1] = args2.top(); args2.pop();
        auto ret1 = rets.top(); rets.pop();
        rets.push(arg1 + ret1);
        continue;
}
        }
    }

    return rets.top();
}
