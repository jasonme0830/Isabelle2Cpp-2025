#include "datatypes.hpp"

snat add(const snat &arg1, const snat &arg2) {
    // add sZero n = n
    if (arg1.is_sZero()) {
        return arg2;
    }

    // add (sSucc m) n = sSucc (add m n)
    if (arg1.is_sSucc()) {
        auto &&m = arg1.as_sSucc().p1();
        auto temp0 = snat::sSucc(
            add(m, arg2)
        );
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

snat fib(const snat &arg1) {
    // fib sZero = sSucc sZero
    if (arg1.is_sZero()) {
        auto temp0 = snat::sSucc(
            snat::sZero()
        );
        return temp0;
    }

    // fib (sSucc sZero) = sSucc sZero
    if (arg1.is_sSucc()) {
        if (arg1.as_sSucc().p1().is_sZero()) {
            auto temp0 = snat::sSucc(
                snat::sZero()
            );
            return temp0;
        }
    }

    // fib (sSucc (sSucc m)) = add (fib (sSucc m)) (fib m)
    if (arg1.is_sSucc()) {
        if (arg1.as_sSucc().p1().is_sSucc()) {
            auto &&m = arg1.as_sSucc().p1().as_sSucc().p1();
            auto temp0 = snat::sSucc(
                m
            );
            return add(fib(temp0), fib(m));
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

std::uint64_t natofsnat(const snat &arg1) {
    // natofsnat sZero = 0
    if (arg1.is_sZero()) {
        return 0;
    }

    // natofsnat (sSucc n) = (natofsnat n) + 1
    if (arg1.is_sSucc()) {
        auto &&n = arg1.as_sSucc().p1();
        return natofsnat(n) + 1;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

snat snatofnat(const std::uint64_t &arg1) {
    // snatofnat 0 = sZero
    if (arg1 == 0) {
        return snat::sZero();
    }

    // snatofnat (Suc n) = sSucc (snatofnat n)
    if (arg1 != 0) {
        auto &&n = arg1 - 1;
        auto temp0 = snat::sSucc(
            snatofnat(n)
        );
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2) {
    // app sNil ys = ys
    if (arg1.is_sNil()) {
        return arg2;
    }

    // app (sCons x xs) ys = sCons x (app xs ys)
    if (arg1.is_sCons()) {
        auto &&x = arg1.as_sCons().p1();
        auto &&xs = arg1.as_sCons().p2();
        auto temp0 = slist<T1>::sCons(
            x, app(xs, arg2)
        );
        return temp0;
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
        auto &&x = arg1.as_sCons().p1();
        auto &&xs = arg1.as_sCons().p2();
        return app(rev(xs), sCons(x, ::sNil()));
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<snat> snat2slist(const snat &arg1) {
    // snat2slist n = sCons n sNil
    auto temp0 = slist<snat>::sCons(
        arg1, slist<snat>::sNil()
    );
    return temp0;
}

template<typename T1>
snat len(const slist<T1> &arg1) {
    // len sNil = sZero
    if (arg1.is_sNil()) {
        return snat::sZero();
    }

    // len (sCons x xs) = sSucc (len xs)
    if (arg1.is_sCons()) {
        auto &&xs = arg1.as_sCons().p2();
        auto temp0 = snat::sSucc(
            len(xs)
        );
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<std::uint64_t> listwithlen(const std::uint64_t &arg1) {
    // listwithlen 0 = sNil
    if (arg1 == 0) {
        return slist<std::uint64_t>::sNil();
    }

    // listwithlen (Suc n) = sCons 1 (listwithlen n)
    if (arg1 != 0) {
        auto &&n = arg1 - 1;
        auto temp0 = slist<std::uint64_t>::sCons(
            1, listwithlen(n)
        );
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::uint64_t leninnat(const slist<T1> &arg1) {
    // leninnat sNil = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // leninnat (sCons x xs) = (leninnat xs) + 1
    if (arg1.is_sCons()) {
        auto &&xs = arg1.as_sCons().p2();
        return leninnat(xs) + 1;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> dblist(const slist<T1> &arg1) {
    // dblist sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // dblist (sCons x sNil) = sCons x (sCons x sNil)
    if (arg1.is_sCons()) {
        auto &&x = arg1.as_sCons().p1();
        if (arg1.as_sCons().p2().is_sNil()) {
            auto temp0 = slist<T1>::sCons(
                x, slist<T1>::sNil()
            );
            auto temp1 = slist<T1>::sCons(
                x, temp0
            );
            return temp1;
        }
    }

    // dblist (sCons x xs) = app (dblist (sCons x sNil)) (dblist xs)
    if (arg1.is_sCons()) {
        auto &&x = arg1.as_sCons().p1();
        auto &&xs = arg1.as_sCons().p2();
        auto temp0 = slist<T1>::sCons(
            x, slist<T1>::sNil()
        );
        return app(dblist(temp0), dblist(xs));
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

sbool snot(const sbool &arg1) {
    // snot sTrue = sFalse
    if (arg1.is_sTrue()) {
        return sbool::sFalse();
    }

    // snot sFlase = sTrue
    return sbool::sTrue();
}

