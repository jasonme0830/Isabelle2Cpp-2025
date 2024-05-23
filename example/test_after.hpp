#include <cstdint>
#include <cstdlib>
#include <deque>
#include <map>
#include <memory>
#include <variant>

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



template<typename T1>
bool searchtree1(const T1 &arg1, const tree<T1> &arg2) {
    auto impl = [&]() -> bool {
        // searchtree1 a Tip=False
        if (arg2.is_Tip()) {
            return false;
        }

        // searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right))
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        auto temp0 = std::move(arg1);
        auto temp1 = std::move(right);
        auto temp2 = arg1;
        auto temp3 = std::move(left);
        return (arg1 == std::move(x)) || (searchtree1(std::move(temp2), std::move(temp3)) || searchtree1(std::move(temp0), std::move(temp1)));
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
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        auto temp0 = std::move(arg1);
        auto temp1 = std::move(right);
        auto temp2 = arg1;
        auto temp3 = std::move(left);
        return searchtree2(std::move(temp2), std::move(temp3)) || ((arg1 == std::move(x)) || searchtree2(std::move(temp0), std::move(temp1)));
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
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        auto temp0 = arg1;
        auto temp1 = std::move(right);
        auto temp2 = arg1;
        auto temp3 = std::move(left);
        return searchtree3(std::move(temp2), std::move(temp3)) || (searchtree3(std::move(temp0), std::move(temp1)) || (std::move(arg1) == std::move(x)));
    };

    static std::map<std::tuple<T1, tree<T1>>, bool> cache;
    auto args = std::make_tuple(arg1, arg2);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
tree<T1> inserttree(T1 arg1, tree<T1> arg2) {
    // inserttree a (Node left x right) = (if a\<le>x then (Node (inserttree a left) x right)  ...
    if (arg2.is_Node()) {
        auto left = std::move(arg2.as_Node().p1());
        auto x = std::move(arg2.as_Node().p2());
        auto right = std::move(arg2.as_Node().p3());
        tree<T1> temp0;
        if (arg1 <= x) {
            auto temp1 = std::move(arg1);
            auto temp2 = std::move(left);
            auto temp3 = tree<T1>::Node(
                inserttree(std::move(temp1), std::move(temp2)), std::move(x), std::move(right)
            );
            temp0 = temp3;
        } else {
            auto temp4 = std::move(arg1);
            auto temp5 = std::move(right);
            auto temp6 = tree<T1>::Node(
                std::move(left), std::move(x), inserttree(std::move(temp4), std::move(temp5))
            );
            temp0 = temp6;
        }
        return temp0;
    }

    // inserttree a Tip= Node Tip a Tip
    auto temp0 = tree<T1>::Node(
        tree<T1>::Tip(), std::move(arg1), tree<T1>::Tip()
    );
    return temp0;
}

template<typename T1>
std::deque<T1> transtolist(const tree<T1> &arg1) {
    auto impl = [&]() -> std::deque<T1> {
        // transtolist Tip=[]
        if (arg1.is_Tip()) {
            return std::deque<T1>();
        }

        // transtolist (Node left a right) =( a # (transtolist left)@(transtolist right))
        auto left = arg1.as_Node().p1();
        auto a = arg1.as_Node().p2();
        auto right = arg1.as_Node().p3();
        auto temp0 = std::move(left);
        auto temp1 = std::move(right);
        auto temp2 = transtolist(std::move(temp0));
        auto temp3 = transtolist(std::move(temp1));
        temp2.insert(temp2.end(), temp3.begin(), temp3.end());
        auto temp4 = temp2;
        temp4.push_front(std::move(a));
        return temp4;
    };

    static std::map<std::tuple<tree<T1>>, std::deque<T1>> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
T1 rightest(tree<T1> arg1) {
    // rightest (Node left x right) = (if right=Tip then x  ...
    auto x = std::move(arg1.as_Node().p2());
    auto right = std::move(arg1.as_Node().p3());
    T1 temp0;
    if (right.is_Tip()) {
        temp0 = std::move(x);
    } else {
        auto temp1 = std::move(right);
        temp0 = rightest(std::move(temp1));
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
    auto left = std::move(arg1.as_Node().p1());
    auto right = std::move(arg1.as_Node().p3());
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = std::move(left);
    } else {
        auto temp1 = std::move(right);
        temp0 = rightestleft(std::move(temp1));
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
                rightestleft(std::move(temp2)), rightest(std::move(temp3)), std::move(right)
            );
            temp1 = temp4;
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
    auto left = std::move(arg2.as_Node().p1());
    auto x = std::move(arg2.as_Node().p2());
    auto right = std::move(arg2.as_Node().p3());
    tree<T1> temp0;
    if (arg1 == x) {
        auto temp2 = tree<T1>::Node(
            std::move(left), std::move(x), std::move(right)
        );
        auto temp1 = temp2;
        temp0 = deltreeroot(std::move(temp1));
    } else {
        tree<T1> temp3;
        if (arg1 < x) {
            auto temp4 = std::move(arg1);
            auto temp5 = std::move(left);
            auto temp6 = tree<T1>::Node(
                deltree(std::move(temp4), std::move(temp5)), std::move(x), std::move(right)
            );
            temp3 = temp6;
        } else {
            auto temp7 = std::move(arg1);
            auto temp8 = std::move(right);
            auto temp9 = tree<T1>::Node(
                std::move(left), std::move(x), deltree(std::move(temp7), std::move(temp8))
            );
            temp3 = temp9;
        }
        temp0 = temp3;
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
    auto left = std::move(arg3.as_Node().p1());
    auto x = std::move(arg3.as_Node().p2());
    auto right = std::move(arg3.as_Node().p3());
    tree<T1> temp0;
    if (arg1 == x) {
        auto temp1 = tree<T1>::Node(
            std::move(left), std::move(arg2), std::move(right)
        );
        temp0 = temp1;
    } else {
        tree<T1> temp2;
        if (arg1 < x) {
            auto temp3 = std::move(arg1);
            auto temp4 = std::move(arg2);
            auto temp5 = std::move(left);
            auto temp6 = tree<T1>::Node(
                changetree(std::move(temp3), std::move(temp4), std::move(temp5)), std::move(x), std::move(right)
            );
            temp2 = temp6;
        } else {
            auto temp7 = std::move(arg1);
            auto temp8 = std::move(arg2);
            auto temp9 = std::move(right);
            auto temp10 = tree<T1>::Node(
                std::move(left), std::move(x), changetree(std::move(temp7), std::move(temp8), std::move(temp9))
            );
            temp2 = temp10;
        }
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
std::deque<T1> Merge(std::deque<T1> arg1, std::deque<T1> arg2) {
    // Merge [] xs=xs
    if (arg1.empty()) {
        return arg2;
    }

    // Merge xs [] = xs
    if (arg2.empty()) {
        return arg1;
    }

    // Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys))
    auto x = std::move(arg1.front());
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(std::move(arg1));
    auto y = std::move(arg2.front());
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto ys = std::move(std::move(arg2));
    std::deque<T1> temp0;
    if (x <= y) {
        auto temp1 = std::move(xs);
        auto temp3 = std::move(ys);
        temp3.push_front(std::move(y));
        auto temp2 = std::move(temp3);
        auto temp4 = Merge(std::move(temp1), std::move(temp2));
        temp4.push_front(std::move(x));
        temp0 = std::move(temp4);
    } else {
        auto temp6 = std::move(xs);
        temp6.push_front(std::move(x));
        auto temp5 = std::move(temp6);
        auto temp7 = std::move(ys);
        auto temp8 = Merge(std::move(temp5), std::move(temp7));
        temp8.push_front(std::move(y));
        temp0 = std::move(temp8);
    }
    return temp0;
}

template<typename T1>
std::deque<T1> MergeSort(const std::deque<T1> &arg1) {
    auto impl = [&]() -> std::deque<T1> {
        // MergeSort [] = []
        if (arg1.empty()) {
            return std::deque<T1>();
        }

        // MergeSort (Cons a []) = [a]
        if (!arg1.empty()) {
            if (std::deque<T1>(arg1.begin() + 1, arg1.end()).empty()) {
                auto a = arg1.front();
                return std::deque<T1>{std::move(a)};
            }
        }

        // MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs))
        auto temp2 = arg1;
        auto temp1 = std::deque<T1>(arg1.begin(), arg1.begin() + size(std::move(temp2)) / 2);
        auto temp0 = MergeSort(std::move(temp1));
        auto temp5 = arg1;
        auto temp6 = size(std::move(temp5)) / 2;
        auto temp7 = std::move(arg1);
        temp7.erase(temp7.begin(), std::next(temp7.begin(), temp6));
        auto temp4 = std::move(temp7);
        auto temp3 = MergeSort(std::move(temp4));
        return Merge(std::move(temp0), std::move(temp3));
    };

    static std::map<std::tuple<std::deque<T1>>, std::deque<T1>> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
std::deque<T1> sorttree(const tree<T1> &arg1) {
    // sorttree Tip = []
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // sorttree (Node Tip a Tip) =[a]
    if (arg1.is_Node()) {
        if (arg1.as_Node().p1().is_Tip()) {
            if (arg1.as_Node().p3().is_Tip()) {
                auto a = arg1.as_Node().p2();
                return std::deque<T1>{std::move(a)};
            }
        }
    }

    // sorttree (Node left x right) =(MergeSort (transtolist (Node left x right)))
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp2 = tree<T1>::Node(
        std::move(left), std::move(x), std::move(right)
    );
    auto temp1 = temp2;
    auto temp0 = transtolist(std::move(temp1));
    return MergeSort(std::move(temp0));
}

// generated by HOL2Cpp
