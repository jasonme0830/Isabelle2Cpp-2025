#include <cstdint>
#include <list>
#include <optional>
#include <set>
#include <utility>

std::uint64_t
add(std::uint64_t arg0, std::uint64_t arg1) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        auto n = arg2;
        return n;
    }
    for (;;) {
        auto m = (arg1) - 1;
        auto n = arg2;
        return (add(m, n)) + 1;
    }
}

template<typename T0>
std::list<T0>
app(std::list<T0> arg0, std::list<T0> arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        auto ys = arg2;
        return ys;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto ys = arg2;
        auto temp0 = app(xs, ys);
        temp0.push_front(x);
        return temp0;
    }
}

template<typename T0>
std::list<T0>
rev(std::list<T0> arg0) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T0>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::list<decltype(x)>{x};
        return app(rev(xs), temp0);
    }
}

template<typename T0>
std::list<T0>
rev2(std::list<T0> arg0) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T0>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::list<T0>();
        temp0.push_front(x);
        auto temp1 = rev(xs);
        temp1.insert(temp1.end(), temp0.begin(), temp0.end());
        return temp1;
    }
}

template<typename T0>
std::list<T0>
rev3(std::list<T0> arg0) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T0>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = rev(xs);
        temp0.insert(temp0.end(), std::list<T0>{x}.begin(), std::list<T0>{x}.end());
        return temp0;
    }
}

template<typename T0>
std::optional<std::list<T0>>
testoption(std::optional<T0> arg0) {
    for (;;) {
        if (arg1.has_value()) {
            break;
        }
        return std::make_optional<>({});
    }
    for (;;) {
        if (!arg1.has_value()) {
            break;
        }
        auto x = arg1.value();
        return std::make_optional<>(std::list<decltype(x)>{x});
    }
}

template<typename T0>
std::set<T0>
testset(std::list<T0> arg0) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::set<T0>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::set<T0>{x};
        auto temp1 = testset(xs);
        decltype(temp0) temp2;
        for (auto temp3 : temp1) {
            if (lv.count(temp3)) {
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
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::set<T0>{x};
        auto temp1 = testset(xs);
        for (auto temp2 : temp1) {
            temp0.insert(temp2);
        }
        return temp0;
    }
}

template<typename T0>
std::list<T0>
testifelse(std::list<T0> arg0, T0 arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        auto n = arg2;
        return std::list<T0>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto n = arg2;
        std::list<T0> temp0;
        if ((x) < (n)) {
            auto temp1 = testifelse(xs, n);
            temp1.push_front(x);
            temp0 = temp1;
        } else {
            temp0 = std::list<T0>();
        }
        return temp0;
    }
}

template<typename T0, typename T1>
std::pair<T0, T1>
pair(T0 arg0, T1 arg1) {
    for (;;) {
        auto f = arg1;
        auto s = arg2;
        return std::make_pair(f, s);
    }
}

template<typename T0, typename T1>
T0
fst(std::pair<T0, T1> arg0) {
    for (;;) {
        auto first = arg1.first;
        auto second = arg1.second;
        return first;
    }
}

template<typename T0, typename T1>
T1
snd(std::pair<T0, T1> arg0) {
    for (;;) {
        auto first = arg1.first;
        auto second = arg1.second;
        return second;
    }
}

