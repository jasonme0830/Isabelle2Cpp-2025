#include "datatypes.hpp"

snat add(const snat &arg1, const snat &arg2) {
    for (;;) {
        if (arg1->cons != snatCons::sZero) {
            break;
        }
        return arg2;
    }
    for (;;) {
        if (arg1->cons != snatCons::sSucc) {
            break;
        }
        auto m = arg1->get_c2().p1;
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(add(m, arg2));
        return temp0;
    }
    std::abort();
}

snat fib(const snat &arg1) {
    for (;;) {
        if (arg1->cons != snatCons::sZero) {
            break;
        }
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(std::make_shared<snatElem>(snatCons::sZero));
        return temp0;
    }
    for (;;) {
        if (arg1->cons != snatCons::sSucc) {
            break;
        }
        if (arg1->get_c2().p1->cons != snatCons::sZero) {
            break;
        }
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(std::make_shared<snatElem>(snatCons::sZero));
        return temp0;
    }
    for (;;) {
        if (arg1->cons != snatCons::sSucc) {
            break;
        }
        if (arg1->get_c2().p1->cons != snatCons::sSucc) {
            break;
        }
        auto m = arg1->get_c2().p1->get_c2().p1;
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(m);
        return add(fib(temp0), fib(m));
    }
    std::abort();
}

std::uint64_t natofsnat(const snat &arg1) {
    for (;;) {
        if (arg1->cons != snatCons::sZero) {
            break;
        }
        return 0;
    }
    for (;;) {
        if (arg1->cons != snatCons::sSucc) {
            break;
        }
        auto n = arg1->get_c2().p1;
        return (natofsnat(n)) + (1);
    }
    std::abort();
}

snat snatofnat(const std::uint64_t &arg1) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return std::make_shared<snatElem>(snatCons::sZero);
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto n = (arg1) - 1;
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(snatofnat(n));
        return temp0;
    }
    std::abort();
}

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return arg2;
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        temp0->set_c2(x, app(xs, arg2));
        return temp0;
    }
    std::abort();
}

template<typename T1>
slist<T1> rev(const slist<T1> &arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        temp0->set_c2(x, std::make_shared<slistElem<T1>>(slistCons::sNil));
        return app(rev(xs), temp0);
    }
    std::abort();
}

slist<snat> snat2slist(const snat &arg1) {
    for (;;) {
        slist<snat> temp0 = std::make_shared<slistElem<snat>>(slistCons::sCons);
        temp0->set_c2(arg1, std::make_shared<slistElem<snat>>(slistCons::sNil));
        return temp0;
    }
    std::abort();
}

template<typename T1>
snat len(const slist<T1> &arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return std::make_shared<snatElem>(snatCons::sZero);
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        snat temp0 = std::make_shared<snatElem>(snatCons::sSucc);
        temp0->set_c2(len(xs));
        return temp0;
    }
    std::abort();
}

slist<std::uint64_t> listwithlen(const std::uint64_t &arg1) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return std::make_shared<slistElem<std::uint64_t>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto n = (arg1) - 1;
        slist<std::uint64_t> temp0 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
        temp0->set_c2(1, listwithlen(n));
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::uint64_t leninnat(const slist<T1> &arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return 0;
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        return (leninnat(xs)) + (1);
    }
    std::abort();
}

template<typename T1>
slist<T1> dblist(const slist<T1> &arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        if (arg1->get_c2().p2->cons != slistCons::sNil) {
            break;
        }
        slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        slist<T1> temp1 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        temp1->set_c2(x, std::make_shared<slistElem<T1>>(slistCons::sNil));
        temp0->set_c2(x, temp1);
        return temp0;
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        temp0->set_c2(x, std::make_shared<slistElem<T1>>(slistCons::sNil));
        return app(dblist(temp0), dblist(xs));
    }
    std::abort();
}

sbool snot(const sbool &arg1) {
    for (;;) {
        if (arg1.cons != sboolCons::sTrue) {
            break;
        }
        return sbool(sboolCons::sFalse);
    }
    for (;;) {
        return sbool(sboolCons::sTrue);
    }
    std::abort();
}

