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
        _aLeaf() {}
        _aLeaf(const _aLeaf& other){ }
        _aLeaf(_aLeaf&& other) noexcept{ }
        bool operator<(const _aLeaf &) const { return false; }
        _aLeaf& operator=(const _aLeaf& other) { return *this; }
        _aLeaf& operator=(_aLeaf&& other) noexcept { return *this; }
    };
    struct _aNode1 {
        std::shared_ptr<atree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<atree<T1>> p3_;

        atree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        atree<T1> p3() const { return *p3_; }


        _aNode1(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3 )
            :p1_(std::make_shared<atree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<atree<T1>>(p3))
        {}
        _aNode1(const _aNode1& other)
            :p1_(std::make_shared<atree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<atree<T1>>(*other.p3_))
        {}
        _aNode1(_aNode1&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _aNode1 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _aNode1& operator=(const _aNode1& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<atree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<atree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _aNode1& operator=(_aNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_aLeaf, _aNode1> value_;

  public:

    atree(const std::variant<_aLeaf, _aNode1> &value) : value_(value) {}
    //深拷贝构造函数
    atree(const atree<T1>& other) { 
        if(std::holds_alternative<_aLeaf>(other.value_)){ 
            const _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_aNode1>(other.value_)){ 
            const _aNode1& other_node = std::get<_aNode1>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    atree(atree<T1>&& other){
        if(std::holds_alternative<_aLeaf>(other.value_)){ 
            _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_aNode1>(other.value_)){ 
            _aNode1& other_node = std::get<_aNode1>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static atree<T1> new_aLeaf() {
        return atree<T1> ( _aLeaf (  ) );
    }
    static atree<T1> new_aNode1(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3) {
        return atree<T1> ( _aNode1 ( p1, p2, p3 ) );
    }

    bool is_aLeaf() const { return std::holds_alternative<_aLeaf>(value_); }
    bool is_aNode1() const { return std::holds_alternative<_aNode1>(value_); }
    const _aNode1 &as_aNode1() const { return std::get<_aNode1>(value_); }

    bool operator<(const atree<T1> &rhs) const { return value_ < rhs.value_; }
    atree<T1>& operator=(atree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_aLeaf>(other)){
                _aLeaf& other_value = std::get<_aLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_aNode1>(other)){
                _aNode1& other_value = std::get<_aNode1>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    atree<T1>& operator=(const atree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_aLeaf>(other.value_)){ 
                const _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_aNode1>(other.value_)){ 
                const _aNode1& other_node = std::get<_aNode1>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};



template<typename T1>
std::uint64_t test(const std::deque<T1> &arg1) {
    // test Nil = 0
    if (arg1.empty()) {
        return 0;
    }

    // test (Cons x xs) = length (If ((length xs) = 0) Nil xs) + 1
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    std::deque<T1> temp0;
    if (xs.size() == 0) {
        temp0 = std::deque<T1>();
    } else {
        temp0 = xs;
    }
    return temp0.size() + 1;
}

std::uint64_t add(const std::uint64_t &arg1, const std::uint64_t &arg2);

template<typename T1>
std::deque<T1> app(const std::deque<T1> &arg1, const std::deque<T1> &arg2) {
    // app Nil ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // app (Cons x xs) ys = Cons x (app xs ys)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = app(xs, arg2);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> rev(const std::deque<T1> &arg1) {
    // rev Nil = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // rev (Cons x xs) = app (rev xs) (Cons x Nil)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = std::deque<T1>();
    temp0.push_front(x);
    return app(rev(xs), temp0);
}

template<typename T1>
std::deque<T1> rev2(const std::deque<T1> &arg1) {
    // rev2 Nil = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // rev2 (x # xs) = (rev2 xs) @ (x # Nil)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = std::deque<T1>();
    temp0.push_front(x);
    auto temp1 = rev2(xs);
    auto temp2 = temp0;
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
}

template<typename T1>
std::deque<T1> rev3(const std::deque<T1> &arg1) {
    // rev3 [] = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // rev3 (x # xs) = (rev3 xs) @ [x]
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = rev3(xs);
    auto temp1 = std::deque<T1>{x};
    temp0.insert(temp0.end(), temp1.begin(), temp1.end());
    return temp0;
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
std::set<T1> testset(const std::deque<T1> &arg1) {
    // testset Nil = {}
    if (arg1.empty()) {
        return std::set<T1>{};
    }

    // testset (x # xs) = {x} \<inter> testset(xs)
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
        auto temp0 = std::set<T1>{x};
        auto temp1 = testset(xs);
        temp0.merge(temp1);
        return temp1;
    }

    // testset (x # xs) = {x} \<union> testset(xs)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = std::set<T1>{x};
    auto temp1 = testset(xs);
    temp0.merge(temp1);
    return temp0;
}

template<typename T1>
std::deque<T1> testifelse(const std::deque<T1> &arg1, const T1 &arg2) {
    // testifelse [] n = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // testifelse (x # xs) n = (if (x < n) then [x] else []) @ (testifelse xs n)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    std::deque<T1> temp0;
    if (x < arg2) {
        temp0 = std::deque<T1>{x};
    } else {
        temp0 = std::deque<T1>();
    }
    auto temp1 = temp0;
    auto temp2 = testifelse(xs, arg2);
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
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

std::uint64_t fact(const std::uint64_t &arg1);

bool altrue(const std::uint64_t &arg1);

std::deque<std::uint64_t> merge(const std::deque<std::uint64_t> &arg1, const std::deque<std::uint64_t> &arg2);

std::deque<std::uint64_t> merge_sort(const std::deque<std::uint64_t> &arg1);

// generated by HOL2Cpp
