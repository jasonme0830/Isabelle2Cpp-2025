#include <cstdlib>
#include <memory>
#include <variant>

class snat {
    struct _sZero {
        _sZero() {}
        _sZero(const _sZero& other){ }
        _sZero(_sZero&& other) noexcept{ }
        bool operator<(const _sZero &) const { return false; }
        bool operator==(const _sZero &) const { return true; }
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
        bool operator==(const _sZero &rhs) const {
            return false;
        }
        bool operator==(const _sSuc &rhs) const {
            if(std::tie(*p1_) < std::tie(*rhs.p1_)){
                return false;
            }else{
                return std::tie(*p1_) == std::tie(*rhs.p1_);
            }
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

    bool operator==(const snat &rhs) const { return value_ == rhs.value_; }
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
        bool operator==(const _sNil &) const { return true; }
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
        bool operator==(const _sNil &rhs) const {
            return false;
        }
        bool operator==(const _sCons &rhs) const {
            if(std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_)){
                return false;
            }else{
                return std::tie(p1_, *p2_) == std::tie(rhs.p1_, *rhs.p2_);
            }
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

    bool operator==(const slist<T1> &rhs) const { return value_ == rhs.value_; }
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



template<typename T1>
slist<T1> Modify1(T1 arg1, T1 arg2, slist<T1> arg3) {
    // Modify1 a b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Modify1 a b (sCons x xs) = (if x=a then (sCons b (Modify1 a b xs)) else (sCons x (Modify1 a b xs)))
    auto a = std::move(arg1);
    auto b = std::move(arg2);
    auto x = std::move(arg3.as_sCons().p1_);
    auto xs = std::move(*arg3.as_sCons().p2_);
    slist<T1> temp0;
    if (x == a) {
        auto temp1 = slist<T1>::sCons(
            b,
            Modify1(std::move(a), b, std::move(xs))
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

// generated by HOL2Cpp
