#include <cstdint>
#include <cstdlib>
#include <functional>
#include <list>
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

        snat p1() const { return p1_->self(); }

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

    //返回自身根节点函数
    snat self() const {
        if(std::holds_alternative<_sZero>(value_)){ 
            return snat(_sZero());
        }
        if(std::holds_alternative<_sSuc>(value_)){ 
            const _sSuc& value = std::get<_sSuc>(value_);
            return snat( _sSuc(value.p1_));
        }else{
            return snat(_sZero());
        }
    } 

    static snat sZero() {
        return snat ( _sZero ( ));
    }
    static snat sSuc(const snat& p1) {
        return snat ( _sSuc ( 
            std::make_shared<snat>(p1.self())));
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
        slist<T1> p2() const { return p2_->self(); }

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

    //返回自身根节点函数
    slist<T1> self() const {
        if(std::holds_alternative<_sNil>(value_)){ 
            return slist<T1>(_sNil());
        }
        if(std::holds_alternative<_sCons>(value_)){ 
            const _sCons& value = std::get<_sCons>(value_);
            return slist<T1>( _sCons(value.p1_, value.p2_));
        }else{
            return slist<T1>(_sNil());
        }
    } 

    static slist<T1> sNil() {
        return slist<T1> ( _sNil ( ));
    }
    static slist<T1> sCons(const T1& p1, const slist<T1>& p2) {
        return slist<T1> ( _sCons ( 
            p1
            , std::make_shared<slist<T1>>(p2.self())));
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

        tree<T1> p1() const { return p1_->self(); }
        const T1 &p2() const { return p2_; }
        tree<T1> p3() const { return p3_->self(); }

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

    //返回自身根节点函数
    tree<T1> self() const {
        if(std::holds_alternative<_Tip>(value_)){ 
            return tree<T1>(_Tip());
        }
        if(std::holds_alternative<_Node>(value_)){ 
            const _Node& value = std::get<_Node>(value_);
            return tree<T1>( _Node(value.p1_, value.p2_, value.p3_));
        }else{
            return tree<T1>(_Tip());
        }
    } 

    static tree<T1> Tip() {
        return tree<T1> ( _Tip ( ));
    }
    static tree<T1> Node(const tree<T1>& p1, const T1& p2, const tree<T1>& p3) {
        return tree<T1> ( _Node ( 
            std::make_shared<tree<T1>>(p1.self())
            , p2
            , std::make_shared<tree<T1>>(p3.self())));
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



template<typename T1>
slist<T1> AddListHead(const T1 &arg1, const slist<T1> &arg2) {
    // AddListHead a xs = sCons a  xs
    auto a = arg1;
    auto xs = arg2.self();
    auto temp0 = slist<T1>::sCons(
        a,
        xs
    );
    return temp0;
}

template<typename T1>
slist<T1> DelListHead(const slist<T1> &arg1) {
    // DelListHead (sCons x xs) = xs
    if (arg1.is_sCons()) {
        auto xs = arg1.as_sCons().p2();
        return xs;
    }

    // DelListHead sNil = sNil
    return slist<T1>::sNil();
}

template<typename T1>
slist<T1> AddListTail(const T1 &arg1, const slist<T1> &arg2) {
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
slist<T1> DelListTail(const slist<T1> &arg1) {
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
slist<T1> AddListIs(const snat &arg1, const T1 &arg2, const slist<T1> &arg3) {
    // AddListIs i a sNil = sCons a sNil
    if (arg3.is_sNil()) {
        auto a = arg2;
        auto temp0 = slist<T1>::sCons(
            a,
            slist<T1>::sNil()
        );
        return temp0;
    }

    // AddListIs sZero a (sCons x xs) = AddListHead a (sCons x xs)
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

    // AddListIs (sSuc i) a (sCons x xs) = sCons x  (AddListIs i a xs)
    auto i = arg1.as_sSuc().p1();
    auto a = arg2;
    auto x = arg3.as_sCons().p1();
    auto xs = arg3.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        AddListIs(i, a, xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> DelListIs(const snat &arg1, const slist<T1> &arg2) {
    // DelListIs i sNil = sNil
    if (arg2.is_sNil()) {
        return slist<T1>::sNil();
    }

    // DelListIs i (sCons a sNil) = sNil
    if (arg2.is_sCons()) {
        if (arg2.as_sCons().p2().is_sNil()) {
            return slist<T1>::sNil();
        }
    }

    // DelListIs sZero (sCons x xs) = DelListHead (sCons x xs)
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

    // DelListIs (sSuc i) (sCons x xs) = sCons x  (DelListIs i xs)
    auto i = arg1.as_sSuc().p1();
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        DelListIs(i, xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> AddListI(const std::uint64_t &arg1, const T1 &arg2, const slist<T1> &arg3) {
    // AddListI i a sNil = sCons a sNil
    if (arg3.is_sNil()) {
        auto a = arg2;
        auto temp0 = slist<T1>::sCons(
            a,
            slist<T1>::sNil()
        );
        return temp0;
    }

    // AddListI Zero a (sCons x xs) = AddListHead a (sCons x xs)
    if (arg1 == 0) {
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

    // AddListI (Suc i) a (sCons x xs) = sCons x  (AddListI i a xs)
    auto i = arg1 - 1;
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
slist<T1> DelListI(const std::uint64_t &arg1, const slist<T1> &arg2) {
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

    // DelListI Zero (sCons x xs) = DelListHead (sCons x xs)
    if (arg1 == 0) {
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

    // DelListI (Suc i) (sCons x xs) = sCons x  (DelListI i xs)
    auto i = arg1 - 1;
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        DelListI(i, xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> ModifyValue(const T1 &arg1, const T1 &arg2, const slist<T1> &arg3) {
    // ModifyValue a b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // ModifyValue a b (sCons x xs) = (if x=a then (sCons b (ModifyValue a b xs)) else (sCons x (ModifyValue a b xs)))
    auto a = arg1;
    auto b = arg2;
    auto x = arg3.as_sCons().p1();
    auto xs = arg3.as_sCons().p2();
    slist<T1> temp2;
    if (x == a) {
        auto temp3 = slist<T1>::sCons(
            b,
            ModifyValue(a, b, xs)
        );
        temp2 = temp3.self();
    } else {
        auto temp4 = slist<T1>::sCons(
            x,
            ModifyValue(a, b, xs)
        );
        temp2 = temp4.self();
    }
    return temp2;
}

template<typename T1>
slist<T1> ModifyIndex(const snat &arg1, const T1 &arg2, const slist<T1> &arg3) {
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
bool SearchList(const T1 &arg1, const slist<T1> &arg2) {
    // SearchList a sNil = False
    if (arg2.is_sNil()) {
        return false;
    }

    // SearchList a (sCons x xs) = (if a=x then True else (SearchList a xs))
    auto a = arg1;
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    bool temp2;
    if (a == x) {
        temp2 = true;
    } else {
        temp2 = SearchList(a, xs);
    }
    return temp2;
}

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2) {
    // app sNil as =as
    if (arg1.is_sNil()) {
        auto as = arg2.self();
        return as;
    }

    // app(sCons a as ) bs= sCons a (app as bs)
    auto a = arg1.as_sCons().p1();
    auto as = arg1.as_sCons().p2();
    auto bs = arg2.self();
    auto temp0 = slist<T1>::sCons(
        a,
        app(as, bs)
    );
    return temp0;
}

template<typename T1>
slist<T1> Insert(const T1 &arg1, const slist<T1> &arg2) {
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
    slist<T1> temp2;
    if (a <= x) {
        auto temp3 = slist<T1>::sCons(
            x,
            xs
        );
        auto temp4 = slist<T1>::sCons(
            a,
            temp3
        );
        temp2 = temp4.self();
    } else {
        auto temp5 = slist<T1>::sCons(
            x,
            Insert(a, xs)
        );
        temp2 = temp5.self();
    }
    return temp2;
}

template<typename T1>
slist<T1> Reverse(const slist<T1> &arg1) {
    // Reverse sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Reverse (sCons a as) = app(Reverse as)(sCons a sNil)
    auto a = arg1.as_sCons().p1();
    auto as = arg1.as_sCons().p2();
    auto temp0 = Reverse(as);
    auto temp1 = slist<T1>::sCons(
        a,
        slist<T1>::sNil()
    );
    return app(temp0, temp1);
}

template<typename T1>
slist<T1> InsertSortPart(const slist<T1> &arg1, const slist<T1> &arg2) {
    // InsertSortPart sNil ys=ys
    if (arg1.is_sNil()) {
        auto ys = arg2.self();
        return ys;
    }

    // InsertSortPart (sCons x xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto ys = arg2.self();
    auto temp0 = Insert(x, ys);
    return InsertSortPart(xs, temp0);
}

template<typename T1>
slist<T1> InsertSort(const slist<T1> &arg1) {
    // InsertSort xs = InsertSortPart xs sNil
    auto xs = arg1.self();
    return InsertSortPart(xs, slist<T1>::sNil());
}

template<typename T1>
std::list<T1> ffilter(const std::function<bool(const T1 &)> &arg1, std::list<T1> arg2) {
    // ffilter f [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))
    auto f = arg1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (f(x)) {
        auto temp1 = ffilter(f, std::move(xs));
        temp1.push_front(x);
        temp0 = std::move(temp1);
    } else {
        temp0 = ffilter(f, std::move(xs));
    }
    return temp0;
}

template<typename T1>
std::list<T1> Reverse2(std::list<T1> arg1) {
    // Reverse2 [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // Reverse2 (Cons a []) =Cons a []
    if (!arg1.empty()) {
        if (std::list<T1>(std::next(arg1.begin()), arg1.end()).empty()) {
            auto a = arg1.front();
            auto temp0 = std::list<T1>();
            temp0.push_front(a);
            return temp0;
        }
    }

    // Reverse2 (x#xs) = ( Reverse2 xs)@ (Cons x Nil)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = std::list<T1>();
    temp0.push_front(x);
    auto temp1 = Reverse2(std::move(xs));
    auto temp2 = std::move(temp0);
    temp1.splice(temp1.end(), temp2);
    return temp1;
}

template<typename T1>
std::list<T1> Merge(std::list<T1> arg1, std::list<T1> arg2) {
    // Merge [] xs=xs
    if (arg1.empty()) {
        auto xs = arg2;
        return xs;
    }

    // Merge xs [] = xs
    if (arg2.empty()) {
        auto xs = arg1;
        return xs;
    }

    // Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys))
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto y = arg2.front();
    auto ys = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp2;
    if (x <= y) {
        auto temp4 = std::move(ys);
        temp4.push_front(y);
        auto temp3 = std::move(temp4);
        auto temp5 = Merge(std::move(xs), temp3);
        temp5.push_front(x);
        temp2 = std::move(temp5);
    } else {
        auto temp7 = std::move(xs);
        temp7.push_front(x);
        auto temp6 = std::move(temp7);
        auto temp8 = Merge(temp6, std::move(ys));
        temp8.push_front(y);
        temp2 = std::move(temp8);
    }
    return temp2;
}

template<typename T1>
std::list<T1> MergeSort(std::list<T1> arg1) {
    // MergeSort [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // MergeSort (Cons a []) = [a]
    if (!arg1.empty()) {
        if (std::list<T1>(std::next(arg1.begin()), arg1.end()).empty()) {
            auto a = arg1.front();
            return std::list<T1>{a};
        }
    }

    // MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs))
    auto xs = arg1;
    auto temp1 = std::list<T1>(xs.begin(), std::next(xs.begin(), size(xs) / 2));
    auto temp0 = MergeSort(temp1);
    auto temp8 = size(xs) / 2;
    auto temp9 = std::move(xs);
    temp9.erase(temp9.begin(), std::next(temp9.begin(), temp8));
    auto temp5 = std::move(temp9);
    auto temp4 = MergeSort(temp5);
    return Merge(temp0, temp4);
}

std::optional<std::uint64_t> bs(const std::uint64_t &arg1, std::list<std::uint64_t> arg2);

std::uint64_t two_fib(const std::uint64_t &arg1);

std::uint64_t three_fib(const std::uint64_t &arg1);

std::uint64_t four_fib(const std::uint64_t &arg1);

std::uint64_t Jacobsthal(const std::uint64_t &arg1);

std::uint64_t Narayana(const std::uint64_t &arg1);

std::uint64_t Leonardo(const std::uint64_t &arg1);

std::uint64_t Padua(const std::uint64_t &arg1);

std::uint64_t Pell(const std::uint64_t &arg1);

std::uint64_t Lucas(const std::uint64_t &arg1);

template<typename T1>
std::uint64_t slength(std::list<T1> arg1) {
    // slength [] = 0
    if (arg1.empty()) {
        return 0;
    }

    // slength (x # xs) = 1 + slength xs
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    return 1 + slength(std::move(xs));
}

template<typename T1>
std::list<T1> stake(const std::uint64_t &arg1, std::list<T1> arg2) {
    // stake 0 xs = []
    if (arg1 == 0) {
        return std::list<T1>();
    }

    // stake (Suc n) [] = []
    if (arg1 != 0) {
        if (arg2.empty()) {
            return std::list<T1>();
        }
    }

    // stake (Suc n) (x # xs) = x # stake n xs
    auto n = arg1 - 1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = stake(n, std::move(xs));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> sdrop(const std::uint64_t &arg1, std::list<T1> arg2) {
    // sdrop 0 xs = xs
    if (arg1 == 0) {
        auto xs = arg2;
        return xs;
    }

    // sdrop (Suc n) [] = []
    if (arg1 != 0) {
        if (arg2.empty()) {
            return std::list<T1>();
        }
    }

    // sdrop (Suc n) (x # xs) = sdrop n xs
    auto n = arg1 - 1;
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    return sdrop(n, std::move(xs));
}

std::list<std::uint64_t> supto(const std::uint64_t &arg1, const std::uint64_t &arg2);

template<typename T1>
T1 snth(std::list<T1> arg1, const std::uint64_t &arg2) {
    // snth [] n = 0
    if (arg1.empty()) {
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
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto n = arg2 - 1;
    return snth(std::move(xs), n);
}

template<typename T1>
tree<T1> copy_tree(const tree<T1> &arg1) {
    // copy_tree (Node left x right) = Node (copy_tree left) x (copy_tree right)
    if (arg1.is_Node()) {
        auto left = arg1.as_Node().p1();
        auto x = arg1.as_Node().p2();
        auto right = arg1.as_Node().p3();
        auto temp0 = tree<T1>::Node(
            copy_tree(left),
            x,
            copy_tree(right)
        );
        return temp0;
    }

    // copy_tree Tip = Tip
    return tree<T1>::Tip();
}

template<typename T1>
bool searchtree1(const T1 &arg1, const tree<T1> &arg2) {
    // searchtree1 a Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right))
    auto a = arg1;
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    return (a == x) || (searchtree1(a, left) || searchtree1(a, right));
}

template<typename T1>
bool searchtree2(const T1 &arg1, const tree<T1> &arg2) {
    // searchtree2 a Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtree2 a (Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right))
    auto a = arg1;
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    return searchtree2(a, left) || ((a == x) || searchtree2(a, right));
}

template<typename T1>
bool searchtree3(const T1 &arg1, const tree<T1> &arg2) {
    // searchtree3 a Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtree3 a (Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) )
    auto a = arg1;
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    return searchtree3(a, left) || (searchtree3(a, right) || (a == x));
}

template<typename T1>
tree<T1> inserttree(const T1 &arg1, const tree<T1> &arg2) {
    // inserttree a Tip = Node Tip a Tip
    if (arg2.is_Tip()) {
        auto a = arg1;
        auto temp0 = tree<T1>::Node(
            tree<T1>::Tip(),
            a,
            tree<T1>::Tip()
        );
        return temp0;
    }

    // inserttree a (Node left x right) = (if a\<le>x then (Node (inserttree a left) x right)  ...
    auto a = arg1;
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    tree<T1> temp2;
    if (a <= x) {
        auto temp3 = tree<T1>::Node(
            inserttree(a, left),
            x,
            right
        );
        temp2 = temp3.self();
    } else {
        auto temp4 = tree<T1>::Node(
            left,
            x,
            inserttree(a, right)
        );
        temp2 = temp4.self();
    }
    return temp2;
}

template<typename T1>
std::list<T1> transtolist(const tree<T1> &arg1) {
    // transtolist Tip = []
    if (arg1.is_Tip()) {
        return std::list<T1>();
    }

    // transtolist (Node left a right) =( a # (transtolist left)@(transtolist right))
    auto left = arg1.as_Node().p1();
    auto a = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp0 = transtolist(left);
    auto temp1 = transtolist(right);
    temp0.splice(temp0.end(), temp1);
    auto temp2 = std::move(temp0);
    temp2.push_front(a);
    return temp2;
}

template<typename T1>
T1 rightest(const tree<T1> &arg1) {
    // rightest (Node left x right) = (if right=Tip then x  ...
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    T1 temp2;
    if (right.is_Tip()) {
        temp2 = x;
    } else {
        temp2 = rightest(right);
    }
    return temp2;
}

template<typename T1>
tree<T1> delRightest(const tree<T1> &arg1) {
    // delRightest (Node left x Tip) = left
    if (arg1.is_Node()) {
        if (arg1.as_Node().p3().is_Tip()) {
            auto left = arg1.as_Node().p1();
            return left;
        }
    }

    // delRightest (Node left x right) = Node left x (delRightest right)
    if (arg1.is_Node()) {
        auto left = arg1.as_Node().p1();
        auto x = arg1.as_Node().p2();
        auto right = arg1.as_Node().p3();
        auto temp0 = tree<T1>::Node(
            left,
            x,
            delRightest(right)
        );
        return temp0;
    }

    // delRightest Tip = Tip
    return tree<T1>::Tip();
}

template<typename T1>
tree<T1> rightestleft(const tree<T1> &arg1) {
    // rightestleft Tip = Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // rightestleft (Node left x right) =(if right=Tip then left  ...
    auto left = arg1.as_Node().p1();
    auto right = arg1.as_Node().p3();
    tree<T1> temp2;
    if (right.is_Tip()) {
        temp2 = left.self();
    } else {
        temp2 = rightestleft(right);
    }
    return temp2;
}

template<typename T1>
tree<T1> deltreeroot(const tree<T1> &arg1) {
    // deltreeroot Tip = Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // deltreeroot (Node left x right) =(if right=Tip  ...
    auto left = arg1.as_Node().p1();
    auto right = arg1.as_Node().p3();
    tree<T1> temp2;
    if (right.is_Tip()) {
        temp2 = left.self();
    } else {
        tree<T1> temp5;
        if (left.is_Tip()) {
            temp5 = right.self();
        } else {
            auto temp6 = tree<T1>::Node(
                delRightest(left),
                rightest(left),
                right
            );
            temp5 = temp6.self();
        }
        temp2 = temp5.self();
    }
    return temp2;
}

template<typename T1>
tree<T1> addtreeroot(const T1 &arg1, const tree<T1> &arg2) {
    // addtreeroot x Tip = Node Tip x Tip
    if (arg2.is_Tip()) {
        auto x = arg1;
        auto temp0 = tree<T1>::Node(
            tree<T1>::Tip(),
            x,
            tree<T1>::Tip()
        );
        return temp0;
    }

    // addtreeroot x (Node left m Tip) = Node left x (Node Tip m Tip)
    if (arg2.is_Node()) {
        if (arg2.as_Node().p3().is_Tip()) {
            auto x = arg1;
            auto left = arg2.as_Node().p1();
            auto m = arg2.as_Node().p2();
            auto temp0 = tree<T1>::Node(
                tree<T1>::Tip(),
                m,
                tree<T1>::Tip()
            );
            auto temp1 = tree<T1>::Node(
                left,
                x,
                temp0
            );
            return temp1;
        }
    }

    // addtreeroot x (Node Tip m right) = Node (Node Tip m Tip) x right
    if (arg2.is_Node()) {
        if (arg2.as_Node().p1().is_Tip()) {
            auto x = arg1;
            auto m = arg2.as_Node().p2();
            auto right = arg2.as_Node().p3();
            auto temp0 = tree<T1>::Node(
                tree<T1>::Tip(),
                m,
                tree<T1>::Tip()
            );
            auto temp1 = tree<T1>::Node(
                temp0,
                x,
                right
            );
            return temp1;
        }
    }

    // addtreeroot x (Node left m right) = (Node (Node left m right) x Tip)
    auto x = arg1;
    auto left = arg2.as_Node().p1();
    auto m = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    auto temp0 = tree<T1>::Node(
        left,
        m,
        right
    );
    auto temp1 = tree<T1>::Node(
        temp0,
        x,
        tree<T1>::Tip()
    );
    return temp1;
}

template<typename T1>
tree<T1> deltree(const T1 &arg1, const tree<T1> &arg2) {
    // deltree a Tip = Tip
    if (arg2.is_Tip()) {
        return tree<T1>::Tip();
    }

    // deltree a (Node left x right) =( if a=x then(deltreeroot(Node left x right)) ...
    auto a = arg1;
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    tree<T1> temp2;
    if (a == x) {
        auto temp3 = tree<T1>::Node(
            left,
            x,
            right
        );
        temp2 = deltreeroot(temp3);
    } else {
        tree<T1> temp6;
        if (a < x) {
            auto temp7 = tree<T1>::Node(
                deltree(a, left),
                x,
                right
            );
            temp6 = temp7.self();
        } else {
            auto temp8 = tree<T1>::Node(
                left,
                x,
                deltree(a, right)
            );
            temp6 = temp8.self();
        }
        temp2 = temp6.self();
    }
    return temp2;
}

template<typename T1>
tree<T1> changetree(const T1 &arg1, const T1 &arg2, const tree<T1> &arg3) {
    // changetree a b Tip = Tip
    if (arg3.is_Tip()) {
        return tree<T1>::Tip();
    }

    // changetree a b (Node left x right) = ( if (a=x) then (Node left b right) ...
    auto a = arg1;
    auto b = arg2;
    auto left = arg3.as_Node().p1();
    auto x = arg3.as_Node().p2();
    auto right = arg3.as_Node().p3();
    tree<T1> temp2;
    if (a == x) {
        auto temp3 = tree<T1>::Node(
            left,
            b,
            right
        );
        temp2 = temp3.self();
    } else {
        tree<T1> temp6;
        if (a < x) {
            auto temp7 = tree<T1>::Node(
                changetree(a, b, left),
                x,
                right
            );
            temp6 = temp7.self();
        } else {
            auto temp8 = tree<T1>::Node(
                left,
                x,
                changetree(a, b, right)
            );
            temp6 = temp8.self();
        }
        temp2 = temp6.self();
    }
    return temp2;
}

template<typename T1>
std::list<T1> sorttree(const tree<T1> &arg1) {
    // sorttree Tip = []
    if (arg1.is_Tip()) {
        return std::list<T1>();
    }

    // sorttree (Node Tip a Tip) =[a]
    if (arg1.is_Node()) {
        if (arg1.as_Node().p1().is_Tip()) {
            if (arg1.as_Node().p3().is_Tip()) {
                auto a = arg1.as_Node().p2();
                return std::list<T1>{a};
            }
        }
    }

    // sorttree (Node left x right) =(MergeSort (transtolist (Node left x right)))
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp1 = tree<T1>::Node(
        left,
        x,
        right
    );
    auto temp0 = transtolist(temp1);
    return MergeSort(temp0);
}

// generated by HOL2Cpp
