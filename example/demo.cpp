#include <cstdlib>
#include <list>
#include <memory>
#include <variant>

template<typename T, typename Cons> T construct(Cons cons) { return std::make_shared<typename T::element_type>(cons); }

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

template<typename T1>
std::list<T1> app1(std::list<T1> arg1, std::list<T1> arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        auto ys = arg2;
        return ys;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto ys = arg2;
        auto temp0 = app1(xs, ys);
        temp0.push_front(x);
        return temp0;
    }
    std::abort();
}

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

