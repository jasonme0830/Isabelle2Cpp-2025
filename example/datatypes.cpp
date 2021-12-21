#include "datatypes.hpp"

snat add(const snat &arg1, const snat &arg2) {
    // add sZero n = n
    if (arg1.is_sZero()) {
        return arg2;
    }

    // add (sSucc m) n = sSucc (add m n)
    if (arg1.is_sSucc()) {
        auto m = arg1.as_sSucc().p1();
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
            auto m = arg1.as_sSucc().p1().as_sSucc().p1();
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
        auto n = arg1.as_sSucc().p1();
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
        auto n = arg1 - 1;
        auto temp0 = snat::sSucc(
            snatofnat(n)
        );
        return temp0;
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

slist<std::uint64_t> listwithlen(const std::uint64_t &arg1) {
    // listwithlen 0 = sNil
    if (arg1 == 0) {
        return slist<std::uint64_t>::sNil();
    }

    // listwithlen (Suc n) = sCons 1 (listwithlen n)
    if (arg1 != 0) {
        auto n = arg1 - 1;
        auto temp0 = slist<std::uint64_t>::sCons(
            1, listwithlen(n)
        );
        return temp0;
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

