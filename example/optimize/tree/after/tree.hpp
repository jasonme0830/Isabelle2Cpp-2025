#include <cstdlib>
#include <deque>
#include <memory>
#include <utility>
#include <variant>

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



template<typename T1>
tree<T1> copy_tree(tree<T1> arg1) {
    // copy_tree (Node left x right) = Node (copy_tree left) x (copy_tree right)
    if (arg1.is_Node()) {
        auto left = std::move(*arg1.as_Node().p1_);
        auto x = std::move(arg1.as_Node().p2_);
        auto right = std::move(*arg1.as_Node().p3_);
        auto temp0 = tree<T1>::Node(
            copy_tree(std::move(left)),
            std::move(x),
            copy_tree(std::move(right))
        );
        return temp0;
    }

    // copy_tree Tip = Tip
    return tree<T1>::Tip();
}

template<typename T1>
bool searchtree1(T1 arg1, tree<T1> arg2) {
    // searchtree1 a Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right))
    auto a = std::move(arg1);
    auto left = std::move(*arg2.as_Node().p1_);
    auto x = std::move(arg2.as_Node().p2_);
    auto right = std::move(*arg2.as_Node().p3_);
    return (a == x) || (searchtree1(a, std::move(left)) || searchtree1(std::move(a), std::move(right)));
}

template<typename T1>
bool searchtree2(T1 arg1, tree<T1> arg2) {
    // searchtree2 a Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtree2 a (Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right))
    auto a = std::move(arg1);
    auto left = std::move(*arg2.as_Node().p1_);
    auto x = std::move(arg2.as_Node().p2_);
    auto right = std::move(*arg2.as_Node().p3_);
    return searchtree2(a, std::move(left)) || ((a == x) || searchtree2(std::move(a), std::move(right)));
}

template<typename T1>
bool searchtree3(T1 arg1, tree<T1> arg2) {
    // searchtree3 a Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtree3 a (Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) )
    auto a = std::move(arg1);
    auto left = std::move(*arg2.as_Node().p1_);
    auto x = std::move(arg2.as_Node().p2_);
    auto right = std::move(*arg2.as_Node().p3_);
    return searchtree3(a, std::move(left)) || (searchtree3(a, std::move(right)) || (a == x));
}

template<typename T1>
tree<T1> inserttree(T1 arg1, tree<T1> arg2) {
    // inserttree a Tip = Node Tip a Tip
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
std::deque<T1> transtolist(tree<T1> arg1) {
    // transtolist Tip = []
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // transtolist (Node left a right) =( a # (transtolist left)@(transtolist right))
    auto left = std::move(*arg1.as_Node().p1_);
    auto a = std::move(arg1.as_Node().p2_);
    auto right = std::move(*arg1.as_Node().p3_);
    auto temp0 = transtolist(std::move(left));
    auto temp1 = transtolist(std::move(right));
    temp0.insert(temp0.end(), temp1.begin(), temp1.end());
    auto temp2 = std::move(temp0);
    temp2.push_front(std::move(a));
    return temp2;
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
    // rightestleft Tip = Tip
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
tree<T1> deltreeroot(tree<T1> arg1) {
    // deltreeroot Tip = Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // deltreeroot (Node left x right) =(if right=Tip then left else if left=Tip then right else(Node (rightestleft left)(rightest left)right ) )
    auto left = std::move(*arg1.as_Node().p1_);
    auto right = std::move(*arg1.as_Node().p3_);
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = std::move(left);
    } else {
        tree<T1> temp1;
        if (left.is_Tip()) {
            temp1 = std::move(right);
        } else {
            auto temp2 = left;
            auto temp3 = left;
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
tree<T1> addtreeroot(T1 arg1, tree<T1> arg2) {
    // addtreeroot x Tip = Node Tip x Tip
    if (arg2.is_Tip()) {
        auto x = std::move(arg1);
        auto temp0 = tree<T1>::Node(
            tree<T1>::Tip(),
            std::move(x),
            tree<T1>::Tip()
        );
        return temp0;
    }

    // addtreeroot x (Node left m Tip) = Node left x (Node Tip m Tip)
    if (arg2.is_Node()) {
        if (std::move(*arg2.as_Node().p3_).is_Tip()) {
            auto x = std::move(arg1);
            auto left = std::move(*arg2.as_Node().p1_);
            auto m = std::move(arg2.as_Node().p2_);
            auto temp0 = tree<T1>::Node(
                tree<T1>::Tip(),
                std::move(m),
                tree<T1>::Tip()
            );
            auto temp1 = tree<T1>::Node(
                std::move(left),
                std::move(x),
                std::move(temp0)
            );
            return temp1;
        }
    }

    // addtreeroot x (Node Tip m right) = Node (Node Tip m Tip) x right
    if (arg2.is_Node()) {
        if (std::move(*arg2.as_Node().p1_).is_Tip()) {
            auto x = std::move(arg1);
            auto m = std::move(arg2.as_Node().p2_);
            auto right = std::move(*arg2.as_Node().p3_);
            auto temp0 = tree<T1>::Node(
                tree<T1>::Tip(),
                std::move(m),
                tree<T1>::Tip()
            );
            auto temp1 = tree<T1>::Node(
                std::move(temp0),
                std::move(x),
                std::move(right)
            );
            return temp1;
        }
    }

    // addtreeroot x (Node left m right) = (Node (Node left m right) x Tip)
    auto x = std::move(arg1);
    auto left = std::move(*arg2.as_Node().p1_);
    auto m = std::move(arg2.as_Node().p2_);
    auto right = std::move(*arg2.as_Node().p3_);
    auto temp0 = tree<T1>::Node(
        std::move(left),
        std::move(m),
        std::move(right)
    );
    auto temp1 = tree<T1>::Node(
        std::move(temp0),
        std::move(x),
        tree<T1>::Tip()
    );
    return temp1;
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

template<typename T1>
std::deque<T1> sorttree(tree<T1> arg1) {
    // sorttree Tip = []
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // sorttree (Node Tip a Tip) =[a]
    if (arg1.is_Node()) {
        if (std::move(*arg1.as_Node().p1_).is_Tip()) {
            if (std::move(*arg1.as_Node().p3_).is_Tip()) {
                auto a = std::move(arg1.as_Node().p2_);
                return std::deque<T1>{std::move(a)};
            }
        }
    }

    // sorttree (Node left x right) =(MergeSort (transtolist (Node left x right)))
    auto left = std::move(*arg1.as_Node().p1_);
    auto x = std::move(arg1.as_Node().p2_);
    auto right = std::move(*arg1.as_Node().p3_);
    auto temp0 = tree<T1>::Node(
        std::move(left),
        std::move(x),
        std::move(right)
    );
    return MergeSort(transtolist(std::move(temp0)));
}

// generated by HOL2Cpp
