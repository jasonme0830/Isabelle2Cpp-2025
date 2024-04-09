#include <cstdint>
#include <cstdlib>
#include <memory>
#include <variant>

class snat {
    struct _sZero {
        bool operator<(const _sZero &) const { return false; }
    };
    struct _sSucc {
        std::shared_ptr<snat> p1_;

        snat p1() const { return *p1_; }

        bool operator<(const _sSucc &rhs) const {
            return std::tie(*p1_) < std::tie(*rhs.p1_);
        }
    };

    std::variant<_sZero, _sSucc> value_;
    snat(const std::variant<_sZero, _sSucc> &value) : value_(value) {}

  public:
    snat() = default;

    static snat sZero() {
        return snat { _sZero {  } };
    }
    static snat sSucc(const snat &p1) {
        return snat { _sSucc { std::make_shared<snat>(p1) } };
    }

    bool is_sZero() const { return std::holds_alternative<_sZero>(value_); }
    bool is_sSucc() const { return std::holds_alternative<_sSucc>(value_); }

    const _sSucc &as_sSucc() const { return std::get<_sSucc>(value_); }

    bool operator<(const snat &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class slist {
    struct _sNil {
        bool operator<(const _sNil &) const { return false; }
    };
    struct _sCons {
        T1 p1_;
        std::shared_ptr<slist<T1>> p2_;

        const T1 &p1() const { return p1_; }
        slist<T1> p2() const { return *p2_; }

        bool operator<(const _sCons &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };

    std::variant<_sNil, _sCons> value_;
    slist(const std::variant<_sNil, _sCons> &value) : value_(value) {}

  public:
    slist() = default;

    static slist<T1> sNil() {
        return slist<T1> { _sNil {  } };
    }
    static slist<T1> sCons(const T1 &p1, const slist<T1> &p2) {
        return slist<T1> { _sCons { p1, std::make_shared<slist<T1>>(p2) } };
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }

    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }

    bool operator<(const slist<T1> &rhs) const { return value_ < rhs.value_; }
};

class sbool {
    struct _sTrue {
        bool operator<(const _sTrue &) const { return false; }
    };
    struct _sFalse {
        bool operator<(const _sFalse &) const { return false; }
    };

    std::variant<_sTrue, _sFalse> value_;
    sbool(const std::variant<_sTrue, _sFalse> &value) : value_(value) {}

  public:
    sbool() = default;

    static sbool sTrue() {
        return sbool { _sTrue {  } };
    }
    static sbool sFalse() {
        return sbool { _sFalse {  } };
    }

    bool is_sTrue() const { return std::holds_alternative<_sTrue>(value_); }
    bool is_sFalse() const { return std::holds_alternative<_sFalse>(value_); }

    bool operator<(const sbool &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class alist {
    struct _sNil {
        bool operator<(const _sNil &) const { return false; }
    };
    struct _sCons {
        T1 p1_;
        std::shared_ptr<alist<T1>> p2_;

        const T1 &p1() const { return p1_; }
        alist<T1> p2() const { return *p2_; }

        bool operator<(const _sCons &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };

    std::variant<_sNil, _sCons> value_;
    alist(const std::variant<_sNil, _sCons> &value) : value_(value) {}

  public:
    alist() = default;

    static alist<T1> sNil() {
        return alist<T1> { _sNil {  } };
    }
    static alist<T1> sCons(const T1 &p1, const alist<T1> &p2) {
        return alist<T1> { _sCons { p1, std::make_shared<alist<T1>>(p2) } };
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }

    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }

    bool operator<(const alist<T1> &rhs) const { return value_ < rhs.value_; }
};

snat add(const snat &arg1, const snat &arg2);

snat fib(const snat &arg1);

std::uint64_t natofsnat(const snat &arg1);

snat snatofnat(const std::uint64_t &arg1);

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2) {
    // app sNil ys = ys
    if (arg1.is_sNil()) {
        return arg2;
    }

    // app (sCons x xs) ys = sCons x (app xs ys)
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x, app(xs, arg2)
    );
    return temp0;
}

template<typename T1>
slist<T1> rev(const slist<T1> &arg1) {
    // rev sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // rev (sCons x xs) = app (rev xs) (sCons x sNil)
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x, slist<T1>::sNil()
    );
    return app(rev(xs), temp0);
}

slist<snat> snat2slist(const snat &arg1);

template<typename T1>
snat len(const slist<T1> &arg1) {
    // len sNil = sZero
    if (arg1.is_sNil()) {
        return snat::sZero();
    }

    // len (sCons x xs) = sSucc (len xs)
    auto xs = arg1.as_sCons().p2();
    auto temp0 = snat::sSucc(
        len(xs)
    );
    return temp0;
}

slist<std::uint64_t> listwithlen(const std::uint64_t &arg1);

template<typename T1>
std::uint64_t leninnat(const slist<T1> &arg1) {
    // leninnat sNil = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // leninnat (sCons x xs) = (leninnat xs) + 1
    auto xs = arg1.as_sCons().p2();
    return leninnat(xs) + 1;
}

template<typename T1>
slist<T1> dblist(const slist<T1> &arg1) {
    // dblist sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // dblist (sCons x sNil) = sCons x (sCons x sNil)
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            auto x = arg1.as_sCons().p1();
            auto temp0 = slist<T1>::sCons(
                x, slist<T1>::sNil()
            );
            auto temp1 = slist<T1>::sCons(
                x, temp0
            );
            return temp1;
        }
    }

    // dblist (sCons x xs) = app (dblist (sCons x sNil)) (dblist xs)
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x, slist<T1>::sNil()
    );
    return app(dblist(temp0), dblist(xs));
}

sbool snot(const sbool &arg1);

// generated by HOL2Cpp
