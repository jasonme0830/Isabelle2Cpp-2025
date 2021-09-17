#include "List_raw.hpp"

template<typename T1>
T1 last(const std::list<T1> &arg1) {
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        T1 temp0;
        if ((xs) == (bool())) {
            temp0 = x;
        } else {
            temp0 = last(xs);
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> butlast(const std::list<T1> &arg1) {
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
        if ((xs) == (bool())) {
            temp0 = std::list<T1>();
        } else {
            auto temp1 = butlast(xs);
            temp1.push_front(x);
            temp0 = temp1;
        }
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
        auto temp0 = rev(xs);
        auto temp1 = std::list<T1>{x};
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> filter(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto x = arg2.front();
        auto xs = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        std::list<T1> temp0;
        if (P(x)) {
            auto temp1 = filter(arg1, xs);
            temp1.push_front(x);
            temp0 = temp1;
        } else {
            temp0 = filter(arg1, xs);
        }
        return temp0;
    }
    std::abort();
}

template<typename T1, typename T2>
T1 foldl(const std::function<T1(T1, T2)> &arg1, const T1 &arg2, const std::list<T2> &arg3) {
    for (;;) {
        if (!arg3.empty()) {
            break;
        }
        return arg2;
    }
    for (;;) {
        if (arg3.empty()) {
            break;
        }
        auto x = arg3.front();
        auto xs = decltype(arg3){std::next(arg3.begin()), arg3.end()};
        return foldl(arg1, f(arg2, x), xs);
    }
    std::abort();
}

template<typename T1>
std::list<T1> concat(const std::list<std::list<T1>> &arg1) {
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
        auto temp0 = x;
        auto temp1 = concat(xs);
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> takeWhile(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto x = arg2.front();
        auto xs = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        std::list<T1> temp0;
        if (P(x)) {
            auto temp1 = takeWhile(arg1, xs);
            temp1.push_front(x);
            temp0 = temp1;
        } else {
            temp0 = std::list<T1>();
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> dropWhile(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto x = arg2.front();
        auto xs = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        std::list<T1> temp0;
        if (P(x)) {
            temp0 = dropWhile(arg1, xs);
        } else {
            auto temp1 = xs;
            temp1.push_front(x);
            temp0 = temp1;
        }
        return temp0;
    }
    std::abort();
}

template<typename T1, typename T2>
std::list<std::pair<T1, T2>> product(const std::list<T1> &arg1, const std::list<T2> &arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<std::pair<T1, T2>>();
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
    }
    std::abort();
}

template<typename T1>
std::list<T1> insert(const T1 &arg1, const std::list<T1> &arg2) {
    for (;;) {
        std::list<T1> temp0;
        if (set(arg2).count(arg1)) {
            temp0 = arg2;
        } else {
            auto temp1 = arg2;
            temp1.push_front(arg1);
            temp0 = temp1;
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> union(const std::list<T1> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        auto union = ;
        return fold(insert);
    }
    std::abort();
}

template<typename T1>
std::optional<T1> find(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return std::optional<T1>();
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto x = arg2.front();
        auto xs = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        std::optional<T1> temp0;
        if (P(x)) {
            temp0 = std::make_optional<T1>(x);
        } else {
            temp0 = find(arg1, xs);
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::uint64_t count_list(const std::list<T1> &arg1, const T1 &arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return 0;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        std::uint64_t temp0;
        if ((x) == (arg2)) {
            temp0 = (count_list(xs, arg2)) + (1);
        } else {
            temp0 = count_list(xs, arg2);
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> remove1(const T1 &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto y = arg2.front();
        auto xs = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        std::list<T1> temp0;
        if ((arg1) == (y)) {
            temp0 = xs;
        } else {
            auto temp1 = remove1(arg1, xs);
            temp1.push_front(y);
            temp0 = temp1;
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> removeAll(const T1 &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto y = arg2.front();
        auto xs = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        std::list<T1> temp0;
        if ((arg1) == (y)) {
            temp0 = removeAll(arg1, xs);
        } else {
            auto temp1 = removeAll(arg1, xs);
            temp1.push_front(y);
            temp0 = temp1;
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
bool distinct(const std::list<T1> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return true;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        return (!set(xs).count(x)) && (distinct(xs));
    }
    std::abort();
}

template<typename T1>
std::list<T1> remdups(const std::list<T1> &arg1) {
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
        if (set(xs).count(x)) {
            temp0 = remdups(xs);
        } else {
            auto temp1 = remdups(xs);
            temp1.push_front(x);
            temp0 = temp1;
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> remdups_adj(const std::list<T1> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg1.size() != 1) {
            break;
        }
        auto x = *std::next(arg1.begin(), 0);
        return std::list<T1>{x};
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        if (decltype(arg1){std::next(arg1.begin()), arg1.end()}.empty()) {
            break;
        }
        auto y = decltype(arg1){std::next(arg1.begin()), arg1.end()}.front();
        auto xs = decltype(decltype(arg1){std::next(arg1.begin()), arg1.end()}){std::next(decltype(arg1){std::next(arg1.begin()), arg1.end()}.begin()), decltype(arg1){std::next(arg1.begin()), arg1.end()}.end()};
        std::list<T1> temp0;
        if ((x) == (y)) {
            auto temp1 = xs;
            temp1.push_front(x);
            temp0 = remdups_adj(temp1);
        } else {
            auto temp2 = xs;
            temp2.push_front(y);
            auto temp3 = remdups_adj(temp2);
            temp3.push_front(x);
            temp0 = temp3;
        }
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> replicate(const std::uint64_t &arg1, const T1 &arg2) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return std::list<T1>();
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto n = (arg1) - 1;
        auto temp0 = replicate(n, arg2);
        temp0.push_front(arg2);
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<T1> rotate1(const std::list<T1> &arg1) {
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
        auto temp0 = xs;
        auto temp1 = std::list<T1>{x};
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::list<std::list<T1>> subseqs(const std::list<T1> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<std::list<T1>>{std::list<std::list<T1>>()};
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
    }
    std::abort();
}

template<typename T1>
std::list<T1> splice(const std::list<T1> &arg1, const std::list<T1> &arg2) {
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
        auto temp0 = splice(arg2, xs);
        temp0.push_front(x);
        return temp0;
    }
    std::abort();
}

template<typename T1, typename T2>
std::list<T1> insort_key(const std::function<T2(T1)> &arg1, const T1 &arg2, const std::list<T1> &arg3) {
    for (;;) {
        if (!arg3.empty()) {
            break;
        }
        return std::list<T1>{arg2};
    }
    for (;;) {
        if (arg3.empty()) {
            break;
        }
        auto y = arg3.front();
        auto ys = decltype(arg3){std::next(arg3.begin()), arg3.end()};
        std::list<T1> temp0;
        if ((f(arg2)) <= (f(y))) {
            auto temp1 = ys;
            temp1.push_front(y);
            auto temp2 = temp1;
            temp2.push_front(arg2);
            temp0 = temp2;
        } else {
            auto temp3 = insort_key(arg1, arg2, ys);
            temp3.push_front(y);
            temp0 = temp3;
        }
        return temp0;
    }
    std::abort();
}

template<typename T1, typename T2>
std::list<T1> sort_key(const std::function<T2(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
    }
    std::abort();
}

template<typename T1>
std::pair<std::list<T1>, std::list<T1>> partition(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return std::make_pair(std::list<T1>(), std::list<T1>());
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto x = arg2.front();
        auto xs = decltype(arg2){std::next(arg2.begin()), arg2.end()};
    }
    std::abort();
}

template<typename T1>
std::set<std::list<T1>> listset(const std::list<std::set<T1>> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::set<std::list<T1>>{std::set<std::list<T1>>()};
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto A = arg1.front();
        auto As = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        return set_Cons(A, listset(As));
    }
    std::abort();
}

template<typename T1, typename T2>
std::list<T2> bind(const std::list<T1> &arg1, const std::function<std::list<T2>(T1)> &arg2) {
    for (;;) {
        return concat(map(arg2, arg1));
    }
    std::abort();
}

template<typename T1, typename T2>
std::list<T2> map_tailrec_rev(const std::function<T2(T1)> &arg1, const std::list<T1> &arg2, const std::list<T2> &arg3) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return arg3;
    }
    for (;;) {
        if (arg2.empty()) {
            break;
        }
        auto a = arg2.front();
        auto as = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        auto temp0 = arg3;
        temp0.push_front(f(a));
        return map_tailrec_rev(arg1, as, temp0);
    }
    std::abort();
}

template<typename T1, typename T2>
std::list<T2> map_tailrec(const std::function<T2(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        return rev(map_tailrec_rev(arg1, arg2, std::list<T2>()));
    }
    std::abort();
}

template<typename T1>
bool member(const std::list<T1> &arg1, const T1 &arg2) {
    for (;;) {
        return set(arg1).count(arg2);
    }
    std::abort();
}

template<typename T1>
bool null(const std::list<T1> &arg1) {
    for (;;) {
        return (arg1) == (bool());
    }
    std::abort();
}

template<typename T1, typename T2>
std::list<T2> maps(const std::function<std::list<T2>(T1)> &arg1, const std::list<T1> &arg2) {
    for (;;) {
        return concat(map(arg1, arg2));
    }
    std::abort();
}

template<typename T1>
std::uint64_t gen_length(const std::uint64_t &arg1, const std::list<T1> &arg2) {
    for (;;) {
        return (arg1) + (arg2.size());
    }
    std::abort();
}

