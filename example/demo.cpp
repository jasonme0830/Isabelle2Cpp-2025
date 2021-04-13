#include <cstdlib>
#include <list>
#include <memory>
#include <variant>
#include <iostream>

template<typename T, typename Cons> T construct(Cons cons) { return std::make_shared<typename T::element_type>(cons); }

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

template<typename T0>
std::list<T0> app(std::list<T0> arg0, std::list<T0> arg1) {
    for (;;) {
        if (!arg0.empty()) {
            break;
        }
        auto ys = arg1;
        return ys;
    }
    for (;;) {
        if (arg0.empty()) {
            break;
        }
        auto x = arg0.front();
        arg0.pop_front();
        auto xs = arg0;
        auto ys = arg1;
        auto temp0 = app(xs, ys);
        temp0.push_front(x);
        return temp0;
    }
    std::abort();
}

template<typename T0>
slist<T0> app2(slist<T0> arg0, slist<T0> arg1) {
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

int main() {
    std::list<int> list0 {0, 1, 2};
    std::list<int> list1 {3, 4, 5};
    for (auto elem : app(list0, list1)) {
        std::cout << elem << " ";
    }
}
