#include <cstdint>
#include <cstdlib>
#include <list>
#include <optional>
#include <set>
#include <utility>

template<typename T, typename Cons> T construct(Cons cons) { return std::make_shared<typename T::element_type>(cons); }

std::uint64_t add(std::uint64_t arg1, std::uint64_t arg2) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        auto n = arg2;
        return n;
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto m = (arg1) - 1;
        auto n = arg2;
        return (add(m, n)) + 1;
    }
    std::abort();
}

template<typename T1>
std::list<T1> app(std::list<T1> arg1, std::list<T1> arg2) {
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
    std::abort();
}

template<typename T1>
std::list<T1> rev(std::list<T1> arg1) {
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
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::list<T1>();
        temp0.push_front(x);
        return app(rev(xs), temp0);
    }
    std::abort();
}

template<typename T1>
std::list<T1> rev2(std::list<T1> arg1) {
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
        arg1.pop_front();
        auto xs = arg1;
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
std::list<T1> rev3(std::list<T1> arg1) {
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
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = rev3(xs);
        auto temp1 = std::list<T1>{x};
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::optional<std::list<T1>> testoption(std::optional<T1> arg1) {
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
std::set<T1> testset(std::list<T1> arg1) {
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
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::set<T1>{x};
        auto temp1 = testset(xs);
        decltype(temp0) temp2;
        for (auto temp3 : temp1) {;
        if (lv.count(temp3)) {;
        temp2.insert(temp3);
        };
        };
        return temp2;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::set<T1>{x};
        auto temp1 = testset(xs);
        for (auto temp2 : temp1) {;
        temp0.insert(temp2);
        };
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> testifelse(std::list<T1> arg1, T1 arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        auto n = arg2;
        return std::list<T1>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto n = arg2;
        std::list<T1> temp0;
        if ((x) < (n)) {;
        temp0 = std::list<T1>{x};
        } else {;
        temp0 = std::list<T1>();
        };
        auto temp1 = temp0;
        auto temp2 = testifelse(xs, n);
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
        return temp1;
    }
    std::abort();
}

template<typename T1, typename T2>
std::pair<T1, T2> pair(T1 arg1, T2 arg2) {
    for (;;) {
        auto f = arg1;
        auto s = arg2;
        return std::make_pair(f, s);
    }
    std::abort();
}

template<typename T1, typename T2>
T1 fst(std::pair<T1, T2> arg1) {
    for (;;) {
        auto first = arg1.first;
        auto second = arg1.second;
        return first;
    }
    std::abort();
}

template<typename T1, typename T2>
T2 snd(std::pair<T1, T2> arg1) {
    for (;;) {
        auto first = arg1.first;
        auto second = arg1.second;
        return second;
    }
    std::abort();
}

bool evn(std::uint64_t arg1) {
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

