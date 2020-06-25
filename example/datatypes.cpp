#include <cstdint>
#include <cstdlib>
#include <memory>
#include <variant>

enum snatConsEnum {
    sZero,
    sSucc,
};

struct snatNode;
using snat = std::shared_ptr<snatNode>;

struct snatNode {
    struct c0{
    };
    struct c1{
        snat p0;
    };

    snatConsEnum cons;
    std::variant<c0, c1> value;

    snatNode(snatConsEnum cons) : cons(cons) {}

    c0 &get_c0() {
        return std::get<c0>(value);
    };
    void set_c0() {
        value = c0 {};
    }
    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1(snat _p0) {
        value = c1 {_p0};
    }
};


enum slistConsEnum {
    sNil,
    sCons,
};

template<typename T0>
struct slistNode;
template<typename T0>
using slist = std::shared_ptr<slistNode<T0>>;

template<typename T0>
struct slistNode {
    struct c0{
    };
    struct c1{
        T0 p0;
        slist<T0> p1;
    };

    slistConsEnum cons;
    std::variant<c0, c1> value;

    slistNode(slistConsEnum cons) : cons(cons) {}

    c0 &get_c0() {
        return std::get<c0>(value);
    };
    void set_c0() {
        value = c0 {};
    }
    c1 &get_c1() {
        return std::get<c1>(value);
    };
    void set_c1(T0 _p0, slist<T0> _p1) {
        value = c1 {_p0, _p1};
    }
};


template<typename T0>
slist<T0>
app(slist<T0> arg0, slist<T0> arg1) {
    for (;;) {
        if (arg0->cons != sNil) {
            break;
        }
        auto ys = arg1;
        return ys;
    }
    for (;;) {
        if (arg0->cons != sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        auto ys = arg1;
        slist<T0> temp0 = std::make_shared<slist<T0>::element_type>(sCons);
        temp0->set_c1(x, app(xs, ys));
        return temp0;
    }
    std::abort();
}

template<typename T0>
slist<T0>
rev(slist<T0> arg0) {
    for (;;) {
        if (arg0->cons != sNil) {
            break;
        }
        return std::make_shared<slist<T0>::element_type>(sNil);
    }
    for (;;) {
        if (arg0->cons != sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        slist<T0> temp0 = std::make_shared<slist<T0>::element_type>(sCons);
        temp0->set_c1(x, std::make_shared<slist<T0>::element_type>(sNil));
        return app(rev(xs), temp0);
    }
    std::abort();
}

slist<snat>
snat2slist(snat arg0) {
    for (;;) {
        auto n = arg0;
        slist<snat> temp0 = std::make_shared<slist<snat>::element_type>(sCons);
        temp0->set_c1(n, std::make_shared<slist<snat>::element_type>(sNil));
        return temp0;
    }
    std::abort();
}

template<typename T0>
snat
len(slist<T0> arg0) {
    for (;;) {
        if (arg0->cons != sNil) {
            break;
        }
        return std::make_shared<snat::element_type>(sZero);
    }
    for (;;) {
        if (arg0->cons != sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        snat temp0 = std::make_shared<snat::element_type>(sSucc);
        temp0->set_c1(len(xs));
        return temp0;
    }
    std::abort();
}

slist<std::uint64_t>
listwithlen(std::uint64_t arg0) {
    for (;;) {
        if (arg0 != 0) {
            break;
        }
        return std::make_shared<slist<std::uint64_t>::element_type>(sNil);
    }
    for (;;) {
        auto n = (arg0) - 1;
        slist<std::uint64_t> temp0 = std::make_shared<slist<std::uint64_t>::element_type>(sCons);
        temp0->set_c1(1, listwithlen(n));
        return temp0;
    }
    std::abort();
}

template<typename T0>
std::uint64_t
leninnat(slist<T0> arg0) {
    for (;;) {
        if (arg0->cons != sNil) {
            break;
        }
        return 0;
    }
    for (;;) {
        if (arg0->cons != sCons) {
            break;
        }
        auto x = arg0->get_c1().p0;
        auto xs = arg0->get_c1().p1;
        return (leninnat(xs)) + (1);
    }
    std::abort();
}

