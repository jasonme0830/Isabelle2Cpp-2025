#include <cstdint>
#include <cstdlib>
#include <memory>
#include <variant>

struct snat {
    struct _sZero {};
    struct _sSucc {
        std::shared_ptr<snat> p1_;

        snat p1() const { return *p1_; }
    };

    std::variant<_sZero, _sSucc> value_;

    static snat sZero() {
        return snat { _sZero {} };
    }
    static snat sSucc(snat p1) {
        return snat { _sSucc {std::make_shared<snat>(p1)} };
    }

    bool is_sZero() const { return std::holds_alternative<_sZero>(value_); }
    bool is_sSucc() const { return std::holds_alternative<_sSucc>(value_); }

    const _sSucc &as_sSucc() const { return std::get<_sSucc>(value_); }
};

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
        return slist<T1> { _sNil {} };
    }
    static slist<T1> sCons(T1 p1, slist<T1> p2) {
        return slist<T1> { _sCons {p1, std::make_shared<slist<T1>>(p2)} };
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }

    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }
};

struct sbool {
    struct _sTrue {};
    struct _sFalse {};

    std::variant<_sTrue, _sFalse> value_;

    static sbool sTrue() {
        return sbool { _sTrue {} };
    }
    static sbool sFalse() {
        return sbool { _sFalse {} };
    }

    bool is_sTrue() const { return std::holds_alternative<_sTrue>(value_); }
    bool is_sFalse() const { return std::holds_alternative<_sFalse>(value_); }


};

snat add(const snat &arg1, const snat &arg2);

snat fib(const snat &arg1);

std::uint64_t natofsnat(const snat &arg1);

snat snatofnat(const std::uint64_t &arg1);

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2);

template<typename T1>
slist<T1> rev(const slist<T1> &arg1);

slist<snat> snat2slist(const snat &arg1);

template<typename T1>
snat len(const slist<T1> &arg1);

slist<std::uint64_t> listwithlen(const std::uint64_t &arg1);

template<typename T1>
std::uint64_t leninnat(const slist<T1> &arg1);

template<typename T1>
slist<T1> dblist(const slist<T1> &arg1);

sbool snot(const sbool &arg1);

