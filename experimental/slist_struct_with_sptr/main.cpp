#include "slist.hpp"
#include <iostream>

using T1 = int;
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2) {
    // app sNil ys = ys
    if (arg1.is_sNil()) {
        return arg2;
    }

    // app (sCons x xs) ys = sCons x (app xs ys)
    if (arg1.is_sCons()) {
        auto &&x = arg1.as_sCons().p1();
        auto &&xs = arg1.as_sCons().p2();
        return slist<T1>::sCons(x, app(xs, arg2));
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> rev(const slist<T1> &arg1) {
    // rev sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // rev (sCons x xs) = app (rev xs) (sCons x sNil)
    if (arg1.is_sCons()) {
        auto x = arg1.as_sCons().p1();
        auto xs = arg1.as_sCons().p2();
        return app(rev(xs), slist<T1>::sCons(x, slist<T1>::sNil()));
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

int main() {
    auto l = slist<int>::sCons(1, slist<int>::sCons(2, slist<int>::sNil()));
    std::cout << l.as_sCons().p1() << std::endl;
    auto rl = rev(l);
    std::cout << rl.as_sCons().p1() << std::endl;
}