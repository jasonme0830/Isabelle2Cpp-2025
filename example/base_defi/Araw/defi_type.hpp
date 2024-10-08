#include <cstdint>
#include <cstdlib>
#include <memory>
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


        _sSuc(const snat &p1 )
            :p1_(std::make_shared<snat>(p1))
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
class stree {
    struct _sTip {
        _sTip() {}
        _sTip(const _sTip& other){ }
        _sTip(_sTip&& other) noexcept{ }
        _sTip& operator=(const _sTip& other) { return *this; }
        _sTip& operator=(_sTip&& other) noexcept { return *this; }
        bool operator==(const _sTip &) const { return true; }
        bool operator<(const _sTip &) const { return false; }
        bool operator>(const _sTip &) const { return false; }
        bool operator<=(const _sTip &) const { return true; }
        bool operator>=(const _sTip &) const { return true; }
    };
    struct _sNode {
        std::shared_ptr<stree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<stree<T1>> p3_;

        stree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        stree<T1> p3() const { return *p3_; }


        _sNode(const stree<T1> &p1, const T1 &p2, const stree<T1> &p3 )
            :p1_(std::make_shared<stree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<stree<T1>>(p3))
        {}
        _sNode(const _sNode& other)
            :p1_(std::make_shared<stree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<stree<T1>>(*other.p3_))
        {}
        _sNode(_sNode&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        _sNode& operator=(const _sNode& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<stree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<stree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _sNode& operator=(_sNode&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
        bool operator==(const _sNode &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) == std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator<(const _sNode &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator>(const _sNode &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) > std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator<=(const _sNode &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) <= std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
        bool operator>=(const _sNode &rhs) const {
            return (std::tie(*p1_, p2_, *p3_) >= std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_));
        }
    };

  public:

    std::variant<_sTip, _sNode> value_;

    //默认构造函数
    stree(){
        value_ = _sTip();
    }
    //value构造函数
    stree(std::variant<_sTip, _sNode> value) : value_(value) {}
    //深拷贝构造函数
    stree(const stree<T1>& other) { 
        if(std::holds_alternative<_sTip>(other.value_)){ 
            const _sTip& other_node = std::get<_sTip>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_sNode>(other.value_)){ 
            const _sNode& other_node = std::get<_sNode>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    stree(stree<T1>&& other){
        if(std::holds_alternative<_sTip>(other.value_)){ 
            _sTip& other_node = std::get<_sTip>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_sNode>(other.value_)){ 
            _sNode& other_node = std::get<_sNode>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static stree<T1> sTip() {
        return stree<T1> ( _sTip (  ) );
    }
    static stree<T1> sNode(const stree<T1> &p1, const T1 &p2, const stree<T1> &p3) {
        return stree<T1> ( _sNode ( p1, p2, p3 ) );
    }

    bool is_sTip() const { return std::holds_alternative<_sTip>(value_); }
    bool is_sNode() const { return std::holds_alternative<_sNode>(value_); }
    const _sNode &as_sNode() const { return std::get<_sNode>(value_); }
    stree<T1>& operator=(stree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_sTip>(other.value_)){
                _sTip& other_value = std::get<_sTip>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_sNode>(other.value_)){
                _sNode& other_value = std::get<_sNode>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    stree<T1>& operator=(const stree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_sTip>(other.value_)){ 
                const _sTip& other_node = std::get<_sTip>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_sNode>(other.value_)){ 
                const _sNode& other_node = std::get<_sNode>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

    bool operator==(const stree<T1> &rhs) const {
        if(value_.index() == rhs.value_.index()){
             return value_ == rhs.value_;
        }else{
            return false;
        }
    }

    bool operator<(const stree<T1> &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ < rhs.value_; 
    }

    bool operator>(const stree<T1> &rhs) const {
        if(value_ == rhs.value_) return false;
        return value_ > rhs.value_; 
    }

    bool operator<=(const stree<T1> &rhs) const {
        if(value_ == rhs.value_) return true;
        return value_ < rhs.value_; 
    }

    bool operator>=(const stree<T1> &rhs) const {
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
slist<T1> Modify1(T1 arg1, T1 arg2, slist<T1> arg3) {
    // Modify1 a b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Modify1 a b (sCons x xs) = (if x=a then (sCons b (Modify1 a b xs)) else (sCons x (Modify1 a b xs)))
    auto a = arg1;
    auto b = arg2;
    auto x = arg3.as_sCons().p1();
    auto xs = arg3.as_sCons().p2();
    slist<T1> temp0;
    if (x == a) {
        auto temp1 = slist<T1>::sCons(
            b,
            Modify1(a, b, xs)
        );
        temp0 = temp1;
    } else {
        auto temp2 = slist<T1>::sCons(
            x,
            Modify1(a, b, xs)
        );
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
slist<T1> Modify2(snat arg1, T1 arg2, slist<T1> arg3) {
    // Modify2 n b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Modify2 sZero b (sCons x xs) =(sCons b xs)
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

    // Modify2 (sSuc n) b (sCons x xs) = (sCons x (Modify2 n b xs))
    auto n = arg1.as_sSuc().p1();
    auto b = arg2;
    auto x = arg3.as_sCons().p1();
    auto xs = arg3.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        Modify2(n, b, xs)
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
stree<T1> copy_tree(stree<T1> arg1) {
    // copy_tree (sNode left x right) = sNode (copy_tree left) x (copy_tree right)
    if (arg1.is_sNode()) {
        auto left = arg1.as_sNode().p1();
        auto x = arg1.as_sNode().p2();
        auto right = arg1.as_sNode().p3();
        auto temp0 = stree<T1>::sNode(
            copy_tree(left),
            x,
            copy_tree(right)
        );
        return temp0;
    }

    // copy_tree sTip = sTip
    return stree<T1>::sTip();
}

template<typename T1>
bool searchtree1(T1 arg1, stree<T1> arg2) {
    // searchtree1 a sTip=False
    if (arg2.is_sTip()) {
        return false;
    }

    // searchtree1 a ( sNode left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right))
    auto a = arg1;
    auto left = arg2.as_sNode().p1();
    auto x = arg2.as_sNode().p2();
    auto right = arg2.as_sNode().p3();
    return (a == x) || (searchtree1(a, left) || searchtree1(a, right));
}

template<typename T1>
bool searchtree2(T1 arg1, stree<T1> arg2) {
    // searchtree2 a sTip=False
    if (arg2.is_sTip()) {
        return false;
    }

    // searchtree2 a ( sNode left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right))
    auto a = arg1;
    auto left = arg2.as_sNode().p1();
    auto x = arg2.as_sNode().p2();
    auto right = arg2.as_sNode().p3();
    return searchtree2(a, left) || ((a == x) || searchtree2(a, right));
}

template<typename T1>
bool searchtree3(T1 arg1, stree<T1> arg2) {
    // searchtree3 a sTip=False
    if (arg2.is_sTip()) {
        return false;
    }

    // searchtree3 a ( sNode left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) )
    auto a = arg1;
    auto left = arg2.as_sNode().p1();
    auto x = arg2.as_sNode().p2();
    auto right = arg2.as_sNode().p3();
    return searchtree3(a, left) || (searchtree3(a, right) || (a == x));
}

template<typename T1>
stree<T1> inserttree(T1 arg1, stree<T1> arg2) {
    // inserttree a sTip= sNode sTip a sTip
    if (arg2.is_sTip()) {
        auto a = arg1;
        auto temp0 = stree<T1>::sNode(
            stree<T1>::sTip(),
            a,
            stree<T1>::sTip()
        );
        return temp0;
    }

    // inserttree a ( sNode left x right) = (if a\<le>x then ( sNode (inserttree a left) x right)  ...
    auto a = arg1;
    auto left = arg2.as_sNode().p1();
    auto x = arg2.as_sNode().p2();
    auto right = arg2.as_sNode().p3();
    stree<T1> temp0;
    if (a <= x) {
        auto temp1 = stree<T1>::sNode(
            inserttree(a, left),
            x,
            right
        );
        temp0 = temp1;
    } else {
        auto temp2 = stree<T1>::sNode(
            left,
            x,
            inserttree(a, right)
        );
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
T1 rightest(stree<T1> arg1) {
    // rightest ( sNode left x right) = (if right= sTip then x  ...
    auto x = arg1.as_sNode().p2();
    auto right = arg1.as_sNode().p3();
    T1 temp0;
    if (right.is_sTip()) {
        temp0 = x;
    } else {
        temp0 = rightest(right);
    }
    return temp0;
}

template<typename T1>
stree<T1> rightestleft(stree<T1> arg1) {
    // rightestleft sTip= sTip
    if (arg1.is_sTip()) {
        return stree<T1>::sTip();
    }

    // rightestleft ( sNode left x right) =(if right= sTip then left  ...
    auto left = arg1.as_sNode().p1();
    auto right = arg1.as_sNode().p3();
    stree<T1> temp0;
    if (right.is_sTip()) {
        temp0 = left;
    } else {
        temp0 = rightestleft(right);
    }
    return temp0;
}

template<typename T1>
stree<T1> deltreeroot(stree<T1> arg1) {
    // deltreeroot sTip= sTip
    if (arg1.is_sTip()) {
        return stree<T1>::sTip();
    }

    // deltreeroot ( sNode left x right) =(if right= sTip then left else if left= sTip then right else( sNode (rightestleft left)(rightest left)right ) )
    auto left = arg1.as_sNode().p1();
    auto right = arg1.as_sNode().p3();
    stree<T1> temp0;
    if (right.is_sTip()) {
        temp0 = left;
    } else {
        stree<T1> temp1;
        if (left.is_sTip()) {
            temp1 = right;
        } else {
            auto temp2 = stree<T1>::sNode(
                rightestleft(left),
                rightest(left),
                right
            );
            temp1 = temp2;
        }
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
stree<T1> deltree(T1 arg1, stree<T1> arg2) {
    // deltree a sTip = sTip
    if (arg2.is_sTip()) {
        return stree<T1>::sTip();
    }

    // deltree a ( sNode left x right) =( if a=x then(deltreeroot( sNode left x right)) ...
    auto a = arg1;
    auto left = arg2.as_sNode().p1();
    auto x = arg2.as_sNode().p2();
    auto right = arg2.as_sNode().p3();
    stree<T1> temp0;
    if (a == x) {
        auto temp1 = stree<T1>::sNode(
            left,
            x,
            right
        );
        temp0 = deltreeroot(temp1);
    } else {
        stree<T1> temp2;
        if (a < x) {
            auto temp3 = stree<T1>::sNode(
                deltree(a, left),
                x,
                right
            );
            temp2 = temp3;
        } else {
            auto temp4 = stree<T1>::sNode(
                left,
                x,
                deltree(a, right)
            );
            temp2 = temp4;
        }
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
stree<T1> changetree(T1 arg1, T1 arg2, stree<T1> arg3) {
    // changetree a b sTip = sTip
    if (arg3.is_sTip()) {
        return stree<T1>::sTip();
    }

    // changetree a b ( sNode left x right) = ( if (a=x) then ( sNode left b right) ...
    auto a = arg1;
    auto b = arg2;
    auto left = arg3.as_sNode().p1();
    auto x = arg3.as_sNode().p2();
    auto right = arg3.as_sNode().p3();
    stree<T1> temp0;
    if (a == x) {
        auto temp1 = stree<T1>::sNode(
            left,
            b,
            right
        );
        temp0 = temp1;
    } else {
        stree<T1> temp2;
        if (a < x) {
            auto temp3 = stree<T1>::sNode(
                changetree(a, b, left),
                x,
                right
            );
            temp2 = temp3;
        } else {
            auto temp4 = stree<T1>::sNode(
                left,
                x,
                changetree(a, b, right)
            );
            temp2 = temp4;
        }
        temp0 = temp2;
    }
    return temp0;
}

// generated by HOL2Cpp
