#include <cstdint>
#include <cstdlib>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <optional>
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
std::deque<T1> AddListHead(T1 arg1, std::deque<T1> arg2) {
    // AddListHead a xs =a # xs
    auto a = arg1;
    auto xs = arg2;
    auto temp0 = xs;
    temp0.push_front(a);
    return temp0;
}

template<typename T1>
std::deque<T1> AddListTail(T1 arg1, std::deque<T1> arg2) {
    // AddListTail a [] =Cons a []
    if (arg2.empty()) {
        auto a = arg1;
        auto temp0 = std::deque<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // AddListTail a (x#xs) = x # (AddListTail a xs )
    auto a = arg1;
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    auto temp0 = AddListTail(a, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> AddListI(std::uint64_t arg1, T1 arg2, std::deque<T1> arg3) {
    // AddListI i a [] = Cons a []
    if (arg3.empty()) {
        auto a = arg2;
        auto temp0 = std::deque<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // AddListI 0 a (x#xs) = AddListHead a (x#xs)
    if (arg1 == 0) {
        if (!arg3.empty()) {
            auto a = arg2;
            auto x = arg3.front();
            auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
            auto temp0 = xs;
            temp0.push_front(x);
            return AddListHead(a, temp0);
        }
    }

    // AddListI (Suc i) a (x#xs) = x # (AddListI i a xs)
    auto i = arg1 - 1;
    auto a = arg2;
    auto x = arg3.front();
    auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
    auto temp0 = AddListI(i, a, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> DelListHead(std::deque<T1> arg1) {
    // DelListHead (x#xs) = xs
    if (!arg1.empty()) {
        auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
        return xs;
    }

    // DelListHead [] = []
    return std::deque<T1>();
}

template<typename T1>
std::deque<T1> DelListTail(std::deque<T1> arg1) {
    // DelListTail [] = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // DelListTail (Cons a []) = []
    if (!arg1.empty()) {
        if (std::deque<T1>(arg1.begin() + 1, arg1.end()).empty()) {
            return std::deque<T1>();
        }
    }

    // DelListTail (x#xs) =  Cons x (DelListTail xs)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = DelListTail(xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> DelListI(std::uint64_t arg1, std::deque<T1> arg2) {
    // DelListI i [] = []
    if (arg2.empty()) {
        return std::deque<T1>();
    }

    // DelListI i (Cons a []) = []
    if (!arg2.empty()) {
        if (std::deque<T1>(arg2.begin() + 1, arg2.end()).empty()) {
            return std::deque<T1>();
        }
    }

    // DelListI 0 (x#xs) = DelListHead (x#xs)
    if (arg1 == 0) {
        if (!arg2.empty()) {
            auto x = arg2.front();
            auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
            auto temp0 = xs;
            temp0.push_front(x);
            return DelListHead(temp0);
        }
    }

    // DelListI (Suc i) (x#xs) = x # (DelListI i xs)
    auto i = arg1 - 1;
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    auto temp0 = DelListI(i, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
bool SearchList(T1 arg1, std::deque<T1> arg2) {
    // SearchList a [] = False
    if (arg2.empty()) {
        return false;
    }

    // SearchList a (x#xs) = (if a=x then True else (SearchList a xs))
    auto a = arg1;
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    bool temp0;
    if (a == x) {
        temp0 = true;
    } else {
        temp0 = SearchList(a, xs);
    }
    return temp0;
}

template<typename T1>
std::deque<T1> ModifyValue(T1 arg1, T1 arg2, std::deque<T1> arg3) {
    // ModifyValue a b [] = []
    if (arg3.empty()) {
        return std::deque<T1>();
    }

    // ModifyValue a b (x#xs) = (if x=a then b#(ModifyValue a b xs) else x#(ModifyValue a b xs))
    auto a = arg1;
    auto b = arg2;
    auto x = arg3.front();
    auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
    std::deque<T1> temp0;
    if (x == a) {
        auto temp1 = ModifyValue(a, b, xs);
        temp1.push_front(b);
        temp0 = temp1;
    } else {
        auto temp2 = ModifyValue(a, b, xs);
        temp2.push_front(x);
        temp0 = temp2;
    }
    return temp0;
}

template<typename T1>
std::deque<T1> ModifyIndex(std::uint64_t arg1, T1 arg2, std::deque<T1> arg3) {
    // ModifyIndex n b [] = []
    if (arg3.empty()) {
        return std::deque<T1>();
    }

    // ModifyIndex 0 b (x#xs) =(b#xs)
    if (arg1 == 0) {
        if (!arg3.empty()) {
            auto b = arg2;
            auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
            auto temp0 = xs;
            temp0.push_front(b);
            return temp0;
        }
    }

    // ModifyIndex (Suc n) b (x#xs) = (x#(ModifyIndex n b xs))
    auto n = arg1 - 1;
    auto b = arg2;
    auto x = arg3.front();
    auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
    auto temp0 = ModifyIndex(n, b, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> ffilter(std::function<bool(T1 )> arg1, std::deque<T1> arg2) {
    // ffilter f [] = []
    if (arg2.empty()) {
        return std::deque<T1>();
    }

    // ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))
    auto f = arg1;
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (f(x)) {
        auto temp1 = ffilter(f, xs);
        temp1.push_front(x);
        temp0 = temp1;
    } else {
        temp0 = ffilter(f, xs);
    }
    return temp0;
}

template<typename T1>
std::deque<T1> app(std::deque<T1> arg1, std::deque<T1> arg2) {
    // app Nil as =as
    if (arg1.empty()) {
        auto as = arg2;
        return as;
    }

    // app(Cons a as )bs= Cons a (app as bs)
    auto a = arg1.front();
    auto as = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto bs = arg2;
    auto temp0 = app(as, bs);
    temp0.push_front(a);
    return temp0;
}

template<typename T1>
std::deque<T1> Reverse(std::deque<T1> arg1) {
    // Reverse Nil =Nil
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // Reverse (Cons a as) = app(Reverse as)(Cons a Nil)
    auto a = arg1.front();
    auto as = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = std::deque<T1>();
    temp0.push_front(a);
    return app(Reverse(as), temp0);
}

template<typename T1>
std::deque<T1> Reverse2(std::deque<T1> arg1) {
    // Reverse2 [] = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // Reverse2 (Cons a []) =Cons a []
    if (!arg1.empty()) {
        if (std::deque<T1>(arg1.begin() + 1, arg1.end()).empty()) {
            auto a = arg1.front();
            auto temp0 = std::deque<T1>();
            temp0.push_front(a);
            return temp0;
        }
    }

    // Reverse2 (x#xs) = ( Reverse2 xs)@ (Cons x Nil)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = std::deque<T1>();
    temp0.push_front(x);
    auto temp1 = Reverse2(xs);
    auto temp2 = temp0;
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
}

template<typename T1>
std::deque<T1> Insert(T1 arg1, std::deque<T1> arg2) {
    // Insert a [] =Cons a []
    if (arg2.empty()) {
        auto a = arg1;
        auto temp0 = std::deque<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs))
    auto a = arg1;
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (a <= x) {
        auto temp1 = xs;
        temp1.push_front(x);
        auto temp2 = temp1;
        temp2.push_front(a);
        temp0 = temp2;
    } else {
        auto temp3 = Insert(a, xs);
        temp3.push_front(x);
        temp0 = temp3;
    }
    return temp0;
}

template<typename T1>
std::deque<T1> InsertSortPart(std::deque<T1> arg1, std::deque<T1> arg2) {
    // InsertSortPart [] ys=ys
    if (arg1.empty()) {
        auto ys = arg2;
        return ys;
    }

    // InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto ys = arg2;
    return InsertSortPart(xs, Insert(x, ys));
}

template<typename T1>
std::deque<T1> InsertSort(std::deque<T1> arg1) {
    // InsertSort xs = InsertSortPart xs Nil
    auto xs = arg1;
    return InsertSortPart(xs, std::deque<T1>());
}

template<typename T1>
std::deque<T1> Merge(std::deque<T1> arg1, std::deque<T1> arg2) {
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
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto y = arg2.front();
    auto ys = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
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
std::deque<T1> MergeSort(std::deque<T1> arg1) {
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

    // MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs))
    auto xs = arg1;
    return Merge(MergeSort(std::deque<T1>(xs.begin(), xs.begin() + size(xs) / 2)), MergeSort(std::deque<T1>(xs.begin() + size(xs) / 2, xs.end())));
}

std::optional<std::uint64_t> bs(std::uint64_t arg1, std::deque<std::uint64_t> arg2);

std::uint64_t two_fib(std::uint64_t arg1);

std::uint64_t three_fib(std::uint64_t arg1);

std::uint64_t four_fib(std::uint64_t arg1);

std::uint64_t Jacobsthal(std::uint64_t arg1);

std::uint64_t Narayana(std::uint64_t arg1);

std::uint64_t Leonardo(std::uint64_t arg1);

std::uint64_t Padua(std::uint64_t arg1);

std::uint64_t Pell(std::uint64_t arg1);

std::uint64_t Lucas(std::uint64_t arg1);

template<typename T1>
std::uint64_t slength(std::deque<T1> arg1) {
    // slength [] = 0
    if (arg1.empty()) {
        return 0;
    }

    // slength (x # xs) = 1 + slength xs
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    return 1 + slength(xs);
}

template<typename T1>
std::deque<T1> stake(std::uint64_t arg1, std::deque<T1> arg2) {
    // stake 0 xs = []
    if (arg1 == 0) {
        return std::deque<T1>();
    }

    // stake (Suc n) [] = []
    if (arg1 != 0) {
        if (arg2.empty()) {
            return std::deque<T1>();
        }
    }

    // stake (Suc n) (x # xs) = x # stake n xs
    auto n = arg1 - 1;
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    auto temp0 = stake(n, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> sdrop(std::uint64_t arg1, std::deque<T1> arg2) {
    // sdrop 0 xs = xs
    if (arg1 == 0) {
        auto xs = arg2;
        return xs;
    }

    // sdrop (Suc n) [] = []
    if (arg1 != 0) {
        if (arg2.empty()) {
            return std::deque<T1>();
        }
    }

    // sdrop (Suc n) (x # xs) = sdrop n xs
    auto n = arg1 - 1;
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    return sdrop(n, xs);
}

std::deque<std::uint64_t> supto(std::uint64_t arg1, std::uint64_t arg2);

template<typename T1>
T1 snth(std::deque<T1> arg1, std::uint64_t arg2) {
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
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto n = arg2 - 1;
    return snth(xs, n);
}

template<typename T1>
tree<T1> copy_tree(tree<T1> arg1) {
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
std::deque<T1> transtolist(tree<T1> arg1) {
    // transtolist Tip = []
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // transtolist (Node left a right) =( a # (transtolist left)@(transtolist right))
    auto left = arg1.as_Node().p1();
    auto a = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp0 = transtolist(left);
    auto temp1 = transtolist(right);
    temp0.insert(temp0.end(), temp1.begin(), temp1.end());
    auto temp2 = temp0;
    temp2.push_front(a);
    return temp2;
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
tree<T1> addtreeroot(T1 arg1, tree<T1> arg2) {
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

template<typename T1>
std::deque<T1> sorttree(tree<T1> arg1) {
    // sorttree Tip = []
    if (arg1.is_Tip()) {
        return std::deque<T1>();
    }

    // sorttree (Node Tip a Tip) =[a]
    if (arg1.is_Node()) {
        if (arg1.as_Node().p1().is_Tip()) {
            if (arg1.as_Node().p3().is_Tip()) {
                auto a = arg1.as_Node().p2();
                return std::deque<T1>{a};
            }
        }
    }

    // sorttree (Node left x right) =(MergeSort (transtolist (Node left x right)))
    auto left = arg1.as_Node().p1();
    auto x = arg1.as_Node().p2();
    auto right = arg1.as_Node().p3();
    auto temp0 = tree<T1>::Node(
        left,
        x,
        right
    );
    return MergeSort(transtolist(temp0));
}

// generated by HOL2Cpp
