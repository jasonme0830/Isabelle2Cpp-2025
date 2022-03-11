#include <utility>
#include <stack>
#include <tuple>

/**
 * fib(fib(arg1 - 1) - 2)
 *
 * push state 1
 * arg1 = arg1 - 1
 * push state 0
 *
 * fib(ret1 - 2)
 *
 * pop ret1
 * arg1 = ret1 - 2
 * push state 0
*/

/**
 * fib(arg1 - 1) + fib(arg1 - 2)
 *
 * push arg1
 * push state 1
 * arg1 = arg1 - 1
 * push state 0
 *
 * ret1 + fib(arg1 - 2)
 *
 * pop ret1
 * push ret1
 * push state 2
 * arg1 = arg1 - 2;
 * push state 0
 *
 * ret1 + ret2
 *
 * pop ret2
 * pop ret1
 * push ret1 + ret2
*/

/**
 * 一次遍历，记录每个递归调用的次序，维护每个递归调用对应的 case N
 * 遍历到一个递归调用时，先添加参数更新和 push state 0 语句
 * 变量可以判断其声明位置，N 之前的 case 则可以在开头添加 push 语句
 * （或者分别维护 pop/push/push-state 语句块）
 * 同时需要在当前语句前添加 pop ret 语句，ret 也根据 N 编号
 * 遍历完成后，即可添加 push state N + 1
 *
 * N > 0 的 case 需要 pop variable
*/

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

std::uint64_t fib_loop(std::uint64_t arg1) {
    std::stack<std::uint64_t> states;
    states.push(0);

    std::stack<std::uint64_t> rets;
    std::stack<std::uint64_t> arg1s;

    while (!states.empty()) {
        auto state = states.top(); states.pop();
        switch (state) {
case 0:
        if (arg1 == 0) {
            rets.push(1);
            continue;
        }

        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                rets.push(1);
                continue;
            }
        }

        arg1s.push(arg1);
        states.push(1);

        std::tie(arg1) = std::forward_as_tuple(arg1 - 1);
        states.push(0);
        continue;

case 1: {
        auto arg1 = arg1s.top(); arg1s.pop();
        states.push(2);

        std::tie(arg1) = std::forward_as_tuple(arg1 - 2);
        states.push(0);
        continue;
}
case 2: {
        auto ret2 = rets.top(); rets.pop();
        auto ret1 = rets.top(); rets.pop();
        rets.push(ret1 + ret2);
        continue;
}
        }
    }
}

// the definition of factorial_termial is in recursive2tail.cpp
std::uint64_t factorial_termial(std::uint64_t arg1);

std::uint64_t ft4_loop(std::uint64_t arg1) {
    std::stack<std::uint64_t> states;
    states.push(0);

    std::stack<std::uint64_t> rets;
    std::stack<std::uint64_t> arg1s;

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
            arg1s.push(arg1); // store the variable
            states.push(1);

            std::tie(arg1) = std::forward_as_tuple(arg1 - 2);
            states.push(0);
            continue;
        }

        arg1s.push(arg1);
        states.push(2);

        std::tie(arg1) = std::forward_as_tuple(arg1 - 2);
        states.push(0);
        continue;

case 1: {
        auto arg1 = arg1s.top(); arg1s.pop();
        auto ret1 = rets.top(); rets.pop();
        rets.push(arg1 + ret1);
        continue;
}
case 2: {
        auto arg1 = arg1s.top(); arg1s.pop();
        auto ret1 = rets.top(); rets.pop();
        rets.push(arg1 * ret1);
        continue;
}
        }
    }

    return rets.top();
}
