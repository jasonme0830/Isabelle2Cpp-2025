#include <cstddef>
#include <stack>
#include <iostream>

std::uint64_t ft_loop(std::uint64_t arg1) {
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

int main() {
    for (std::uint64_t i = 0; i < 10000000; ++i) {
        std::cout << ft_loop(99);
    }
    std::cout << std::endl;
}
