#include <cstdint>
#include <cstdlib>
#include <map>
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
class tree {
    struct _Tip {
        _Tip() {}
        _Tip(const _Tip& other){ }
        _Tip(_Tip&& other) noexcept{ }
        bool operator<(const _Tip &) const { return false; }
        _Tip& operator=(const _Tip& other) { return *this; }
        _Tip& operator=(_Tip&& other) noexcept { return *this; }
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

        bool operator<(const _Node &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
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

    bool operator<(const tree<T1> &rhs) const { return value_ < rhs.value_; }
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

};



std::uint64_t natofsnat(snat arg1);

snat snatofnat(std::uint64_t arg1);

template<typename T1>
bool searchtree1(const T1 &arg1, const tree<T1> &arg2) {
    auto impl = [&]() -> bool {
        // searchtree1 a Tip=False
        if (arg2.is_Tip()) {
            return false;
        }

        // searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right))
        auto a = arg1;
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        return (a == std::move(x)) || (searchtree1(a, std::move(left)) || searchtree1(std::move(a), std::move(right)));
    };

    static std::map<std::tuple<T1, tree<T1>>, bool> cache;
    auto args = std::make_tuple(arg1, arg2);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
bool searchtree2(const T1 &arg1, const tree<T1> &arg2) {
    auto impl = [&]() -> bool {
        // searchtree2 a Tip=False
        if (arg2.is_Tip()) {
            return false;
        }

        // searchtree2 a (Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right))
        auto a = arg1;
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        return searchtree2(a, std::move(left)) || ((a == std::move(x)) || searchtree2(std::move(a), std::move(right)));
    };

    static std::map<std::tuple<T1, tree<T1>>, bool> cache;
    auto args = std::make_tuple(arg1, arg2);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
bool searchtree3(const T1 &arg1, const tree<T1> &arg2) {
    auto impl = [&]() -> bool {
        // searchtree3 a Tip=False
        if (arg2.is_Tip()) {
            return false;
        }

        // searchtree3 a (Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) )
        auto a = arg1;
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        return searchtree3(a, std::move(left)) || (searchtree3(a, std::move(right)) || (std::move(a) == std::move(x)));
    };

    static std::map<std::tuple<T1, tree<T1>>, bool> cache;
    auto args = std::make_tuple(arg1, arg2);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
tree<T1> inserttree(T1 arg1, tree<T1> arg2) {
    // inserttree a Tip= Node Tip a Tip
    if (arg2.is_Tip()) {
        auto a = std::move(arg1);
        auto temp0 = tree<T1>::Node(
            tree<T1>::Tip(),
            std::move(a),
            tree<T1>::Tip()
        );
        return temp0;
    }

    // inserttree a (Node left x right) = (if a\<le>x then (Node (inserttree a left) x right)  ...
    auto a = std::move(arg1);
    auto left = std::move(*arg2.as_Node().p1_);
    auto x = std::move(arg2.as_Node().p2_);
    auto right = std::move(*arg2.as_Node().p3_);
    tree<T1> temp0;
    if (a <= x) {
        auto temp1 = tree<T1>::Node(
            inserttree(std::move(a), std::move(left)),
            std::move(x),
            std::move(right)
        );
        temp0 = std::move(temp1);
    } else {
        auto temp2 = tree<T1>::Node(
            std::move(left),
            std::move(x),
            inserttree(std::move(a), std::move(right))
        );
        temp0 = std::move(temp2);
    }
    return temp0;
}

template<typename T1>
T1 rightest(tree<T1> arg1) {
    // rightest (Node left x right) = (if right=Tip then x  ...
    auto x = std::move(arg1.as_Node().p2_);
    auto right = std::move(*arg1.as_Node().p3_);
    T1 temp0;
    if (right.is_Tip()) {
        temp0 = std::move(x);
    } else {
        temp0 = rightest(std::move(right));
    }
    return temp0;
}

template<typename T1>
tree<T1> rightestleft(tree<T1> arg1) {
    // rightestleft Tip=Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // rightestleft (Node left x right) =(if right=Tip then left  ...
    auto left = std::move(*arg1.as_Node().p1_);
    auto right = std::move(*arg1.as_Node().p3_);
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = std::move(left);
    } else {
        temp0 = rightestleft(std::move(right));
    }
    return temp0;
}

template<typename T1>
tree<T1> deltreeroot(const tree<T1> &arg1) {
    // deltreeroot Tip=Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // deltreeroot (Node left x right) =(if right=Tip then left else if left=Tip then right else(Node (rightestleft left)(rightest left)right ) )
    auto left = arg1.as_Node().p1();
    auto right = arg1.as_Node().p3();
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = std::move(left);
    } else {
        tree<T1> temp1;
        if (left.is_Tip()) {
            temp1 = std::move(right);
        } else {
            auto temp2 = left;
            auto temp3 = std::move(left);
            auto temp4 = tree<T1>::Node(
                rightestleft(std::move(temp2)),
                rightest(std::move(temp3)),
                std::move(right)
            );
            temp1 = std::move(temp4);
        }
        temp0 = std::move(temp1);
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
    auto a = std::move(arg1);
    auto left = std::move(*arg2.as_Node().p1_);
    auto x = std::move(arg2.as_Node().p2_);
    auto right = std::move(*arg2.as_Node().p3_);
    tree<T1> temp0;
    if (a == x) {
        auto temp1 = tree<T1>::Node(
            std::move(left),
            std::move(x),
            std::move(right)
        );
        temp0 = deltreeroot(std::move(temp1));
    } else {
        tree<T1> temp2;
        if (a < x) {
            auto temp3 = tree<T1>::Node(
                deltree(std::move(a), std::move(left)),
                std::move(x),
                std::move(right)
            );
            temp2 = std::move(temp3);
        } else {
            auto temp4 = tree<T1>::Node(
                std::move(left),
                std::move(x),
                deltree(std::move(a), std::move(right))
            );
            temp2 = std::move(temp4);
        }
        temp0 = std::move(temp2);
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
    auto a = std::move(arg1);
    auto b = std::move(arg2);
    auto left = std::move(*arg3.as_Node().p1_);
    auto x = std::move(arg3.as_Node().p2_);
    auto right = std::move(*arg3.as_Node().p3_);
    tree<T1> temp0;
    if (a == x) {
        auto temp1 = tree<T1>::Node(
            std::move(left),
            std::move(b),
            std::move(right)
        );
        temp0 = std::move(temp1);
    } else {
        tree<T1> temp2;
        if (a < x) {
            auto temp3 = tree<T1>::Node(
                changetree(std::move(a), std::move(b), std::move(left)),
                std::move(x),
                std::move(right)
            );
            temp2 = std::move(temp3);
        } else {
            auto temp4 = tree<T1>::Node(
                std::move(left),
                std::move(x),
                changetree(std::move(a), std::move(b), std::move(right))
            );
            temp2 = std::move(temp4);
        }
        temp0 = std::move(temp2);
    }
    return temp0;
}

// generated by HOL2Cpp
