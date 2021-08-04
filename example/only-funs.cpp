#include <cstdint>
#include <cstdlib>
#include <list>
#include <optional>
#include <set>
#include <utility>

std::uint64_t add(const std::uint64_t &arg1, const std::uint64_t &arg2) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return arg2;
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto m = (arg1) - 1;
        return (add(m, arg2)) + 1;
    }
    std::abort();
}

template<typename T1>
std::list<T1> app(const std::list<T1> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return arg2;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = app(xs, arg2);
        temp0.push_front(x);
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> rev(const std::list<T1> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = std::list<T1>();
        temp0.push_front(x);
        return app(rev(xs), temp0);
    }
    std::abort();
}

template<typename T1>
std::list<T1> rev2(const std::list<T1> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = std::list<T1>();
        temp0.push_front(x);
        auto temp1 = rev2(xs);
        auto temp2 = temp0;
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
        return temp1;
    }
    std::abort();
}

template<typename T1>
std::list<T1> rev3(const std::list<T1> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = rev3(xs);
        auto temp1 = std::list<T1>{x};
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::optional<std::list<T1>> testoption(const std::optional<T1> &arg1) {
    for (;;) {
        if (arg1.has_value()) {
            break;
        }
        return std::make_optional<std::list<T1>>(std::list<T1>());
    }
    for (;;) {
        if (!arg1.has_value()) {
            break;
        }
        auto x = arg1.value();
        return std::make_optional<std::list<T1>>(std::list<T1>{x});
    }
    std::abort();
}

template<typename T1>
std::set<T1> testset(const std::list<T1> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::set<T1>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = std::set<T1>{x};
        auto temp1 = testset(xs);
        decltype(temp0) temp2;
        for (auto temp3 : temp1) {
            if (temp0.count(temp3)) {
                temp2.insert(temp3);
            }
        }
        return temp2;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = std::set<T1>{x};
        auto temp1 = testset(xs);
        for (auto temp2 : temp1) {
            temp0.insert(temp2);
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> testifelse(const std::list<T1> &arg1, const T1 &arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        std::list<T1> temp0;
        if ((x) < (arg2)) {
            temp0 = std::list<T1>{x};
        } else {
            temp0 = std::list<T1>();
        }
        auto temp1 = temp0;
        auto temp2 = testifelse(xs, arg2);
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
        return temp1;
    }
    std::abort();
}

template<typename T1, typename T2>
std::pair<T1, T2> pair(const T1 &arg1, const T2 &arg2) {
    for (;;) {
        return std::make_pair(arg1, arg2);
    }
    std::abort();
}

template<typename T1, typename T2>
T1 fst(const std::pair<T1, T2> &arg1) {
    for (;;) {
        auto first = arg1.first;
        auto second = arg1.second;
        return first;
    }
    std::abort();
}

template<typename T1, typename T2>
T2 snd(const std::pair<T1, T2> &arg1) {
    for (;;) {
        auto first = arg1.first;
        auto second = arg1.second;
        return second;
    }
    std::abort();
}

bool evn(const std::uint64_t &arg1) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return true;
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        if ((arg1) - 1 != 0) {
            break;
        }
        return false;
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        if ((arg1) - 1 == 0) {
            break;
        }
        auto n = ((arg1) - 1) - 1;
        return evn(n);
    }
    std::abort();
}

std::uint64_t fib(const std::uint64_t &arg1) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return 1;
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        if ((arg1) - 1 != 0) {
            break;
        }
        return 1;
    }
    for (;;) {
        return (fib((arg1) - (1))) + (fib((arg1) - (2)));
    }
    std::abort();
}

