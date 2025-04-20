#include <cstdint>
#include <cstdlib>
#include <deque>
#include <map>
#include <memory>
#include <optional>
#include <utility>
#include <variant>

class snat {
    struct _sZero {
        _sZero() {}
        _sZero(const _sZero& other){ }
        _sZero(_sZero&& other) noexcept{ }
        _sZero& operator=(const _sZero& other) { return *this; }
        _sZero& operator=(_sZero&& other) noexcept { return *this; }
        bool operator==(const _sZero &) const { return true; }
        bool operator<(const _sZero &) const { return false; }
        bool operator>(const _sZero &) const { return false; }
        bool operator<=(const _sZero &) const { return true; }
        bool operator>=(const _sZero &) const { return true; }
    };
    struct _sSuc {
        std::shared_ptr<snat> p1_;

        snat p1() const { return *p1_; }

        _sSuc(std::shared_ptr<snat> p1)
            :p1_(std::move(p1))
        {}
        _sSuc(const _sSuc& other)
            :p1_(std::make_shared<snat>(*other.p1_))
        {}
        _sSuc(_sSuc&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        _sSuc& operator=(const _sSuc& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<snat>(*other.p1_);
            } 
            return *this; 
        } 
        _sSuc& operator=(_sSuc&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
        bool operator==(const _sSuc &rhs) const {
            return (std::tie(*p1_) == std::tie(*rhs.p1_));
        }
        bool operator<(const _sSuc &rhs) const {
            return (std::tie(*p1_) < std::tie(*rhs.p1_));
        }
        bool operator>(const _sSuc &rhs) const {
            return (std::tie(*p1_) > std::tie(*rhs.p1_));
        }
        bool operator<=(const _sSuc &rhs) const {
            return (std::tie(*p1_) <= std::tie(*rhs.p1_));
        }
        bool operator>=(const _sSuc &rhs) const {
            return (std::tie(*p1_) >= std::tie(*rhs.p1_));
        }
    };

  public:

std::variant<_sZero, _sSuc> value_;

    //默认构造函数
    snat(){
        value_ = _sZero();
    }
    //value构造函数
    snat(std::variant<_sZero, _sSuc> value) : value_(std::move(value)) {}
    //深拷贝构造函数
    snat(const snat& other) { 
        if(std::holds_alternative<_sZero>(other.value_)){ 
            value_ = std::get<_sZero>(other.value_); 
        } 
        if(std::holds_alternative<_sSuc>(other.value_)){ 
            value_ = std::get<_sSuc>(other.value_); 
        } 
    } 
    //移动构造函数
    snat(snat&& other){
        if(std::holds_alternative<_sZero>(other.value_)){ 
            value_ = std::move(std::get<_sZero>(other.value_)); 
        } 
        if(std::holds_alternative<_sSuc>(other.value_)){ 
            value_ = std::move(std::get<_sSuc>(other.value_)); 
        } 
    }

    static snat sZero() {
        return snat ( _sZero ( ));
    }
    static snat sSuc(snat p1) {
        return snat ( _sSuc ( 
            std::make_shared<snat>(std::move(p1))));
    }

    bool is_sZero() const { return std::holds_alternative<_sZero>(value_); }
    bool is_sSuc() const { return std::holds_alternative<_sSuc>(value_); }
    const _sSuc &as_sSuc() const { return std::get<_sSuc>(value_); }
    //拷贝赋值运算符
    snat& operator=(const snat& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_sZero>(other.value_)){ 
                value_ = std::get<_sZero>(other.value_); 
            } 
            if(std::holds_alternative<_sSuc>(other.value_)){ 
                value_ = std::get<_sSuc>(other.value_); 
            } 
        } 
        return *this; 
    }
    snat& operator=(snat&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_sZero>(other.value_)){
                value_ = std::move(std::get<_sZero>(other.value_)); 
            }
            if(std::holds_alternative<_sSuc>(other.value_)){
                value_ = std::move(std::get<_sSuc>(other.value_)); 
            }
        }
        return *this;
    }

    bool operator==(const snat &rhs) const {
        if(value_.index() == rhs.value_.index()){
            return value_ == rhs.value_;
        }else{
            return false;
        }
    }
    bool operator<(const snat &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ < rhs.value_; 
    }
    bool operator>(const snat &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ > rhs.value_; 
    }
    bool operator<=(const snat &rhs) const {
        if(value_ == rhs.value_) return true;
        return value_ < rhs.value_; 
    }
    bool operator>=(const snat &rhs) const {
        if(value_ == rhs.value_) return true;
        return value_ > rhs.value_; 
    }

};

template<typename T1>
class slist {
    struct _sNil {
        _sNil() {}
        _sNil(const _sNil& other){ }
        _sNil(_sNil&& other) noexcept{ }
        _sNil& operator=(const _sNil& other) { return *this; }
        _sNil& operator=(_sNil&& other) noexcept { return *this; }
        bool operator==(const _sNil &) const { return true; }
        bool operator<(const _sNil &) const { return false; }
        bool operator>(const _sNil &) const { return false; }
        bool operator<=(const _sNil &) const { return true; }
        bool operator>=(const _sNil &) const { return true; }
    };
    struct _sCons {
        T1 p1_;
        std::shared_ptr<slist<T1>> p2_;

        const T1 &p1() const { return p1_; }
        slist<T1> p2() const { return *p2_; }

        _sCons(T1 p1, std::shared_ptr<slist<T1>> p2)
            :p1_(std::move(p1))
            ,p2_(std::move(p2))
        {}
        _sCons(const _sCons& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<slist<T1>>(*other.p2_))
        {}
        _sCons(_sCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        _sCons& operator=(const _sCons& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<slist<T1>>(*other.p2_);
            } 
            return *this; 
        } 
        _sCons& operator=(_sCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
        bool operator==(const _sCons &rhs) const {
            return (std::tie(p1_, *p2_) == std::tie(rhs.p1_, *rhs.p2_));
        }
        bool operator<(const _sCons &rhs) const {
            return (std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_));
        }
        bool operator>(const _sCons &rhs) const {
            return (std::tie(p1_, *p2_) > std::tie(rhs.p1_, *rhs.p2_));
        }
        bool operator<=(const _sCons &rhs) const {
            return (std::tie(p1_, *p2_) <= std::tie(rhs.p1_, *rhs.p2_));
        }
        bool operator>=(const _sCons &rhs) const {
            return (std::tie(p1_, *p2_) >= std::tie(rhs.p1_, *rhs.p2_));
        }
    };

  public:

std::variant<_sNil, _sCons> value_;

    //默认构造函数
    slist(){
        value_ = _sNil();
    }
    //value构造函数
    slist(std::variant<_sNil, _sCons> value) : value_(std::move(value)) {}
    //深拷贝构造函数
    slist(const slist<T1>& other) { 
        if(std::holds_alternative<_sNil>(other.value_)){ 
            value_ = std::get<_sNil>(other.value_); 
        } 
        if(std::holds_alternative<_sCons>(other.value_)){ 
            value_ = std::get<_sCons>(other.value_); 
        } 
    } 
    //移动构造函数
    slist(slist<T1>&& other){
        if(std::holds_alternative<_sNil>(other.value_)){ 
            value_ = std::move(std::get<_sNil>(other.value_)); 
        } 
        if(std::holds_alternative<_sCons>(other.value_)){ 
            value_ = std::move(std::get<_sCons>(other.value_)); 
        } 
    }

    static slist<T1> sNil() {
        return slist<T1> ( _sNil ( ));
    }
    static slist<T1> sCons(T1 p1, slist<T1> p2) {
        return slist<T1> ( _sCons ( 
            std::move(p1)
            , std::make_shared<slist<T1>>(std::move(p2))));
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }
    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }
    //拷贝赋值运算符
    slist<T1>& operator=(const slist<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_sNil>(other.value_)){ 
                value_ = std::get<_sNil>(other.value_); 
            } 
            if(std::holds_alternative<_sCons>(other.value_)){ 
                value_ = std::get<_sCons>(other.value_); 
            } 
        } 
        return *this; 
    }
    slist<T1>& operator=(slist<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_sNil>(other.value_)){
                value_ = std::move(std::get<_sNil>(other.value_)); 
            }
            if(std::holds_alternative<_sCons>(other.value_)){
                value_ = std::move(std::get<_sCons>(other.value_)); 
            }
        }
        return *this;
    }

    bool operator==(const slist<T1> &rhs) const {
        if(value_.index() == rhs.value_.index()){
            return value_ == rhs.value_;
        }else{
            return false;
        }
    }
    bool operator<(const slist<T1> &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ < rhs.value_; 
    }
    bool operator>(const slist<T1> &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ > rhs.value_; 
    }
    bool operator<=(const slist<T1> &rhs) const {
        if(value_ == rhs.value_) return true;
        return value_ < rhs.value_; 
    }
    bool operator>=(const slist<T1> &rhs) const {
        if(value_ == rhs.value_) return true;
        return value_ > rhs.value_; 
    }

};

template<typename T1>
class tree {
    struct _Tip {
        _Tip() {}
        _Tip(const _Tip& other){ }
        _Tip(_Tip&& other) noexcept{ }
        _Tip& operator=(const _Tip& other) { return *this; }
        _Tip& operator=(_Tip&& other) noexcept { return *this; }
        bool operator==(const _Tip &) const { return true; }
        bool operator<(const _Tip &) const { return false; }
        bool operator>(const _Tip &) const { return false; }
        bool operator<=(const _Tip &) const { return true; }
        bool operator>=(const _Tip &) const { return true; }
    };
    struct _Node {
        std::shared_ptr<tree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<tree<T1>> p3_;

        tree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        tree<T1> p3() const { return *p3_; }

        _Node(std::shared_ptr<tree<T1>> p1, T1 p2, std::shared_ptr<tree<T1>> p3)
            :p1_(std::move(p1))
            ,p2_(std::move(p2))
            ,p3_(std::move(p3))
        {}
        _Node(const _Node& other)
            :p1_(std::make_shared<tree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<tree<T1>>(*other.p3_))
        {}
        _Node(_Node&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        _Node& operator=(const _Node& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<tree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<tree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _Node& operator=(_Node&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
        bool operator==(const _Node &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) == std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator<(const _Node &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator>(const _Node &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) > std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator<=(const _Node &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) <= std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator>=(const _Node &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) >= std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
    };

  public:

std::variant<_Tip, _Node> value_;

    //默认构造函数
    tree(){
        value_ = _Tip();
    }
    //value构造函数
    tree(std::variant<_Tip, _Node> value) : value_(std::move(value)) {}
    //深拷贝构造函数
    tree(const tree<T1>& other) { 
        if(std::holds_alternative<_Tip>(other.value_)){ 
            value_ = std::get<_Tip>(other.value_); 
        } 
        if(std::holds_alternative<_Node>(other.value_)){ 
            value_ = std::get<_Node>(other.value_); 
        } 
    } 
    //移动构造函数
    tree(tree<T1>&& other){
        if(std::holds_alternative<_Tip>(other.value_)){ 
            value_ = std::move(std::get<_Tip>(other.value_)); 
        } 
        if(std::holds_alternative<_Node>(other.value_)){ 
            value_ = std::move(std::get<_Node>(other.value_)); 
        } 
    }

    static tree<T1> Tip() {
        return tree<T1> ( _Tip ( ));
    }
    static tree<T1> Node(tree<T1> p1, T1 p2, tree<T1> p3) {
        return tree<T1> ( _Node ( 
            std::make_shared<tree<T1>>(std::move(p1))
            , std::move(p2)
            , std::make_shared<tree<T1>>(std::move(p3))));
    }

    bool is_Tip() const { return std::holds_alternative<_Tip>(value_); }
    bool is_Node() const { return std::holds_alternative<_Node>(value_); }
    const _Node &as_Node() const { return std::get<_Node>(value_); }
    //拷贝赋值运算符
    tree<T1>& operator=(const tree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_Tip>(other.value_)){ 
                value_ = std::get<_Tip>(other.value_); 
            } 
            if(std::holds_alternative<_Node>(other.value_)){ 
                value_ = std::get<_Node>(other.value_); 
            } 
        } 
        return *this; 
    }
    tree<T1>& operator=(tree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_Tip>(other.value_)){
                value_ = std::move(std::get<_Tip>(other.value_)); 
            }
            if(std::holds_alternative<_Node>(other.value_)){
                value_ = std::move(std::get<_Node>(other.value_)); 
            }
        }
        return *this;
    }

    bool operator==(const tree<T1> &rhs) const {
        if(value_.index() == rhs.value_.index()){
            return value_ == rhs.value_;
        }else{
            return false;
        }
    }
    bool operator<(const tree<T1> &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ < rhs.value_; 
    }
    bool operator>(const tree<T1> &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ > rhs.value_; 
    }
    bool operator<=(const tree<T1> &rhs) const {
        if(value_ == rhs.value_) return true;
        return value_ < rhs.value_; 
    }
    bool operator>=(const tree<T1> &rhs) const {
        if(value_ == rhs.value_) return true;
        return value_ > rhs.value_; 
    }

};



std::uint64_t natofsnat(snat arg1);

snat snatofnat(std::uint64_t arg1);

template<typename T1>
slist<T1> AddListHead(T1 arg1, slist<T1> arg2) {
    // AddListHead a xs = sCons a  xs
    auto a = arg1;
    auto xs = arg2;
    auto temp0 = slist<T1>::sCons(
        a,
        xs
    );
    return temp0;
}

template<typename T1>
slist<T1> DelListHead(slist<T1> arg1) {
    // DelListHead (sCons x xs) = xs
    if (arg1.is_sCons()) {
        auto xs = arg1.as_sCons().p2();
        return xs;
    }

    // DelListHead sNil = sNil
    return slist<T1>::sNil();
}

template<typename T1>
slist<T1> AddListTail(T1 arg1, slist<T1> arg2) {
    // AddListTail a sNil =sCons a sNil
    if (arg2.is_sNil()) {
        auto a = arg1;
        auto temp0 = slist<T1>::sCons(
            a,
            slist<T1>::sNil()
        );
        return temp0;
    }

    // AddListTail a (sCons x xs) = sCons x  (AddListTail a xs )
    auto a = arg1;
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        AddListTail(a, xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> AddListI(snat arg1, T1 arg2, slist<T1> arg3) {
    // AddListI i a sNil = sCons a sNil
    if (arg3.is_sNil()) {
        auto a = arg2;
        auto temp0 = slist<T1>::sCons(
            a,
            slist<T1>::sNil()
        );
        return temp0;
    }

    // AddListI sZero a (sCons x xs) = AddListHead a (sCons x xs)
    if (arg1.is_sZero()) {
        if (arg3.is_sCons()) {
            auto a = arg2;
            auto x = arg3.as_sCons().p1();
            auto xs = arg3.as_sCons().p2();
            auto temp0 = slist<T1>::sCons(
                x,
                xs
            );
            return AddListHead(a, temp0);
        }
    }

    // AddListI (sSuc i) a (sCons x xs) = sCons x  (AddListI i a xs)
    auto i = arg1.as_sSuc().p1();
    auto a = arg2;
    auto x = arg3.as_sCons().p1();
    auto xs = arg3.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        AddListI(i, a, xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> DelListTail(slist<T1> arg1) {
    // DelListTail sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // DelListTail (sCons a sNil) = sNil
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            return slist<T1>::sNil();
        }
    }

    // DelListTail (sCons x xs) =  sCons x (DelListTail xs)
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        DelListTail(xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> DelListI(snat arg1, slist<T1> arg2) {
    // DelListI i sNil = sNil
    if (arg2.is_sNil()) {
        return slist<T1>::sNil();
    }

    // DelListI i (sCons a sNil) = sNil
    if (arg2.is_sCons()) {
        if (arg2.as_sCons().p2().is_sNil()) {
            return slist<T1>::sNil();
        }
    }

    // DelListI sZero (sCons x xs) = DelListHead (sCons x xs)
    if (arg1.is_sZero()) {
        if (arg2.is_sCons()) {
            auto x = arg2.as_sCons().p1();
            auto xs = arg2.as_sCons().p2();
            auto temp0 = slist<T1>::sCons(
                x,
                xs
            );
            return DelListHead(temp0);
        }
    }

    // DelListI (sSuc i) (sCons x xs) = sCons x  (DelListI i xs)
    auto i = arg1.as_sSuc().p1();
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        DelListI(i, xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> ModifyValue(T1 arg1, T1 arg2, slist<T1> arg3) {
    // ModifyValue a b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // ModifyValue a b (sCons x xs) = (if x=a then (sCons b (ModifyValue a b xs)) else (sCons x (ModifyValue a b xs)))
    auto a = arg1;
    auto b = arg2;
    auto x = arg3.as_sCons().p1();
    auto xs = arg3.as_sCons().p2();
    slist<T1> temp0;
    if (x == a) {
        auto temp1 = slist<T1>::sCons(
            b,
            ModifyValue(a, b, xs)
        );
        temp0 = temp1;
    } else {
        auto temp2 = slist<T1>::sCons(
            x,
            ModifyValue(a, b, xs)
        );
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
slist<T1> ModifyIndex(snat arg1, T1 arg2, slist<T1> arg3) {
    // ModifyIndex n b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // ModifyIndex sZero b (sCons x xs) =(sCons b xs)
    if (arg1.is_sZero()) {
        if (arg3.is_sCons()) {
            auto b = arg2;
            auto xs = arg3.as_sCons().p2();
            auto temp0 = slist<T1>::sCons(
                b,
                xs
            );
            return temp0;
        }
    }

    // ModifyIndex (sSuc n) b (sCons x xs) = (sCons x (ModifyIndex n b xs))
    auto n = arg1.as_sSuc().p1();
    auto b = arg2;
    auto x = arg3.as_sCons().p1();
    auto xs = arg3.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        ModifyIndex(n, b, xs)
    );
    return temp0;
}

template<typename T1>
bool SearchList(T1 arg1, slist<T1> arg2) {
    // SearchList a sNil = False
    if (arg2.is_sNil()) {
        return false;
    }

    // SearchList a (sCons x xs) = (if a=x then True else (SearchList a xs))
    auto a = arg1;
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    bool temp0;
    if (a == x) {
        temp0 = true;
    } else {
        temp0 = SearchList(a, xs);
    }
    return temp0;
}

template<typename T1>
slist<T1> app(slist<T1> arg1, slist<T1> arg2) {
    // app sNil as =as
    if (arg1.is_sNil()) {
        auto as = arg2;
        return as;
    }

    // app(sCons a as ) bs= sCons a (app as bs)
    auto a = arg1.as_sCons().p1();
    auto as = arg1.as_sCons().p2();
    auto bs = arg2;
    auto temp0 = slist<T1>::sCons(
        a,
        app(as, bs)
    );
    return temp0;
}

template<typename T1>
slist<T1> Insert(T1 arg1, slist<T1> arg2) {
    // Insert a sNil =sCons a sNil
    if (arg2.is_sNil()) {
        auto a = arg1;
        auto temp0 = slist<T1>::sCons(
            a,
            slist<T1>::sNil()
        );
        return temp0;
    }

    // Insert a (sCons x xs) =(if a \<le> x then (sCons a (sCons x xs)) else (sCons x (Insert a xs)))
    auto a = arg1;
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    slist<T1> temp0;
    if (a <= x) {
        auto temp1 = slist<T1>::sCons(
            x,
            xs
        );
        auto temp2 = slist<T1>::sCons(
            a,
            temp1
        );
        temp0 = temp2;
    } else {
        auto temp3 = slist<T1>::sCons(
            x,
            Insert(a, xs)
        );
        temp0 = temp3;
    }
    return temp0;
}

template<typename T1>
slist<T1> Reverse(slist<T1> arg1) {
    // Reverse sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Reverse (sCons a as) = app(Reverse as)(sCons a sNil)
    auto a = arg1.as_sCons().p1();
    auto as = arg1.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        a,
        slist<T1>::sNil()
    );
    return app(Reverse(as), temp0);
}

template<typename T1>
slist<T1> InsertSortPart(slist<T1> arg1, slist<T1> arg2) {
    // InsertSortPart sNil ys=ys
    if (arg1.is_sNil()) {
        auto ys = arg2;
        return ys;
    }

    // InsertSortPart (sCons x xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto ys = arg2;
    return InsertSortPart(xs, Insert(x, ys));
}

template<typename T1>
slist<T1> InsertSort(slist<T1> arg1) {
    // InsertSort xs = InsertSortPart xs sNil
    auto xs = arg1;
    return InsertSortPart(xs, slist<T1>::sNil());
}

template<typename T1>
slist<T1> Merge(std::deque<T1> arg1, slist<T1> arg2) {
    // Merge sNil xs=xs
    if (arg1.is_sNil()) {
        auto xs = arg2;
        return xs;
    }

    // Merge xs sNil = xs
    if (arg2.is_sNil()) {
        auto xs = arg1;
        return xs;
    }

    // Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys))
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto y = arg2.front();
    auto ys = slist<T1>(arg2.begin() + 1, arg2.end());
    slist<T1> temp0;
    if (x <= y) {
        auto temp1 = ys;
        temp1.push_front(y);
        auto temp2 = Merge(xs, temp1);
        temp2.push_front(x);
        temp0 = temp2;
    } else {
        auto temp3 = xs;
        temp3.push_front(x);
        auto temp4 = Merge(temp3, ys);
        temp4.push_front(y);
        temp0 = temp4;
    }
    return temp0;
}

template<typename T1>
slist<T1> MergeSort(std::deque<T1> arg1) {
    // MergeSort sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // MergeSort  (sCons a sNil) = [a]
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            auto a = arg1.as_sCons().p1();
            return slist<T1>{a};
        }
    }

    // MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs))
    auto xs = arg1;
    return Merge(MergeSort(std::deque<T1>(xs.begin(), xs.begin() + size(xs) / 2)), MergeSort(std::deque<T1>(xs.begin() + size(xs) / 2, xs.end())));
}

std::optional<snat> bs(snat arg1, slist<snat> arg2);

snat fib(snat arg1);

template<typename T1>
std::uint64_t ssize(slist<T1> arg1) {
    // ssize sNil = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // ssize  (sCons x sNil) = (Suc 0)
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            return 0 + 1;
        }
    }

    // ssize  (sCons x xs) = Suc (ssize xs)
    auto xs = arg1.as_sCons().p2();
    return ssize(xs) + 1;
}

template<typename T1>
std::uint64_t slength(slist<T1> arg1) {
    // slength sNil = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // slength  (sCons x sNil) = (Suc 0)
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            return 0 + 1;
        }
    }

    // slength  (sCons x xs) = (Suc (slength xs))
    auto xs = arg1.as_sCons().p2();
    return slength(xs) + 1;
}

template<typename T1>
slist<T1> stake(std::uint64_t arg1, slist<T1> arg2) {
    // stake 0 xs = sNil
    if (arg1 == 0) {
        return slist<T1>::sNil();
    }

    // stake (Suc n) sNil = sNil
    if (arg1 != 0) {
        if (arg2.is_sNil()) {
            return slist<T1>::sNil();
        }
    }

    // stake (Suc n)  (sCons x xs) = (sCons x (stake n xs))
    auto n = arg1 - 1;
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        stake(n, xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> sdrop(std::uint64_t arg1, slist<T1> arg2) {
    // sdrop 0 xs = xs
    if (arg1 == 0) {
        auto xs = arg2;
        return xs;
    }

    // sdrop (Suc n) sNil = sNil
    if (arg1 != 0) {
        if (arg2.is_sNil()) {
            return slist<T1>::sNil();
        }
    }

    // sdrop (Suc n)  (sCons x xs) = (sdrop n xs)
    auto n = arg1 - 1;
    auto xs = arg2.as_sCons().p2();
    return sdrop(n, xs);
}

slist<std::uint64_t> supto(std::uint64_t arg1, std::uint64_t arg2);

template<typename T1>
T1 snth(slist<T1> arg1, std::uint64_t arg2) {
    // snth sNil n = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // snth (x#xs) 0 = x
    if (!arg1.empty()) {
        if (arg2 == 0) {
            auto x = arg1.front();
            return x;
        }
    }

    // snth (x#xs) (Suc n) = snth xs n
    auto xs = slist<T1>(arg1.begin() + 1, arg1.end());
    auto n = arg2 - 1;
    return snth(xs, n);
}

// generated by HOL2Cpp
