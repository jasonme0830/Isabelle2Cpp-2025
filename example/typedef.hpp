#include <cstdint>
#include <cstdlib>
#include <deque>
#include <memory>
#include <set>
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
class alsit {
    struct _aNil {
        bool operator<(const _aNil &) const { return false; }
    };
    struct _aCons {
        T1 p1_;
        alist<T1> p2_;

        const T1 &p1() const { return p1_; }
        const alist<T1> &p2() const { return p2_; }

        bool operator<(const _aCons &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
    };

    std::variant<_aNil, _aCons> value_;
    alsit(const std::variant<_aNil, _aCons> &value) : value_(value) {}

  public:
    alsit() = default;

    static alsit<T1> aNil() {
        return alsit<T1> { _aNil {  } };
    }
    static alsit<T1> aCons(const T1 &p1, const alist<T1> &p2) {
        return alsit<T1> { _aCons { p1, p2 } };
    }

    bool is_aNil() const { return std::holds_alternative<_aNil>(value_); }
    bool is_aCons() const { return std::holds_alternative<_aCons>(value_); }

    const _aCons &as_aCons() const { return std::get<_aCons>(value_); }

    bool operator<(const alsit<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class atree {
    struct _aLeaf {
        bool operator<(const _aLeaf &) const { return false; }
    };
    struct _aNode1 {
        std::shared_ptr<atree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<atree<T1>> p3_;

        atree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        atree<T1> p3() const { return *p3_; }

        bool operator<(const _aNode1 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
    };

    std::variant<_aLeaf, _aNode1> value_;
    atree(const std::variant<_aLeaf, _aNode1> &value) : value_(value) {}

  public:
    atree() = default;

    static atree<T1> aLeaf() {
        return atree<T1> { _aLeaf {  } };
    }
    static atree<T1> aNode1(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3) {
        return atree<T1> { _aNode1 { std::make_shared<atree<T1>>(p1), p2, std::make_shared<atree<T1>>(p3) } };
    }

    bool is_aLeaf() const { return std::holds_alternative<_aLeaf>(value_); }
    bool is_aNode1() const { return std::holds_alternative<_aNode1>(value_); }

    const _aNode1 &as_aNode1() const { return std::get<_aNode1>(value_); }

    bool operator<(const atree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
using slist = alsit;


snat add(const snat &arg1, const snat &arg2);

template<typename T1>
std::deque<T1> app(std::deque<T1> arg1, std::deque<T1> arg2) {
    // app Nil ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // app (Cons x xs) ys = Cons x (app xs ys)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = app(std::move(xs), std::move(arg2));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> rev(std::deque<T1> arg1) {
    // rev Nil = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // rev (Cons x xs) = app (rev xs) (Cons x Nil)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = std::deque<T1>();
    temp0.push_front(x);
    return app(rev(std::move(xs)), std::move(temp0));
}

snat snatofnat(const std::uint64_t &arg1);

template<typename T1>
std::set<T1> testset(std::deque<T1> arg1) {
    // testset Nil = {}
    if (arg1.empty()) {
        return std::set<T1>{};
    }

    // testset (x # xs) = {x} \<inter> testset(xs)
    if (arg1.size() >= 1) {
        auto x = arg1.front();
        arg1.erase(arg1.begin(), arg1.begin() + 1);
        auto xs = std::move(arg1);
        auto temp0 = std::set<T1>{x};
        auto temp1 = testset(std::move(xs));
        temp0.merge(temp1);
        return temp1;
    }

    // testset (x # xs) = {x} \<union> testset(xs)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = std::set<T1>{x};
    auto temp1 = testset(std::move(xs));
    temp0.merge(temp1);
    return temp0;
}

std::uint64_t fib(const std::uint64_t &arg1);

snat sfib(const snat &arg1);

std::deque<std::uint64_t> merge(std::deque<std::uint64_t> arg1, std::deque<std::uint64_t> arg2);

std::deque<std::uint64_t> merge_sort(std::deque<std::uint64_t> arg1);

// generated by HOL2Cpp
