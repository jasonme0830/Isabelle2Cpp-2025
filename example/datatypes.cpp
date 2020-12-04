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
    struct c0 {
    };
    struct c1 {
        snat p0;
    };

    snatElem(snatCons cons) : cons(cons) {}

    c0 &get_c0() {
        return std::get<c0>(value);
    };
    void set_c0() {
        value = c0{};
    }
    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1(snat _p0) {
        value = c1{_p0};
    }

    snatCons cons;
    std::variant<c0, c1> value;
};

enum slistCons {
    sNil,
    sCons,
};

template<typename T0>
struct slistElem;
template<typename T0>
using slist = std::shared_ptr<slistElem<T0>>;

template<typename T0>
struct slistElem {
    struct c0 {
    };
    struct c1 {
        T0 p0;
        slist<T0> p1;
    };

    slistElem(slistCons cons) : cons(cons) {}

    c0 &get_c0() {
        return std::get<c0>(value);
    };
    void set_c0() {
        value = c0{};
    }
    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1(T0 _p0, slist<T0> _p1) {
        value = c1{_p0, _p1};
    }

    slistCons cons;
    std::variant<c0, c1> value;
};

enum sboolCons {
    sTrue,
    sFalse,
};

struct sbool {
    struct c0 {
    };
    struct c1 {
    };

    sbool(sboolCons cons) : cons(cons) {}

    c0 &get_c0() {
        return std::get<c0>(value);
    };
    void set_c0() {
        value = c0{};
    }
    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1() {
        value = c1{};
    }

    sboolCons cons;
    std::variant<c0, c1> value;
};

enum optionCons {
    None,
    Some,
};

template<typename T0>
struct option {
    struct c0 {
    };
    struct c1 {
        T0 p0;
    };

    option(optionCons cons) : cons(cons) {}

    c0 &get_c0() {
        return std::get<c0>(value);
    };
    void set_c0() {
        value = c0{};
    }
    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1(T0 _p0) {
        value = c1{_p0};
    }

    optionCons cons;
    std::variant<c0, c1> value;
};

template<typename T0>
slist<T0> app(slist<T0> arg0, slist<T0> arg1) {
    for (;;) {
        if (arg0->cons != slistCons::sNil) {
            break;
        }
        auto ys = arg1;
        return ys;
    }
    for (;;) {
        if (arg0->cons != slistCons::sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        auto ys = arg1;
        slist<T0> temp0 = construct<slist<T0>>(slistCons::sCons);
        temp0->set_c1(x, app(xs, ys));
        return temp0;
    }
    std::abort();
}

template<typename T0>
slist<T0> rev(slist<T0> arg0) {
    for (;;) {
        if (arg0->cons != slistCons::sNil) {
            break;
        }
        return construct<slist<T0>>(slistCons::sNil);
    }
    for (;;) {
        if (arg0->cons != slistCons::sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        slist<T0> temp0 = construct<slist<T0>>(slistCons::sCons);
        temp0->set_c1(x, construct<slist<T0>>(slistCons::sNil));
        return app(rev(xs), temp0);
    }
    std::abort();
}

slist<snat> snat2slist(snat arg0) {
    for (;;) {
        auto n = arg0;
        slist<snat> temp0 = construct<slist<snat>>(slistCons::sCons);
        temp0->set_c1(n, construct<slist<snat>>(slistCons::sNil));
        return temp0;
    }
    std::abort();
}

template<typename T0>
snat len(slist<T0> arg0) {
    for (;;) {
        if (arg0->cons != slistCons::sNil) {
            break;
        }
        return construct<snat>(snatCons::sZero);
    }
    for (;;) {
        if (arg0->cons != slistCons::sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        snat temp0 = construct<snat>(snatCons::sSucc);
        temp0->set_c1(len(xs));
        return temp0;
    }
    std::abort();
}

slist<std::uint64_t> listwithlen(std::uint64_t arg0) {
    for (;;) {
        if (arg0 != 0) {
            break;
        }
        return construct<slist<std::uint64_t>>(slistCons::sNil);
    }
    for (;;) {
        auto n = (arg0) - 1;
        slist<std::uint64_t> temp0 = construct<slist<std::uint64_t>>(slistCons::sCons);
        temp0->set_c1(1, listwithlen(n));
        return temp0;
    }
    std::abort();
}

template<typename T0>
std::uint64_t leninnat(slist<T0> arg0) {
    for (;;) {
        if (arg0->cons != slistCons::sNil) {
            break;
        }
        return 0;
    }
    for (;;) {
        if (arg0->cons != slistCons::sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        return (leninnat(xs)) + (1);
    }
    std::abort();
}

template<typename T0>
slist<T0> dblist(slist<T0> arg0) {
    for (;;) {
        if (arg0->cons != slistCons::sNil) {
            break;
        }
        return construct<slist<T0>>(slistCons::sNil);
    }
    for (;;) {
        if (arg0->cons != slistCons::sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        if (arg0->get_c1().p1->cons != slistCons::sNil) {
            break;
        }
        slist<T0> temp0 = construct<slist<T0>>(slistCons::sCons);
        slist<T0> temp1 = construct<slist<T0>>(slistCons::sCons);
        temp1->set_c1(x, construct<slist<T0>>(slistCons::sNil));
        temp0->set_c1(x, temp1);
        return temp0;
    }
    for (;;) {
        if (arg0->cons != slistCons::sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        slist<T0> temp0 = construct<slist<T0>>(slistCons::sCons);
        temp0->set_c1(x, construct<slist<T0>>(slistCons::sNil));
        return app(dblist(temp0), dblist(xs));
    }
    std::abort();
}

sbool snot(sbool arg0) {
    for (;;) {
        if (arg0.cons != sboolCons::sTrue) {
            break;
        }
        return sbool(sboolCons::sFalse);
    }
    for (;;) {
        auto sFlase = arg0;
        return sbool(sboolCons::sTrue);
    }
    std::abort();
}

