#include <cstdint>
#include <cstdlib>
#include <list>
#include <memory>
#include <variant>

template<typename T1>
struct slist {
    struct _sNil {};
    struct _sCons {
        T1 p1_;
        std::shared_ptr<slist<T1>> p2_;

        const T1 &p1() const { return p1_; }
        slist<T1> p2() const { return *p2_; }
    };

    std::variant<_sNil, _sCons> value_;

    static slist<T1> sNil() {
        return slist<T1> { _sNil {  } };
    }
    static slist<T1> sCons(T1 p1, slist<T1> p2) {
        return slist<T1> { _sCons { p1, std::make_shared<slist<T1>>(p2) } };
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }

    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }
};

std::list<std::uint64_t> merge(const std::list<std::uint64_t> &arg1, const std::list<std::uint64_t> &arg2);

std::list<std::uint64_t> merge_sort(const std::list<std::uint64_t> &arg1);

template<typename T1>
std::uint64_t slength(const slist<T1> &arg1) {
    // slength sNil = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // slength (sCons x xs) = Suc (slength xs)
    if (arg1.is_sCons()) {
        auto xs = arg1.as_sCons().p2();
        return (slength(xs)) + 1;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> stake(const std::uint64_t &arg1, const slist<T1> &arg2) {
    // stake n sNil = sNil
    if (arg2.is_sNil()) {
        return slist<T1>::sNil();
    }

    // stake 0 _ = sNil
    if (arg1 == 0) {
        return slist<T1>::sNil();
    }

    // stake (Suc m) (sCons x xs) = sCons x (stake m xs)
    if (arg1 != 0) {
        if (arg2.is_sCons()) {
            auto m = arg1 - 1;
            auto x = arg2.as_sCons().p1();
            auto xs = arg2.as_sCons().p2();
            auto temp0 = slist<T1>::sCons(
                x, stake(m, xs)
            );
            return temp0;
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> sdrop(const std::uint64_t &arg1, const slist<T1> &arg2) {
    // sdrop n sNil = sNil
    if (arg2.is_sNil()) {
        return slist<T1>::sNil();
    }

    // sdrop 0 xs = xs
    if (arg1 == 0) {
        return arg2;
    }

    // sdrop (Suc m) (sCons x xs) = sdrop m xs
    if (arg1 != 0) {
        if (arg2.is_sCons()) {
            auto m = arg1 - 1;
            auto xs = arg2.as_sCons().p2();
            return sdrop(m, xs);
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<std::uint64_t> smerge(const slist<std::uint64_t> &arg1, const slist<std::uint64_t> &arg2);

slist<std::uint64_t> smerge_sort(const slist<std::uint64_t> &arg1);

