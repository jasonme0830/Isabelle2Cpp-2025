#include <cstdint>
#include <cstdlib>
#include <list>
#include <optional>
#include <set>
#include <utility>

std::uint64_t
add(std::uint64_t arg0, std::uint64_t arg1) {
    for (;;) {
        if (arg0 != 0) {
            break;
        }
        auto n = arg1;
        return n;
    }
    for (;;) {
        auto m = (arg0) - 1;
        auto n = arg1;
        return (add(m, n)) + 1;
    }
    std::abort();
}

template<typename T0>
std::list<T0>
app(std::list<T0> arg0, std::list<T0> arg1) {
    for (;;) {
        if (!arg0.empty()) {
            break;
        }
        auto ys = arg1;
        return ys;
    }
    for (;;) {
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
        auto ys = arg1;
        auto temp0 = app(xs, ys);
        temp0.push_front(x);
        return temp0;
    }
    std::abort();
}

template<typename T0>
std::list<T0>
rev(std::list<T0> arg0) {
    for (;;) {
        if (!arg0.empty()) {
            break;
        }
        return std::list<T0>();
    }
    for (;;) {
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
        return app(rev(xs), std::list<decltype(x)>{x});
    }
    std::abort();
}

template<typename T0>
std::list<T0>
rev2(std::list<T0> arg0) {
    for (;;) {
        if (!arg0.empty()) {
            break;
        }
        return std::list<T0>();
    }
    for (;;) {
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
        auto temp0 = std::list<T0>();
        temp0.push_front(x);
        auto temp1 = rev2(xs);
        auto temp2 = temp0;
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
        return temp1;
    }
    std::abort();
}

template<typename T0>
std::list<T0>
rev3(std::list<T0> arg0) {
    for (;;) {
        if (!arg0.empty()) {
            break;
        }
        return std::list<T0>();
    }
    for (;;) {
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
        auto temp0 = rev3(xs);
        auto temp1 = std::list<T0>{x};
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        return temp0;
    }
    std::abort();
}

template<typename T0>
std::optional<std::list<T0>>
testoption(std::optional<T0> arg0) {
    for (;;) {
        if (arg0.has_value()) {
            break;
        }
        return std::make_optional<>({});
    }
    for (;;) {
        if (!arg0.has_value()) {
            break;
        }
        auto x = arg0.value();
        return std::make_optional<>(std::list<decltype(x)>{x});
    }
    std::abort();
}

template<typename T0>
std::set<T0>
testset(std::list<T0> arg0) {
    for (;;) {
        if (!arg0.empty()) {
            break;
        }
        return std::set<T0>();
    }
    for (;;) {
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
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
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
        auto temp0 = std::set<T0>{x};
        auto temp1 = testset(xs);
        for (auto temp2 : temp1) {
            temp0.insert(temp2);
        }
        return temp0;
    }
    std::abort();
}

template<typename T0>
std::list<T0>
testifelse(std::list<T0> arg0, T0 arg1) {
    for (;;) {
        if (!arg0.empty()) {
            break;
        }
        auto n = arg1;
        return std::list<T0>();
    }
    for (;;) {
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
        auto n = arg1;
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
    std::abort();
}

template<typename T0, typename T1>
std::pair<T0, T1>
pair(T0 arg0, T1 arg1) {
    for (;;) {
        auto f = arg0;
        auto s = arg1;
        return std::make_pair(f, s);
    }
    std::abort();
}

template<typename T0, typename T1>
T0
fst(std::pair<T0, T1> arg0) {
    for (;;) {
        auto first = arg0.first;
        auto second = arg0.second;
        return first;
    }
    std::abort();
}

template<typename T0, typename T1>
T1
snd(std::pair<T0, T1> arg0) {
    for (;;) {
        auto first = arg0.first;
        auto second = arg0.second;
        return second;
    }
    std::abort();
}

bool
evn(std::uint64_t arg0) {
    for (;;) {
        if (arg0 != 0) {
            break;
        }
        return true;
    }
    for (;;) {
        if ((arg0) - 1 != 0) {
            break;
        }
        return false;
    }
    for (;;) {
        auto n = ((arg0) - 1) - 1;
        return evn(n);
    }
    std::abort();
}

