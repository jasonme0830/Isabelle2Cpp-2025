#include "datatypes.hpp"

snat add(const snat &arg1, const snat &arg2) {
    // add sZero n = n
    if (arg1->cons == snatCons::sZero) {
        return arg2;
    }

    // add (sSucc m) n = sSucc (add m n)
    if (arg1->cons == snatCons::sSucc) {
        auto m = arg1->get_c2().p1;
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(add(m, arg2));
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

snat fib(const snat &arg1) {
    // fib sZero = sSucc sZero
    if (arg1->cons == snatCons::sZero) {
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(std::make_shared<snatElem>(snatCons::sZero));
        return temp0;
    }

    // fib (sSucc sZero) = sSucc sZero
    if (arg1->cons == snatCons::sSucc) {
        if (arg1->get_c2().p1->cons == snatCons::sZero) {
            snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
            temp0->set_c2(std::make_shared<snatElem>(snatCons::sZero));
            return temp0;
        }
    }

    // fib (sSucc (sSucc m)) = add (fib (sSucc m)) (fib m)
    if (arg1->cons == snatCons::sSucc) {
        if (arg1->get_c2().p1->cons == snatCons::sSucc) {
            auto m = arg1->get_c2().p1->get_c2().p1;
            snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
            temp0->set_c2(m);
            return add(fib(temp0), fib(m));
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

std::uint64_t natofsnat(const snat &arg1) {
    // natofsnat sZero = 0
    if (arg1->cons == snatCons::sZero) {
        return 0;
    }

    // natofsnat (sSucc n) = (natofsnat n) + 1
    if (arg1->cons == snatCons::sSucc) {
        auto n = arg1->get_c2().p1;
        return (natofsnat(n)) + (1);
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

snat snatofnat(const std::uint64_t &arg1) {
    // snatofnat 0 = sZero
    if (arg1 == 0) {
        return std::make_shared<snatElem>(snatCons::sZero);
    }

    // snatofnat (Suc n) = sSucc (snatofnat n)
    if (arg1 != 0) {
        auto n = (arg1) - 1;
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(snatofnat(n));
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2) {
    // app sNil ys = ys
    if (arg1->cons == slistCons::sNil) {
        return arg2;
    }

    // app (sCons x xs) ys = sCons x (app xs ys)
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        temp0->set_c2(x, app(xs, arg2));
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> rev(const slist<T1> &arg1) {
    // rev sNil = sNil
    if (arg1->cons == slistCons::sNil) {
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }

    // rev (sCons x xs) = app (rev xs) (sCons x sNil)
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        return app(rev(xs), sCons(x, std::make_shared<Elem>(slistCons::sNil)));
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<snat> snat2slist(const snat &arg1) {
    // snat2slist n = sCons n sNil
    slist<snat> temp0 = std::make_shared<slistElem<snat>>(slistCons::sCons);
    temp0->set_c2(arg1, std::make_shared<slistElem<snat>>(slistCons::sNil));
    return temp0;
}

template<typename T1>
snat len(const slist<T1> &arg1) {
    // len sNil = sZero
    if (arg1->cons == slistCons::sNil) {
        return std::make_shared<snatElem>(snatCons::sZero);
    }

    // len (sCons x xs) = sSucc (len xs)
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(len(xs));
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<std::uint64_t> listwithlen(const std::uint64_t &arg1) {
    // listwithlen 0 = sNil
    if (arg1 == 0) {
        return std::make_shared<slistElem<std::uint64_t>>(slistCons::sNil);
    }

    // listwithlen (Suc n) = sCons 1 (listwithlen n)
    if (arg1 != 0) {
        auto n = (arg1) - 1;
        slist<std::uint64_t> temp0 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
        temp0->set_c2(1, listwithlen(n));
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::uint64_t leninnat(const slist<T1> &arg1) {
    // leninnat sNil = 0
    if (arg1->cons == slistCons::sNil) {
        return 0;
    }

    // leninnat (sCons x xs) = (leninnat xs) + 1
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        return (leninnat(xs)) + (1);
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> dblist(const slist<T1> &arg1) {
    // dblist sNil = sNil
    if (arg1->cons == slistCons::sNil) {
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }

    // dblist (sCons x sNil) = sCons x (sCons x sNil)
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        if (arg1->get_c2().p2->cons == slistCons::sNil) {
            slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
            slist<T1> temp1 = std::make_shared<slistElem<T1>>(slistCons::sCons);
            temp1->set_c2(x, std::make_shared<slistElem<T1>>(slistCons::sNil));
            temp0->set_c2(x, temp1);
            return temp0;
        }
    }

    // dblist (sCons x xs) = app (dblist (sCons x sNil)) (dblist xs)
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        temp0->set_c2(x, std::make_shared<slistElem<T1>>(slistCons::sNil));
        return app(dblist(temp0), dblist(xs));
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

sbool snot(const sbool &arg1) {
    // snot sTrue = sFalse
    if (arg1.cons == sboolCons::sTrue) {
        return sbool(sboolCons::sFalse);
    }

    // snot sFlase = sTrue
    return sbool(sboolCons::sTrue);
}

