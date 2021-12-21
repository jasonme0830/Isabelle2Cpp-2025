#include <cstdint>
#include <cstdlib>
#include <list>
#include <optional>
#include <set>
#include <utility>

std::uint64_t add(const std::uint64_t &arg1, const std::uint64_t &arg2);

template<typename T1>
std::list<T1> app(const std::list<T1> &arg1, const std::list<T1> &arg2) {
    // app Nil ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // app (Cons x xs) ys = Cons x (app xs ys)
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = app(xs, arg2);
        temp0.push_front(x);
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::list<T1> rev(const std::list<T1> &arg1) {
    // rev Nil = Nil
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // rev (Cons x xs) = app (rev xs) (Cons x Nil)
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        return app(rev(xs), std::list<decltype(x)>{x});
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::list<T1> rev2(const std::list<T1> &arg1) {
    // rev2 Nil = Nil
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // rev2 (x # xs) = (rev2 xs) @ (x # Nil)
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = std::list<T1>();
        temp0.push_front(x);
        auto temp1 = rev2(xs);
        auto temp2 = temp0;
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
        return temp1;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::list<T1> rev3(const std::list<T1> &arg1) {
    // rev3 [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // rev3 (x # xs) = (rev3 xs) @ [x]
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = rev3(xs);
        auto temp1 = std::list<T1>{x};
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::optional<std::list<T1>> testoption(const std::optional<T1> &arg1) {
    // testoption None = Some Nil
    if (!arg1.has_value()) {
        return std::make_optional<std::list<T1>>(std::list<T1>());
    }

    // testoption (Some x) = Some [x]
    if (arg1.has_value()) {
        auto x = arg1.value();
        return std::make_optional<std::list<T1>>(std::list<T1>{x});
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::set<T1> testset(const std::list<T1> &arg1) {
    // testset Nil = {}
    if (arg1.empty()) {
        return std::set<T1>();
    }

    // testset (x # xs) = {x} \<inter> testset(xs)
    if (!arg1.empty()) {
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

    // testset (x # xs) = {x} \<union> testset(xs)
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        auto temp0 = std::set<T1>{x};
        auto temp1 = testset(xs);
        for (auto temp2 : temp1) {
            temp0.insert(temp2);
        }
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::list<T1> testifelse(const std::list<T1> &arg1, const T1 &arg2) {
    // testifelse [] n = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // testifelse (x # xs) n = (if (x < n) then [x] else []) @ (testifelse xs n)
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        std::list<T1> temp0;
        if (x < arg2) {
            temp0 = std::list<T1>{x};
        } else {
            temp0 = std::list<T1>();
        }
        auto temp1 = temp0;
        auto temp2 = testifelse(xs, arg2);
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
        return temp1;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1, typename T2>
std::pair<T1, T2> pair(const T1 &arg1, const T2 &arg2) {
    // pair f s = (f, s)
    return std::make_pair(arg1, arg2);
}

template<typename T1, typename T2>
T1 fst(const std::pair<T1, T2> &arg1) {
    // fst (first, second) = first
    auto first = arg1.first;
    return first;
}

template<typename T1, typename T2>
T2 snd(const std::pair<T1, T2> &arg1) {
    // snd (first, second) = second
    auto second = arg1.second;
    return second;
}

bool evn(const std::uint64_t &arg1);

std::uint64_t fib(const std::uint64_t &arg1);

bool altrue(const std::uint64_t &arg1);

