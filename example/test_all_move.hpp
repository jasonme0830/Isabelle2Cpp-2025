#include <cstdint>
#include <cstdlib>
#include <deque>
#include <memory>
#include <optional>
#include <set>
#include <utility>
#include <variant>

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
std::uint64_t test(std::deque<T1> arg1) {
    // test Nil = 0
    if (arg1.empty()) {
        return 0;
    }

    // test (Cons x xs) = length (If ((length xs) = 0) Nil xs) + 1
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    std::deque<T1> temp0;
    if (xs.size() == 0) {
        temp0 = std::deque<T1>();
    } else {
        temp0 = std::move(xs);
    }
    return temp0.size() + 1;
}

std::uint64_t add(const std::uint64_t &arg1, const std::uint64_t &arg2);

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
    auto temp0 = std::move(xs);
    auto temp1 = std::move(arg2);
    auto temp2 = app(std::move(temp0), std::move(temp1));
    temp2.push_front(x);
    return temp2;
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
    auto temp1 = std::move(xs);
    auto temp0 = rev(std::move(temp1));
    auto temp3 = std::deque<T1>();
    temp3.push_front(x);
    auto temp2 = std::move(temp3);
    return app(std::move(temp0), std::move(temp2));
}

template<typename T1>
std::deque<T1> rev2(std::deque<T1> arg1) {
    // rev2 Nil = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // rev2 (x # xs) = (rev2 xs) @ (x # Nil)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = std::move(xs);
    auto temp1 = std::deque<T1>();
    temp1.push_front(x);
    auto temp2 = rev2(std::move(temp0));
    auto temp3 = std::move(temp1);
    temp2.insert(temp2.end(), temp3.begin(), temp3.end());
    return temp2;
}

template<typename T1>
std::deque<T1> rev3(std::deque<T1> arg1) {
    // rev3 [] = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // rev3 (x # xs) = (rev3 xs) @ [x]
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = std::move(xs);
    auto temp1 = rev3(std::move(temp0));
    auto temp2 = std::deque<T1>{x};
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
}

template<typename T1>
std::optional<std::deque<T1>> testoption(const std::optional<T1> &arg1) {
    // testoption None = Some Nil
    if (!arg1.has_value()) {
        return std::make_optional<std::deque<T1>>(std::deque<T1>());
    }

    // testoption (Some x) = Some [x]
    auto x = arg1.value();
    return std::make_optional<std::deque<T1>>(std::deque<T1>{x});
}

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
        auto temp0 = std::move(xs);
        auto temp1 = std::set<T1>{x};
        auto temp2 = testset(std::move(temp0));
        temp1.merge(temp2);
        return temp2;
    }

    // testset (x # xs) = {x} \<union> testset(xs)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = std::move(xs);
    auto temp1 = std::set<T1>{x};
    auto temp2 = testset(std::move(temp0));
    temp1.merge(temp2);
    return temp1;
}

template<typename T1>
std::deque<T1> testifelse(std::deque<T1> arg1, const T1 &arg2) {
    // testifelse [] n = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // testifelse (x # xs) n = (if (x < n) then [x] else []) @ (testifelse xs n)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    std::deque<T1> temp0;
    if (x < arg2) {
        temp0 = std::deque<T1>{x};
    } else {
        temp0 = std::deque<T1>();
    }
    auto temp1 = std::move(xs);
    auto temp2 = arg2;
    auto temp3 = temp0;
    auto temp4 = testifelse(std::move(temp1), std::move(temp2));
    temp3.insert(temp3.end(), temp4.begin(), temp4.end());
    return temp3;
}

template<typename T1, typename T2>
std::pair<T1, T2> pair(const T1 &arg1, const T2 &arg2) {
    // pair f s = (f, s)
    return std::make_pair(arg1, arg2);
}

template<typename T1, typename T2>
T1 fst(const std::pair<T1, T2> &arg1) {
    // fst (first, second) = first
    auto first = arg1.first;
    return first;
}

template<typename T1, typename T2>
T2 snd(const std::pair<T1, T2> &arg1) {
    // snd (first, second) = second
    auto second = arg1.second;
    return second;
}

bool evn(const std::uint64_t &arg1);

std::uint64_t natofsnat(const std::uint64_t &arg1);

std::uint64_t snatofnat(const std::uint64_t &arg1);

std::uint64_t fib(const std::uint64_t &arg1);

std::uint64_t sfib(const std::uint64_t &arg1);

bool altrue(const std::uint64_t &arg1);

std::deque<std::uint64_t> merge(std::deque<std::uint64_t> arg1, std::deque<std::uint64_t> arg2);

std::deque<std::uint64_t> merge_sort(std::deque<std::uint64_t> arg1);

// generated by HOL2Cpp
