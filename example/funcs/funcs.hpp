#include <cstdint>
#include <cstdlib>
#include <deque>
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

    std::variant<_Tip, _Node> value_;

  public:

    tree(const std::variant<_Tip, _Node> &value) : value_(value) {}
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

    static tree<T1> new_Tip() {
        return tree<T1> ( _Tip (  ) );
    }
    static tree<T1> new_Node(const tree<T1> &p1, const T1 &p2, const tree<T1> &p3) {
        return tree<T1> ( _Node ( p1, p2, p3 ) );
    }

    bool is_Tip() const { return std::holds_alternative<_Tip>(value_); }
    bool is_Node() const { return std::holds_alternative<_Node>(value_); }
    const _Node &as_Node() const { return std::get<_Node>(value_); }

    bool operator<(const tree<T1> &rhs) const { return value_ < rhs.value_; }
    tree<T1>& operator=(tree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_Tip>(other)){
                _Tip& other_value = std::get<_Tip>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_Node>(other)){
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
std::deque<T1> applist(const std::deque<T1> &arg1, const std::deque<T1> &arg2) {
    // applist Nil ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // applist (Cons x xs) ys = Cons x(applist xs ys)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = applist(xs, arg2);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> addlistH(const T1 &arg1, const std::deque<T1> &arg2) {
    // addlistH a Nil = (Cons a Nil)
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(arg1);
        return temp0;
    }

    // addlistH a xs = (Cons a xs)
    auto temp0 = arg2;
    temp0.push_front(arg1);
    return temp0;
}

template<typename T1>
std::deque<T1> addlistE(const T1 &arg1, const std::deque<T1> &arg2) {
    // addlistE a Nil = Cons a Nil
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(arg1);
        return temp0;
    }

    // addlistE a (Cons x xs) = Cons x (addlistE a xs)
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    auto temp0 = addlistE(arg1, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> addlistN(const T1 &arg1, const std::deque<T1> &arg2, const std::uint64_t &arg3) {
    // addlistN a Nil n = Cons a Nil
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(arg1);
        return temp0;
    }

    // addlistN a xs 0 = Cons a xs
    if (arg3 == 0) {
        auto temp0 = arg2;
        temp0.push_front(arg1);
        return temp0;
    }

    // addlistN a (Cons x xs) (Suc n) = (Cons x (addlistN a xs n))
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    auto n = arg3 - 1;
    auto temp0 = addlistN(arg1, xs, n);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> dellistH(const std::deque<T1> &arg1) {
    // dellistH Nil = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // dellistH (Cons x xs) = xs
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    return xs;
}

template<typename T1>
std::deque<T1> dellistE(const std::deque<T1> &arg1) {
    // dellistE Nil = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // dellistE (Cons x Nil) = Nil
    if (!arg1.empty()) {
        if (std::deque<T1>(arg1.begin() + 1, arg1.end()).empty()) {
            return std::deque<T1>();
        }
    }

    // dellistE (Cons x xs) = Cons x (dellistE xs)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = dellistE(xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> dellistN(const std::deque<T1> &arg1, const std::uint64_t &arg2) {
    // dellistN Nil n = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // dellistN (Cons x xs) 0 = xs
    if (!arg1.empty()) {
        if (arg2 == 0) {
            auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
            return xs;
        }
    }

    // dellistN (Cons x xs) (Suc n) = (Cons x (dellistN xs n))
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto n = arg2 - 1;
    auto temp0 = dellistN(xs, n);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> dellistF(const T1 &arg1, const std::deque<T1> &arg2) {
    // dellistF a Nil = Nil
    if (arg2.empty()) {
        return std::deque<T1>();
    }

    // dellistF a (Cons x xs) = (if a=x then xs  ...
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (arg1 == x) {
        temp0 = xs;
    } else {
        auto temp1 = dellistF(arg1, xs);
        temp1.push_front(x);
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
std::deque<T1> changelistF(const T1 &arg1, const std::deque<T1> &arg2, const T1 &arg3) {
    // changelistF a Nil b = Nil
    if (arg2.empty()) {
        return std::deque<T1>();
    }

    // changelistF a (Cons x xs) b = (if a=x then (Cons b xs) ...
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (arg1 == x) {
        auto temp1 = xs;
        temp1.push_front(arg3);
        temp0 = temp1;
    } else {
        auto temp2 = changelistF(arg1, xs, arg3);
        temp2.push_front(x);
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
std::deque<T1> changelistN(const std::deque<T1> &arg1, const std::uint64_t &arg2, const T1 &arg3) {
    // changelistN Nil n a = Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // changelistN (Cons x xs) 0 a = Cons a xs
    if (!arg1.empty()) {
        if (arg2 == 0) {
            auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
            auto temp0 = xs;
            temp0.push_front(arg3);
            return temp0;
        }
    }

    // changelistN (Cons x xs) (Suc n) a = Cons x (changelistN xs n a)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto n = arg2 - 1;
    auto temp0 = changelistN(xs, n, arg3);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
bool searchlist(const T1 &arg1, const std::deque<T1> &arg2) {
    // searchlist a Nil = False
    if (arg2.empty()) {
        return false;
    }

    // searchlist a (Cons x xs) = (if a=x then True else (searchlist a xs))
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    bool temp0;
    if (arg1 == x) {
        temp0 = true;
    } else {
        temp0 = searchlist(arg1, xs);
    }
    return temp0;
}

template<typename T1>
std::deque<T1> revlist(const std::deque<T1> &arg1, const std::deque<T1> &arg2) {
    // revlist Nil ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // revlist (Cons x xs) ys = revlist xs (Cons x ys)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = arg2;
    temp0.push_front(x);
    return revlist(xs, temp0);
}

template<typename T1>
std::deque<T1> insertsort(const T1 &arg1, const std::deque<T1> &arg2) {
    // insertsort a Nil = Cons a Nil
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(arg1);
        return temp0;
    }

    // insertsort a (Cons x xs) = (if a<x then (Cons a (Cons x xs)) ...
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (arg1 < x) {
        auto temp1 = xs;
        temp1.push_front(x);
        auto temp2 = temp1;
        temp2.push_front(arg1);
        temp0 = temp2;
    } else {
        auto temp3 = insertsort(arg1, xs);
        temp3.push_front(x);
        temp0 = temp3;
    }
    return temp0;
}

template<typename T1>
tree<T1> addtree(const T1 &arg1, const tree<T1> &arg2) {
    // addtree b Tip = (Node Tip b Tip)
    if (arg2.is_Tip()) {
        auto temp0 = tree<T1>::Node(
            tree<T1>::Tip(), arg1, tree<T1>::Tip()
        );
        return temp0;
    }

    // addtree b (Node left x right) = (if b=x then (Node left x right) ...
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    tree<T1> temp0;
    if (arg1 == x) {
        auto temp1 = tree<T1>::Node(
            left, x, right
        );
        temp0 = temp1;
    } else {
        tree<T1> temp2;
        if (arg1 < x) {
            temp2 = addtree(arg1, left);
        } else {
            temp2 = addtree(arg1, right);
        }
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
T1 smallest(const tree<T1> &arg1) {
    // smallest Tip = undefined
    if (arg1.is_Tip()) {
        return undefined;
    }

    // smallest (Node left x right) = (if left=Tip then x  ...
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    T1 temp0;
    if (left == tree<T1>::Tip()) {
        temp0 = x;
    } else {
        temp0 = smallest(left);
    }
    return temp0;
}

template<typename T1>
tree<T1> deltree(const tree<T1> &arg1, const T1 &arg2) {
    // deltree Tip n = Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // deltree (Node Tip x Tip) n = (if x=n then Tip ...
    if (arg1.is_Node()) {
        if (arg1.as_Node().p1().is_Tip()) {
            if (arg1.as_Node().p3().is_Tip()) {
                auto x = arg1.as_Node().p2();
                tree<T1> temp0;
                if (x == arg2) {
                    temp0 = tree<T1>::Tip();
                } else {
                    auto temp1 = tree<T1>::Node(
                        tree<T1>::Tip(), x, tree<T1>::Tip()
                    );
                    temp0 = temp1;
                }
                return temp0;
            }
        }
    }

    // deltree (Node left x Tip) n = (if x=n then left  ...
    if (arg1.is_Node()) {
        if (arg1.as_Node().p3().is_Tip()) {
            auto left = arg1.as_Node().p1();
            auto x = arg1.as_Node().p2();
            tree<T1> temp0;
            if (x == arg2) {
                temp0 = left;
            } else {
                auto temp1 = tree<T1>::Node(
                    left, x, tree<T1>::Tip()
                );
                temp0 = temp1;
            }
            return temp0;
        }
    }

    // deltree (Node left x right) n = (if n<x then deltree left n ...
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    tree<T1> temp0;
    if (arg2 < x) {
        temp0 = deltree(left, arg2);
    } else {
        tree<T1> temp1;
        if (arg2 > x) {
            temp1 = deltree(right, arg2);
        } else {
            auto temp2 = tree<T1>::Node(
                left, smallest(right), deltree(right, smallest(right))
            );
            temp1 = temp2;
        }
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
tree<T1> changetree(const T1 &arg1, const tree<T1> &arg2, const T1 &arg3) {
    // changetree b Tip n = Tip
    if (arg2.is_Tip()) {
        return tree<T1>::Tip();
    }

    // changetree b (Node left x right) n = (if b=x then (Node left n right) ...
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    tree<T1> temp0;
    if (arg1 == x) {
        auto temp1 = tree<T1>::Node(
            left, arg3, right
        );
        temp0 = temp1;
    } else {
        tree<T1> temp2;
        if (arg1 < x) {
            temp2 = changetree(arg1, left, arg3);
        } else {
            temp2 = changetree(arg1, right, arg3);
        }
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
bool searchtreeF(const T1 &arg1, const tree<T1> &arg2) {
    // searchtreeF b Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtreeF b (Node left x right) = (if b=x then True ...
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    bool temp0;
    if (arg1 == x) {
        temp0 = true;
    } else {
        bool temp1;
        if (searchtreeF(arg1, left) == true) {
            temp1 = true;
        } else {
            bool temp2;
            if (searchtreeF(arg1, right) == true) {
                temp2 = true;
            } else {
                temp2 = false;
            }
            temp1 = temp2;
        }
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
bool searchtreeM(const T1 &arg1, const tree<T1> &arg2) {
    // searchtreeM b Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtreeM b (Node left x right) = (if (searchtreeM b left) =True then True ...
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    bool temp0;
    if (searchtreeM(arg1, left) == true) {
        temp0 = true;
    } else {
        bool temp1;
        if (arg1 == x) {
            temp1 = true;
        } else {
            bool temp2;
            if (searchtreeM(arg1, right) == true) {
                temp2 = true;
            } else {
                temp2 = false;
            }
            temp1 = temp2;
        }
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
bool searchtreeL(const T1 &arg1, const tree<T1> &arg2) {
    // searchtreeL b Tip = False
    if (arg2.is_Tip()) {
        return false;
    }

    // searchtreeL b (Node left x right) = (if (searchtreeL b left)=True then True ...
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    bool temp0;
    if (searchtreeL(arg1, left) == true) {
        temp0 = true;
    } else {
        bool temp1;
        if (searchtreeL(arg1, right) == true) {
            temp1 = true;
        } else {
            bool temp2;
            if (arg1 == x) {
                temp2 = true;
            } else {
                temp2 = false;
            }
            temp1 = temp2;
        }
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
tree<T1> mergetree(const tree<T1> &arg1) {
    // mergetree Tip = Tip
    if (arg1.is_Tip()) {
        return tree<T1>::Tip();
    }

    // mergetree (Node left x right) = (Node (mergetree right) x (mergetree left))
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp0 = tree<T1>::Node(
        mergetree(right), x, mergetree(left)
    );
    return temp0;
}

template<typename T1>
std::deque<T1> looktreeF(const tree<T1> &arg1) {
    // looktreeF Tip = Nil
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // looktreeF (Node left x right) = (applist (Cons x (looktreeF left))  ...
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp0 = looktreeF(left);
    temp0.push_front(x);
    return applist(temp0, looktreeF(right));
}

template<typename T1>
std::deque<T1> looktreeM(const tree<T1> &arg1) {
    // looktreeM Tip = Nil
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // looktreeM (Node left x right) = (applist (addlistE x (looktreeM left)) ...
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    return applist(addlistE(x, looktreeM(left)), looktreeM(right));
}

template<typename T1>
std::deque<T1> looktreeL(const tree<T1> &arg1) {
    // looktreeL Tip = Nil
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // looktreeL (Node left x right) = (addlistE x  ...
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    return addlistE(x, applist(looktreeL(left), looktreeL(right)));
}

template<typename T1>
std::deque<T1> Merge(const std::deque<T1> &arg1, const std::deque<T1> &arg2) {
    // Merge [] xs = xs
    if (arg1.empty()) {
        return arg2;
    }

    // Merge xs [] = xs
    if (arg2.empty()) {
        return arg1;
    }

    // Merge (x#xs)(y#ys) = (if x<y then x#(Merge xs (y#ys)) ...
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto y = arg2.front();
    auto ys = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (x < y) {
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
std::deque<T1> MergeSort(const std::deque<T1> &arg1) {
    // MergeSort [] = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // MergeSort (Cons a []) = [a]
    if (!arg1.empty()) {
        if (std::deque<T1>(arg1.begin() + 1, arg1.end()).empty()) {
            auto a = arg1.front();
            return std::deque<T1>{a};
        }
    }

    // MergeSort xs = Merge(MergeSort(take((size xs)div 2)xs)) ...
    return Merge(MergeSort(std::deque<T1>(arg1.begin(), arg1.begin() + size(arg1) / 2)), MergeSort(std::deque<T1>(arg1.begin() + size(arg1) / 2, arg1.end())));
}

template<typename T1>
std::deque<T1> transtolist(const tree<T1> &arg1) {
    // transtolist Tip = Nil
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // transtolist (Node left x right) = x #(transtolist left) ...
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp0 = transtolist(left);
    auto temp1 = transtolist(right);
    temp0.insert(temp0.end(), temp1.begin(), temp1.end());
    auto temp2 = temp0;
    temp2.push_front(x);
    return temp2;
}

// generated by HOL2Cpp
