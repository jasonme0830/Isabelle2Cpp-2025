#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <variant>

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


        _Node(const tree<T1> &p1, const T1 &p2, const tree<T1> &p3 )
            :p1_(std::make_shared<tree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<tree<T1>>(p3))
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
    tree(std::variant<_Tip, _Node> value) : value_(value) {}
    //深拷贝构造函数
    tree(const tree<T1>& other) { 
        if(std::holds_alternative<_Tip>(other.value_)){ 
            const _Tip& other_node = std::get<_Tip>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_Node>(other.value_)){ 
            const _Node& other_node = std::get<_Node>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    tree(tree<T1>&& other){
        if(std::holds_alternative<_Tip>(other.value_)){ 
            _Tip& other_node = std::get<_Tip>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_Node>(other.value_)){ 
            _Node& other_node = std::get<_Node>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static tree<T1> Tip() {
        return tree<T1> ( _Tip (  ) );
    }
    static tree<T1> Node(const tree<T1> &p1, const T1 &p2, const tree<T1> &p3) {
        return tree<T1> ( _Node ( p1, p2, p3 ) );
    }

    bool is_Tip() const { return std::holds_alternative<_Tip>(value_); }
    bool is_Node() const { return std::holds_alternative<_Node>(value_); }
    const _Node &as_Node() const { return std::get<_Node>(value_); }
    tree<T1>& operator=(tree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_Tip>(other.value_)){
                _Tip& other_value = std::get<_Tip>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_Node>(other.value_)){
                _Node& other_value = std::get<_Node>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    tree<T1>& operator=(const tree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_Tip>(other.value_)){ 
                const _Tip& other_node = std::get<_Tip>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_Node>(other.value_)){ 
                const _Node& other_node = std::get<_Node>(other.value_); 
                value_ = other.value_; 
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
slist<T1> AddListHead(T1 arg1, slist<T1> arg2) {
    // AddListHead a xs = sCons a xs
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

    // AddListTail a (sCons x xs) = sCons x (AddListTail a xs )
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
slist<T1> AddListI(std::uint64_t arg1, T1 arg2, slist<T1> arg3) {
    // AddListI i a sNil = sCons a sNil
    if (arg3.is_sNil()) {
        auto a = arg2;
        auto temp0 = slist<T1>::sCons(
            a,
            slist<T1>::sNil()
        );
        return temp0;
    }

    // AddListI 0 a (sCons x xs) = AddListHead a (sCons x xs)
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

    // AddListI (Suc i) a (sCons x xs) = sCons x (AddListI i a xs)
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
slist<T1> DelListI(std::uint64_t arg1, slist<T1> arg2) {
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

    // DelListI 0 (sCons x xs) = DelListHead (sCons x xs)
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

    // DelListI (Suc i) (sCons x xs) = sCons x (DelListI i xs)
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
slist<T1> Modify1(T1 arg1, T1 arg2, slist<T1> arg3) {
    // Modify1 a b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Modify1 a b (sCons x xs) = (if x=a then sCons b (Modify1 a b xs) else sCons x (Modify1 a b xs))
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
slist<T1> Modify2(std::uint64_t arg1, T1 arg2, slist<T1> arg3) {
    // Modify2 n b sNil = sNil
    if (arg3.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Modify2 0 b (sCons x xs) =(sCons b xs)
    if (arg1 == 0) {
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

    // Modify2 (Suc n) b (sCons x xs) = (sCons x (Modify2 n b xs))
    auto n = arg1 - 1;
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
slist<T1> ffilter(std::function<bool(T1 )> arg1, slist<T1> arg2) {
    // ffilter f sNil = sNil
    if (arg2.is_sNil()) {
        return slist<T1>::sNil();
    }

    // ffilter f(sCons x xs) = (if (f x) then (sCons x (ffilter f xs))else(ffilter f xs ))
    auto f = arg1;
    auto x = arg2.as_sCons().p1();
    auto xs = arg2.as_sCons().p2();
    slist<T1> temp0;
    if (f(x)) {
        auto temp1 = slist<T1>::sCons(
            x,
            ffilter(f, xs)
        );
        temp0 = temp1;
    } else {
        temp0 = ffilter(f, xs);
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

    // app(sCons a as )bs= sCons a (app as bs)
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
    // Reverse sNil =sNil
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
slist<T1> Reverse2(slist<T1> arg1) {
    // Reverse2 sNil = sNil
    if (arg1.is_sNil()) {
        return slist<T1>::sNil();
    }

    // Reverse2 (sCons a sNil) =sCons a sNil
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            auto a = arg1.as_sCons().p1();
            auto temp0 = slist<T1>::sCons(
                a,
                slist<T1>::sNil()
            );
            return temp0;
        }
    }

    // Reverse2 (sCons x xs) = app ( Reverse2 xs) (sCons x sNil)
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto temp0 = slist<T1>::sCons(
        x,
        slist<T1>::sNil()
    );
    return app(Reverse2(xs), temp0);
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

    // Insert a (sCons x xs) =(if a \<le> x then sCons a (sCons x xs) else sCons x (Insert a xs))
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
slist<T1> InsertSortPart(slist<T1> arg1, slist<T1> arg2) {
    // InsertSortPart sNil ys = ys
    if (arg1.is_sNil()) {
        auto ys = arg2;
        return ys;
    }

    // InsertSortPart (sCons x xs) ys = InsertSortPart xs (Insert x ys)
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
std::uint64_t ssize(slist<T1> arg1) {
    // ssize sNil = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // ssize (sCons x sNil) = (Suc 0)
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            return 0 + 1;
        }
    }

    // ssize (sCons x xs) = Suc (ssize xs)
    auto xs = arg1.as_sCons().p2();
    return ssize(xs) + 1;
}

template<typename T1>
std::uint64_t slength(slist<T1> arg1) {
    // slength sNil = 0
    if (arg1.is_sNil()) {
        return 0;
    }

    // slength (sCons x sNil) = (Suc 0)
    if (arg1.is_sCons()) {
        if (arg1.as_sCons().p2().is_sNil()) {
            return 0 + 1;
        }
    }

    // slength (sCons x xs) = (Suc (slength xs))
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

    // stake (Suc n) (sCons x xs) =(sCons x (stake n xs))
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

    // sdrop (Suc n) (sCons x xs) = (sdrop n xs)
    auto n = arg1 - 1;
    auto xs = arg2.as_sCons().p2();
    return sdrop(n, xs);
}

template<typename T1>
slist<T1> Merge(slist<T1> arg1, slist<T1> arg2) {
    // Merge sNil xs = xs
    if (arg1.is_sNil()) {
        auto xs = arg2;
        return xs;
    }

    // Merge xs sNil = xs
    if (arg2.is_sNil()) {
        auto xs = arg1;
        return xs;
    }

    // Merge (sCons x xs)(sCons y ys) = (if x\<le>y then (sCons x (Merge xs (sCons y ys)) ) ...
    auto x = arg1.as_sCons().p1();
    auto xs = arg1.as_sCons().p2();
    auto y = arg2.as_sCons().p1();
    auto ys = arg2.as_sCons().p2();
    slist<T1> temp0;
    if (x <= y) {
        auto temp1 = slist<T1>::sCons(
            y,
            ys
        );
        auto temp2 = slist<T1>::sCons(
            x,
            Merge(xs, temp1)
        );
        temp0 = temp2;
    } else {
        auto temp3 = slist<T1>::sCons(
            x,
            xs
        );
        auto temp4 = slist<T1>::sCons(
            y,
            Merge(temp3, ys)
        );
        temp0 = temp4;
    }
    return temp0;
}

template<typename T1>
bool searchtree1(T1 arg1, tree<T1> arg2) {
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
bool searchtree2(T1 arg1, tree<T1> arg2) {
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
bool searchtree3(T1 arg1, tree<T1> arg2) {
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
tree<T1> inserttree(T1 arg1, tree<T1> arg2) {
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
    tree<T1> temp0;
    if (a <= x) {
        auto temp1 = tree<T1>::Node(
            inserttree(a, left),
            x,
            right
        );
        temp0 = temp1;
    } else {
        auto temp2 = tree<T1>::Node(
            left,
            x,
            inserttree(a, right)
        );
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
slist<T1> transtolist(tree<T1> arg1) {
    // transtolist Tip = sNil
    if (arg1.is_Tip()) {
        return slist<T1>::sNil();
    }

    // transtolist (Node left a right) =(sCons a (app (transtolist left) (transtolist right)))
    auto left = arg1.as_Node().p1();
    auto a = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp0 = slist<T1>::sCons(
        a,
        app(transtolist(left), transtolist(right))
    );
    return temp0;
}

template<typename T1>
T1 rightest(tree<T1> arg1) {
    // rightest (Node left x right) = (if right=Tip then x  ...
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    T1 temp0;
    if (right.is_Tip()) {
        temp0 = x;
    } else {
        temp0 = rightest(right);
    }
    return temp0;
}

template<typename T1>
tree<T1> rightestleft(tree<T1> arg1) {
    // rightestleft Tip = Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // rightestleft (Node left x right) =(if right=Tip then left  ...
    auto left = arg1.as_Node().p1();
    auto right = arg1.as_Node().p3();
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = left;
    } else {
        temp0 = rightestleft(right);
    }
    return temp0;
}

template<typename T1>
tree<T1> deltreeroot(tree<T1> arg1) {
    // deltreeroot Tip = Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // deltreeroot (Node left x right) =(if right=Tip then left else if left=Tip then right else(Node (rightestleft left)(rightest left)right ) )
    auto left = arg1.as_Node().p1();
    auto right = arg1.as_Node().p3();
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = left;
    } else {
        tree<T1> temp1;
        if (left.is_Tip()) {
            temp1 = right;
        } else {
            auto temp2 = tree<T1>::Node(
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
tree<T1> deltree(T1 arg1, tree<T1> arg2) {
    // deltree a Tip = Tip
    if (arg2.is_Tip()) {
        return tree<T1>::Tip();
    }

    // deltree a (Node left x right) =( if a=x then(deltreeroot(Node left x right)) ...
    auto a = arg1;
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    tree<T1> temp0;
    if (a == x) {
        auto temp1 = tree<T1>::Node(
            left,
            x,
            right
        );
        temp0 = deltreeroot(temp1);
    } else {
        tree<T1> temp2;
        if (a < x) {
            auto temp3 = tree<T1>::Node(
                deltree(a, left),
                x,
                right
            );
            temp2 = temp3;
        } else {
            auto temp4 = tree<T1>::Node(
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
tree<T1> changetree(T1 arg1, T1 arg2, tree<T1> arg3) {
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
    tree<T1> temp0;
    if (a == x) {
        auto temp1 = tree<T1>::Node(
            left,
            b,
            right
        );
        temp0 = temp1;
    } else {
        tree<T1> temp2;
        if (a < x) {
            auto temp3 = tree<T1>::Node(
                changetree(a, b, left),
                x,
                right
            );
            temp2 = temp3;
        } else {
            auto temp4 = tree<T1>::Node(
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
