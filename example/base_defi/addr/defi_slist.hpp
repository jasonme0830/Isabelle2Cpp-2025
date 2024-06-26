#include <cstdint>
#include <cstdlib>
#include <memory>
#include <variant>

class snat {
    struct _sZero {
        _sZero() {}
        _sZero(const _sZero& other){ }
        _sZero(_sZero&& other) noexcept{ }
        bool operator<(const _sZero &) const { return false; }
        _sZero& operator=(const _sZero& other) { return *this; }
        _sZero& operator=(_sZero&& other) noexcept { return *this; }
    };
    struct _sSuc {
        std::shared_ptr<snat> p1_;

        snat p1() const { return *p1_; }


        _sSuc(const snat &p1 )
            :p1_(std::make_shared<snat>(p1))
        {}
        _sSuc(const _sSuc& other)
            :p1_(std::make_shared<snat>(*other.p1_))
        {}
        _sSuc(_sSuc&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _sSuc &rhs) const {
            return std::tie(*p1_) < std::tie(*rhs.p1_);
        }
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
    };

  public:

    std::variant<_sZero, _sSuc> value_;

    //默认构造函数
    snat(){
        value_ = _sZero();
    }
    //value构造函数
    snat(std::variant<_sZero, _sSuc> value) : value_(value) {}
    //深拷贝构造函数
    snat(const snat& other) { 
        if(std::holds_alternative<_sZero>(other.value_)){ 
            const _sZero& other_node = std::get<_sZero>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_sSuc>(other.value_)){ 
            const _sSuc& other_node = std::get<_sSuc>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    snat(snat&& other){
        if(std::holds_alternative<_sZero>(other.value_)){ 
            _sZero& other_node = std::get<_sZero>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_sSuc>(other.value_)){ 
            _sSuc& other_node = std::get<_sSuc>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static snat sZero() {
        return snat ( _sZero (  ) );
    }
    static snat sSuc(const snat &p1) {
        return snat ( _sSuc ( p1 ) );
    }

    bool is_sZero() const { return std::holds_alternative<_sZero>(value_); }
    bool is_sSuc() const { return std::holds_alternative<_sSuc>(value_); }
    const _sSuc &as_sSuc() const { return std::get<_sSuc>(value_); }

    bool operator<(const snat &rhs) const { return value_ < rhs.value_; }
    snat& operator=(snat&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_sZero>(other.value_)){
                _sZero& other_value = std::get<_sZero>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_sSuc>(other.value_)){
                _sSuc& other_value = std::get<_sSuc>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    snat& operator=(const snat& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_sZero>(other.value_)){ 
                const _sZero& other_node = std::get<_sZero>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_sSuc>(other.value_)){ 
                const _sSuc& other_node = std::get<_sSuc>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class slist {
    struct _sNil {
        _sNil() {}
        _sNil(const _sNil& other){ }
        _sNil(_sNil&& other) noexcept{ }
        bool operator<(const _sNil &) const { return false; }
        _sNil& operator=(const _sNil& other) { return *this; }
        _sNil& operator=(_sNil&& other) noexcept { return *this; }
    };
    struct _sCons {
        T1 p1_;
        std::shared_ptr<slist<T1>> p2_;

        const T1 &p1() const { return p1_; }
        slist<T1> p2() const { return *p2_; }


        _sCons(const T1 &p1, const slist<T1> &p2 )
            :p1_(p1)
            ,p2_(std::make_shared<slist<T1>>(p2))
        {}
        _sCons(const _sCons& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<slist<T1>>(*other.p2_))
        {}
        _sCons(_sCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _sCons &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
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
    };

  public:

    std::variant<_sNil, _sCons> value_;

    //默认构造函数
    slist(){
        value_ = _sNil();
    }
    //value构造函数
    slist(std::variant<_sNil, _sCons> value) : value_(value) {}
    //深拷贝构造函数
    slist(const slist<T1>& other) { 
        if(std::holds_alternative<_sNil>(other.value_)){ 
            const _sNil& other_node = std::get<_sNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_sCons>(other.value_)){ 
            const _sCons& other_node = std::get<_sCons>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    slist(slist<T1>&& other){
        if(std::holds_alternative<_sNil>(other.value_)){ 
            _sNil& other_node = std::get<_sNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_sCons>(other.value_)){ 
            _sCons& other_node = std::get<_sCons>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static slist<T1> sNil() {
        return slist<T1> ( _sNil (  ) );
    }
    static slist<T1> sCons(const T1 &p1, const slist<T1> &p2) {
        return slist<T1> ( _sCons ( p1, p2 ) );
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }
    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }

    bool operator<(const slist<T1> &rhs) const { return value_ < rhs.value_; }
    slist<T1>& operator=(slist<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_sNil>(other.value_)){
                _sNil& other_value = std::get<_sNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_sCons>(other.value_)){
                _sCons& other_value = std::get<_sCons>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    slist<T1>& operator=(const slist<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_sNil>(other.value_)){ 
                const _sNil& other_node = std::get<_sNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_sCons>(other.value_)){ 
                const _sCons& other_node = std::get<_sCons>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};



std::uint64_t natofsnat(const snat &arg1);

snat snatofnat(const std::uint64_t &arg1);

template<typename T1>
slist<T1> AddListHead(const T1 &arg1, const slist<T1> &arg2) {
    // AddListHead a xs = sCons a  xs
    auto a = arg1;
    auto xs = arg2;
    auto temp0 = slist<T1>::sCons(
        std::move(a),
        std::move(xs)
    );
    return temp0;
}

template<typename T1>
slist<T1> AddListTail(const T1 &arg1, const slist<T1> &arg2) {
    // AddListTail a sNil =sCons a sNil
    if (arg2.is_sNil()) {
        auto a = arg1;
        auto temp0 = slist<T1>::sCons(
            std::move(a),
            slist<T1>::sNil()
        );
        return temp0;
    }

    // AddListTail a (sCons x xs) = sCons x  (AddListTail a xs )
    auto a = arg1;
    auto x = std::move(*arg2.as_sCons().p1_);
    auto xs = std::move(*arg2.as_sCons().p2_);
    auto temp0 = slist<T1>::sCons(
        std::move(x),
        AddListTail(std::move(a), std::move(xs))
    );
    return temp0;
}

template<typename T1>
slist<T1> AddListI(const snat &arg1, const T1 &arg2, const slist<T1> &arg3) {
    // AddListI i a sNil = sCons a sNil
    if (arg3.is_sNil()) {
        auto a = arg2;
        auto temp0 = slist<T1>::sCons(
            std::move(a),
            slist<T1>::sNil()
        );
        return temp0;
    }

    // AddListI sZero a (sCons x xs) = AddListHead a (sCons x xs)
    if (arg1.is_sZero()) {
        if (arg3.is_sCons()) {
            auto a = arg2;
            auto x = std::move(*arg3.as_sCons().p1_);
            auto xs = std::move(*arg3.as_sCons().p2_);
            auto temp0 = std::move(a);
            auto temp1 = slist<T1>::sCons(
                std::move(x),
                std::move(xs)
            );
            return AddListHead(std::move(temp0), std::move(temp1));
        }
    }

    // AddListI (sSuc i) a (sCons x xs) = sCons x  (AddListI i a xs)
    auto i = std::move(*arg1.as_sSuc().p1_);
    auto a = arg2;
    auto x = std::move(*arg3.as_sCons().p1_);
    auto xs = std::move(*arg3.as_sCons().p2_);
    auto temp0 = slist<T1>::sCons(
        std::move(x),
        AddListI(std::move(i), std::move(a), std::move(xs))
    );
    return temp0;
}

template<typename T1>
slist<T1> DelListHead(const slist<T1> &arg1) {
    // DelListHead (sCons x xs) = xs
    if (arg1.is_sCons()) {
        auto xs = std::move(*arg1.as_sCons().p2_);
        return xs;
    }

    // DelListHead sNil = sNil
    return slist<T1>::sNil();
}

template<typename T1>
slist<T1> DelListTail(const slist<T1> &arg1) {
    // DelListTail sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // DelListTail (sCons a sNil) = sNil
    if (arg1.is_sCons()) {
        if (std::move(*arg1.as_sCons().p2_).is_sNil()) {
            return slist<T1>::sNil();
        }
    }

    // DelListTail (sCons x xs) =  sCons x (DelListTail xs)
    auto x = std::move(*arg1.as_sCons().p1_);
    auto xs = std::move(*arg1.as_sCons().p2_);
    auto temp0 = slist<T1>::sCons(
        std::move(x),
        DelListTail(std::move(xs))
    );
    return temp0;
}

template<typename T1>
slist<T1> DelListI(const snat &arg1, const slist<T1> &arg2) {
    // DelListI i sNil = sNil
    if (arg2.is_sNil()) {
        return slist<T1>::sNil();
    }

    // DelListI i (sCons a sNil) = sNil
    if (arg2.is_sCons()) {
        if (std::move(*arg2.as_sCons().p2_).is_sNil()) {
            return slist<T1>::sNil();
        }
    }

    // DelListI sZero (sCons x xs) = DelListHead (sCons x xs)
    if (arg1.is_sZero()) {
        if (arg2.is_sCons()) {
            auto x = std::move(*arg2.as_sCons().p1_);
            auto xs = std::move(*arg2.as_sCons().p2_);
            auto temp0 = slist<T1>::sCons(
                std::move(x),
                std::move(xs)
            );
            return DelListHead(std::move(temp0));
        }
    }

    // DelListI (sSuc i) (sCons x xs) = sCons x  (DelListI i xs)
    auto i = std::move(*arg1.as_sSuc().p1_);
    auto x = std::move(*arg2.as_sCons().p1_);
    auto xs = std::move(*arg2.as_sCons().p2_);
    auto temp0 = slist<T1>::sCons(
        std::move(x),
        DelListI(std::move(i), std::move(xs))
    );
    return temp0;
}

template<typename T1>
slist<T1> Modify1(const T1 &arg1, const T1 &arg2, const slist<T1> &arg3) {
    // Modify1 a b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Modify1 a b (sCons x xs) = (if x=a then (sCons b (Modify1 a b xs)) else (sCons x (Modify1 a b xs)))
    auto a = arg1;
    auto b = arg2;
    auto x = std::move(*arg3.as_sCons().p1_);
    auto xs = std::move(*arg3.as_sCons().p2_);
    slist<T1> temp0;
    if (x == a) {
        auto temp1 = slist<T1>::sCons(
            b,
            Modify1(std::move(a), std::move(b), std::move(xs))
        );
        temp0 = std::move(temp1);
    } else {
        auto temp2 = slist<T1>::sCons(
            std::move(x),
            Modify1(std::move(a), std::move(b), std::move(xs))
        );
        temp0 = std::move(temp2);
    }
    return temp0;
}

template<typename T1>
slist<T1> Modify2(const snat &arg1, const T1 &arg2, const slist<T1> &arg3) {
    // Modify2 n b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Modify2 sZero b (sCons x xs) =(sCons b xs)
    if (arg1.is_sZero()) {
        if (arg3.is_sCons()) {
            auto b = arg2;
            auto xs = std::move(*arg3.as_sCons().p2_);
            auto temp0 = slist<T1>::sCons(
                std::move(b),
                std::move(xs)
            );
            return temp0;
        }
    }

    // Modify2 (sSuc n) b (sCons x xs) = (sCons x (Modify2 n b xs))
    auto n = std::move(*arg1.as_sSuc().p1_);
    auto b = arg2;
    auto x = std::move(*arg3.as_sCons().p1_);
    auto xs = std::move(*arg3.as_sCons().p2_);
    auto temp0 = slist<T1>::sCons(
        std::move(x),
        Modify2(std::move(n), std::move(b), std::move(xs))
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
    auto x = std::move(*arg2.as_sCons().p1_);
    auto xs = std::move(*arg2.as_sCons().p2_);
    bool temp0;
    if (a == std::move(x)) {
        temp0 = true;
    } else {
        temp0 = SearchList(std::move(a), std::move(xs));
    }
    return temp0;
}

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2) {
    // app sNil as =as
    if (arg1.is_sNil()) {
        auto as = arg2;
        return as;
    }

    // app(sCons a as ) bs= sCons a (app as bs)
    auto a = std::move(*arg1.as_sCons().p1_);
    auto as = std::move(*arg1.as_sCons().p2_);
    auto bs = arg2;
    auto temp0 = slist<T1>::sCons(
        std::move(a),
        app(std::move(as), std::move(bs))
    );
    return temp0;
}

template<typename T1>
slist<T1> Reverse(const slist<T1> &arg1) {
    // Reverse sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Reverse (sCons a as) = app(Reverse as)(sCons a sNil)
    auto a = std::move(*arg1.as_sCons().p1_);
    auto as = std::move(*arg1.as_sCons().p2_);
    auto temp0 = Reverse(std::move(as));
    auto temp1 = slist<T1>::sCons(
        std::move(a),
        slist<T1>::sNil()
    );
    return app(std::move(temp0), std::move(temp1));
}

// generated by HOL2Cpp
