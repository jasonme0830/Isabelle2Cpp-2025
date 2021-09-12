#include "test.hpp"

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

