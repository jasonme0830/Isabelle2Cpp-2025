#include <cstdint>
#include <cstdlib>
#include <memory>
#include <variant>

template<typename T, typename Cons> T construct(Cons cons) { return std::make_shared<typename T::element_type>(cons); }

enum snatCons {
    sZero,
    sSucc,
};

struct snatElem;
using snat = std::shared_ptr<snatElem>;

struct snatElem {
    struct c1 {
    };
    struct c2 {
        snat p1;
    };

    snatElem(snatCons cons) : cons(cons) {}

    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1() {
        value = c1{};
    }
    c2 &get_c2() {
        return std::get<c2>(value);
    };
    void set_c2(snat _p1) {
        value = c2{_p1};
    }

    snatCons cons;
    std::variant<c1, c2> value;
};

enum slistCons {
    sNil,
    sCons,
};

template<typename T1>
struct slistElem;
template<typename T1>
using slist = std::shared_ptr<slistElem<T1>>;

template<typename T1>
struct slistElem {
    struct c1 {
    };
    struct c2 {
        T1 p1;
        slist<T1> p2;
    };

    slistElem(slistCons cons) : cons(cons) {}

    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1() {
        value = c1{};
    }
    c2 &get_c2() {
        return std::get<c2>(value);
    };
    void set_c2(T1 _p1, slist<T1> _p2) {
        value = c2{_p1, _p2};
    }

    slistCons cons;
    std::variant<c1, c2> value;
};

enum sboolCons {
    sTrue,
    sFalse,
};

struct sbool {
    struct c1 {
    };
    struct c2 {
    };

    sbool(sboolCons cons) : cons(cons) {}

    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1() {
        value = c1{};
    }
    c2 &get_c2() {
        return std::get<c2>(value);
    };
    void set_c2() {
        value = c2{};
    }

    sboolCons cons;
    std::variant<c1, c2> value;
};

enum optionCons {
    None,
    Some,
};

template<typename T1>
struct option {
    struct c1 {
    };
    struct c2 {
        T1 p1;
    };

    option(optionCons cons) : cons(cons) {}

    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1() {
        value = c1{};
    }
    c2 &get_c2() {
        return std::get<c2>(value);
    };
    void set_c2(T1 _p1) {
        value = c2{_p1};
    }

    optionCons cons;
    std::variant<c1, c2> value;
};

/**
 * app xs ys = app (app1 xs ys) (app2 xs ys)
*/

template<typename T1>
slist<T1> app(slist<T1> arg1, slist<T1> arg2) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        auto ys = arg2;
        return ys;
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        auto ys = arg2;
        slist<T1> temp0 = construct<slist<T1>>(slistCons::sCons);
        temp0->set_c2(x, app(xs, ys));
        return temp0;
    }
    std::abort();
}

template<typename T1>
slist<T1> rev(slist<T1> arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return construct<slist<T1>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        slist<T1> temp0 = construct<slist<T1>>(slistCons::sCons);
        temp0->set_c2(x, construct<slist<T1>>(slistCons::sNil));
        return app(rev(xs), temp0);
    }
    std::abort();
}

slist<snat> snat2slist(snat arg1) {
    for (;;) {
        auto n = arg1;
        slist<snat> temp0 = construct<slist<snat>>(slistCons::sCons);
        temp0->set_c2(n, construct<slist<snat>>(slistCons::sNil));
        return temp0;
    }
    std::abort();
}

template<typename T1>
snat len(slist<T1> arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return construct<snat>(snatCons::sZero);
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        snat temp0 = construct<snat>(snatCons::sSucc);
        temp0->set_c2(len(xs));
        return temp0;
    }
    std::abort();
}

slist<std::uint64_t> listwithlen(std::uint64_t arg1) {
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return construct<slist<std::uint64_t>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto n = (arg1) - 1;
        slist<std::uint64_t> temp0 = construct<slist<std::uint64_t>>(slistCons::sCons);
        temp0->set_c2(1, listwithlen(n));
        return temp0;
    }
    std::abort();
}

template<typename T1>
std::uint64_t leninnat(slist<T1> arg1) {
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
slist<T1> dblist(slist<T1> arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return construct<slist<T1>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        if (arg1->get_c2().p2->cons != slistCons::sNil) {
            break;
        }
        slist<T1> temp0 = construct<slist<T1>>(slistCons::sCons);
        slist<T1> temp1 = construct<slist<T1>>(slistCons::sCons);
        temp1->set_c2(x, construct<slist<T1>>(slistCons::sNil));
        temp0->set_c2(x, temp1);
        return temp0;
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        slist<T1> temp0 = construct<slist<T1>>(slistCons::sCons);
        temp0->set_c2(x, construct<slist<T1>>(slistCons::sNil));
        return app(dblist(temp0), dblist(xs));
    }
    std::abort();
}

sbool snot(sbool arg1) {
    for (;;) {
        if (arg1.cons != sboolCons::sTrue) {
            break;
        }
        return sbool(sboolCons::sFalse);
    }
    for (;;) {
        auto sFlase = arg1;
        return sbool(sboolCons::sTrue);
    }
    std::abort();
}

