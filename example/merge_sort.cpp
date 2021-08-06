#include <cstdint>
#include <cstdlib>
#include <list>
#include <memory>
#include <variant>

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

std::list<std::uint64_t> merge(const std::list<std::uint64_t> &arg1, const std::list<std::uint64_t> &arg2) {
    for (;;) {
        if (!arg2.empty()) {
            break;
        }
        return arg1;
    }
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return arg2;
    }
    for (;;) {
        if (arg1.empty()) {
            break;
        }
        auto x = arg1.front();
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        if (arg2.empty()) {
            break;
        }
        auto y = arg2.front();
        auto ys = decltype(arg2){std::next(arg2.begin()), arg2.end()};
        std::list<std::uint64_t> temp0;
        if ((x) <= (y)) {
            auto temp1 = ys;
            temp1.push_front(y);
            auto temp2 = merge(xs, temp1);
            temp2.push_front(x);
            temp0 = temp2;
        } else {
            auto temp3 = xs;
            temp3.push_front(x);
            auto temp4 = merge(temp3, ys);
            temp4.push_front(y);
            temp0 = temp4;
        }
        return temp0;
    }
    std::abort();
}

std::list<std::uint64_t> merge_sort(const std::list<std::uint64_t> &arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<std::uint64_t>();
    }
    for (;;) {
        if (arg1.size() != 1) {
            break;
        }
        auto x = *std::next(arg1.begin(), 0);
        return std::list<std::uint64_t>{x};
    }
    for (;;) {
        return merge(merge_sort(decltype(arg1){ arg1.begin(), std::next(arg1.begin(), (arg1.size()) / (2)) }), merge_sort(decltype(arg1){ std::next(arg1.begin(), (arg1.size()) / (2)), arg1.end() }));
    }
    std::abort();
}

template<typename T1>
std::uint64_t slength(const slist<T1> &arg1) {
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
        return (slength(xs)) + 1;
    }
    std::abort();
}

template<typename T1>
slist<T1> stake(const std::uint64_t &arg1, const slist<T1> &arg2) {
    for (;;) {
        if (arg2->cons != slistCons::sNil) {
            break;
        }
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto m = (arg1) - 1;
        if (arg2->cons != slistCons::sCons) {
            break;
        }
        auto x = arg2->get_c2().p1;
        auto xs = arg2->get_c2().p2;
        slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
        temp0->set_c2(x, stake(m, xs));
        return temp0;
    }
    std::abort();
}

template<typename T1>
slist<T1> sdrop(const std::uint64_t &arg1, const slist<T1> &arg2) {
    for (;;) {
        if (arg2->cons != slistCons::sNil) {
            break;
        }
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1 != 0) {
            break;
        }
        return arg2;
    }
    for (;;) {
        if (arg1 == 0) {
            break;
        }
        auto m = (arg1) - 1;
        if (arg2->cons != slistCons::sCons) {
            break;
        }
        auto x = arg2->get_c2().p1;
        auto xs = arg2->get_c2().p2;
        return sdrop(m, xs);
    }
    std::abort();
}

slist<std::uint64_t> smerge(const slist<std::uint64_t> &arg1, const slist<std::uint64_t> &arg2) {
    for (;;) {
        if (arg2->cons != slistCons::sNil) {
            break;
        }
        return arg1;
    }
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
        if (arg2->cons != slistCons::sCons) {
            break;
        }
        auto y = arg2->get_c2().p1;
        auto ys = arg2->get_c2().p2;
        slist<std::uint64_t> temp0;
        if ((x) <= (y)) {
            slist<std::uint64_t> temp1 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
            slist<std::uint64_t> temp2 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
            temp2->set_c2(y, ys);
            temp1->set_c2(x, smerge(xs, temp2));
            temp0 = temp1;
        } else {
            slist<std::uint64_t> temp3 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
            slist<std::uint64_t> temp4 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
            temp4->set_c2(x, xs);
            temp3->set_c2(y, smerge(temp4, ys));
            temp0 = temp3;
        }
        return temp0;
    }
    std::abort();
}

slist<std::uint64_t> smerge_sort(const slist<std::uint64_t> &arg1) {
    for (;;) {
        if (arg1->cons != slistCons::sNil) {
            break;
        }
        return std::make_shared<slistElem<std::uint64_t>>(slistCons::sNil);
    }
    for (;;) {
        if (arg1->cons != slistCons::sCons) {
            break;
        }
        auto x = arg1->get_c2().p1;
        if (arg1->get_c2().p2->cons != slistCons::sNil) {
            break;
        }
        slist<std::uint64_t> temp0 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
        temp0->set_c2(x, std::make_shared<slistElem<std::uint64_t>>(slistCons::sNil));
        return temp0;
    }
    for (;;) {
        return smerge(smerge_sort(stake((slength(arg1)) / (2), arg1)), smerge_sort(sdrop((slength(arg1)) / (2), arg1)));
    }
    std::abort();
}

