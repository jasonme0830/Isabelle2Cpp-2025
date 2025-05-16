#include <cstdint>
#include <cstdlib>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
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
        if(std::holds_alternative<_Tip>(other.value_)){ 
            return tree<T1>(_Tip());
        }
        if(std::holds_alternative<_Node>(other.value_)){ 
            const tree<T1>& value = std::get<_Node>(value_);
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
std::list<T1> AddListHead(const T1 &arg1, const std::list<T1> &arg2) {
    // AddListHead a xs =a # xs
    auto a = std::move(arg1);
    auto xs = std::move(arg2);
    auto temp0 = std::move(xs);
    temp0.push_front(a);
    return temp0;
}

template<typename T1>
std::list<T1> AddListTail(const T1 &arg1, const std::list<T1> &arg2) {
    // AddListTail a [] =Cons a []
    if (arg2.empty()) {
        auto a = std::move(arg1);
        auto temp0 = std::list<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // AddListTail a (x#xs) = x # (AddListTail a xs )
    auto a = std::move(arg1);
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = AddListTail(a, std::move(xs));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> AddListI(const std::uint64_t &arg1, const T1 &arg2, const std::list<T1> &arg3) {
    // AddListI i a [] = Cons a []
    if (arg3.empty()) {
        auto a = std::move(arg2);
        auto temp0 = std::list<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // AddListI 0 a (x#xs) = AddListHead a (x#xs)
    if (arg1 == 0) {
        if (!arg3.empty()) {
            auto a = std::move(arg2);
            auto x = arg3.front();
            auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
            auto temp0 = a;
            auto temp2 = std::move(xs);
            temp2.push_front(x);
            auto temp1 = std::move(temp2);
            return AddListHead(temp0, temp1);
        }
    }

    // AddListI (Suc i) a (x#xs) = x # (AddListI i a xs)
    auto i = arg1 - 1;
    auto a = std::move(arg2);
    auto x = arg3.front();
    auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
    auto temp0 = AddListI(i, a, std::move(xs));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> DelListHead(const std::list<T1> &arg1) {
    // DelListHead (x#xs) = xs
    if (!arg1.empty()) {
        auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
        return xs;
    }

    // DelListHead [] = []
    return std::list<T1>();
}

template<typename T1>
std::list<T1> DelListTail(const std::list<T1> &arg1) {
    // DelListTail [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // DelListTail (Cons a []) = []
    if (!arg1.empty()) {
        if (std::list<T1>(std::next(arg1.begin()), arg1.end()).empty()) {
            return std::list<T1>();
        }
    }

    // DelListTail (x#xs) =  Cons x (DelListTail xs)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = DelListTail(std::move(xs));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> DelListI(const std::uint64_t &arg1, const std::list<T1> &arg2) {
    // DelListI i [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // DelListI i (Cons a []) = []
    if (!arg2.empty()) {
        if (std::list<T1>(std::next(arg2.begin()), arg2.end()).empty()) {
            return std::list<T1>();
        }
    }

    // DelListI 0 (x#xs) = DelListHead (x#xs)
    if (arg1 == 0) {
        if (!arg2.empty()) {
            auto x = arg2.front();
            auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
            auto temp1 = std::move(xs);
            temp1.push_front(x);
            auto temp0 = std::move(temp1);
            return DelListHead(temp0);
        }
    }

    // DelListI (Suc i) (x#xs) = x # (DelListI i xs)
    auto i = arg1 - 1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = DelListI(i, std::move(xs));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
bool SearchList(const T1 &arg1, const std::list<T1> &arg2) {
    // SearchList a [] = False
    if (arg2.empty()) {
        return false;
    }

    // SearchList a (x#xs) = (if a=x then True else (SearchList a xs))
    auto a = std::move(arg1);
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    bool temp0;
    if (a == x) {
        temp0 = true;
    } else {
        temp0 = SearchList(a, std::move(xs));
    }
    return temp0;
}

template<typename T1>
std::list<T1> ModifyValue(const T1 &arg1, const T1 &arg2, const std::list<T1> &arg3) {
    // ModifyValue a b [] = []
    if (arg3.empty()) {
        return std::list<T1>();
    }

    // ModifyValue a b (x#xs) = (if x=a then b#(ModifyValue a b xs) else x#(ModifyValue a b xs))
    auto a = std::move(arg1);
    auto b = std::move(arg2);
    auto x = arg3.front();
    auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
    std::list<T1> temp0;
    if (x == a) {
        auto temp1 = ModifyValue(a, b, std::move(xs));
        temp1.push_front(b);
        temp0 = std::move(temp1);
    } else {
        auto temp2 = ModifyValue(a, b, std::move(xs));
        temp2.push_front(x);
        temp0 = std::move(temp2);
    }
    return temp0;
}

template<typename T1>
std::list<T1> ModifyIndex(const std::uint64_t &arg1, const T1 &arg2, const std::list<T1> &arg3) {
    // ModifyIndex n b [] = []
    if (arg3.empty()) {
        return std::list<T1>();
    }

    // ModifyIndex 0 b (x#xs) =(b#xs)
    if (arg1 == 0) {
        if (!arg3.empty()) {
            auto b = std::move(arg2);
            auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
            auto temp0 = std::move(xs);
            temp0.push_front(b);
            return temp0;
        }
    }

    // ModifyIndex (Suc n) b (x#xs) = (x#(ModifyIndex n b xs))
    auto n = arg1 - 1;
    auto b = std::move(arg2);
    auto x = arg3.front();
    auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
    auto temp0 = ModifyIndex(n, b, std::move(xs));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> ffilter(const std::function<bool(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // ffilter f [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))
    auto f = std::move(arg1);
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
std::list<T1> app(const std::list<T1> &arg1, const std::list<T1> &arg2) {
    // app Nil as =as
    if (arg1.empty()) {
        auto as = std::move(arg2);
        return as;
    }

    // app(Cons a as )bs= Cons a (app as bs)
    auto a = arg1.front();
    auto as = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto bs = std::move(arg2);
    auto temp0 = app(std::move(as), std::move(bs));
    temp0.push_front(a);
    return temp0;
}

template<typename T1>
std::list<T1> Reverse(const std::list<T1> &arg1) {
    // Reverse Nil =Nil
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // Reverse (Cons a as) = app(Reverse as)(Cons a Nil)
    auto a = arg1.front();
    auto as = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = Reverse(std::move(as));
    auto temp2 = std::list<T1>();
    temp2.push_front(a);
    auto temp1 = std::move(temp2);
    return app(temp0, temp1);
}

template<typename T1>
std::list<T1> Reverse2(const std::list<T1> &arg1) {
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
std::list<T1> Insert(const T1 &arg1, const std::list<T1> &arg2) {
    // Insert a [] =Cons a []
    if (arg2.empty()) {
        auto a = std::move(arg1);
        auto temp0 = std::list<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs))
    auto a = std::move(arg1);
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (a <= x) {
        auto temp1 = std::move(xs);
        temp1.push_front(x);
        auto temp2 = std::move(temp1);
        temp2.push_front(a);
        temp0 = std::move(temp2);
    } else {
        auto temp3 = Insert(a, std::move(xs));
        temp3.push_front(x);
        temp0 = std::move(temp3);
    }
    return temp0;
}

template<typename T1>
std::list<T1> InsertSortPart(const std::list<T1> &arg1, const std::list<T1> &arg2) {
    // InsertSortPart [] ys=ys
    if (arg1.empty()) {
        auto ys = std::move(arg2);
        return ys;
    }

    // InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto ys = std::move(arg2);
    auto temp0 = x;
    auto temp1 = std::move(ys);
    return InsertSortPart(std::move(xs), Insert(temp0, temp1));
}

template<typename T1>
std::list<T1> InsertSort(const std::list<T1> &arg1) {
    // InsertSort xs = InsertSortPart xs Nil
    auto xs = std::move(arg1);
    auto temp0 = std::move(xs);
    auto temp1 = std::list<T1>();
    return InsertSortPart(temp0, temp1);
}

template<typename T1>
std::list<T1> Merge(const std::list<T1> &arg1, const std::list<T1> &arg2) {
    // Merge [] xs=xs
    if (arg1.empty()) {
        auto xs = std::move(arg2);
        return xs;
    }

    // Merge xs [] = xs
    if (arg2.empty()) {
        auto xs = std::move(arg1);
        return xs;
    }

    // Merge (x#xs)(y#ys) = (if x\<le>y then (x#(Merge xs (y#ys)) )else y # (Merge (x#xs)ys))
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto y = arg2.front();
    auto ys = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (x <= y) {
        auto temp1 = std::move(ys);
        temp1.push_front(y);
        auto temp2 = Merge(std::move(xs), std::move(temp1));
        temp2.push_front(x);
        temp0 = std::move(temp2);
    } else {
        auto temp3 = std::move(xs);
        temp3.push_front(x);
        auto temp4 = Merge(std::move(temp3), std::move(ys));
        temp4.push_front(y);
        temp0 = std::move(temp4);
    }
    return temp0;
}

template<typename T1>
std::list<T1> MergeSort(const std::list<T1> &arg1) {
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
    auto xs = std::move(arg1);
    auto temp1 = xs;
    auto temp0 = MergeSort(std::list<T1>(xs.begin(), std::next(xs.begin(), size(temp1) / 2)));
    auto temp3 = xs;
    auto temp4 = size(temp3) / 2;
    auto temp5 = std::move(xs);
    temp5.erase(temp5.begin(), std::next(temp5.begin(), temp4));
    auto temp2 = MergeSort(std::move(temp5));
    return Merge(temp0, temp2);
}

std::optional<std::uint64_t> bs(const std::uint64_t &arg1, const std::list<std::uint64_t> &arg2);

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
std::uint64_t slength(const std::list<T1> &arg1) {
    // slength [] = 0
    if (arg1.empty()) {
        return 0;
    }

    // slength (x # xs) = 1 + slength xs
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    return 1 + slength(std::move(xs));
}

template<typename T1>
std::list<T1> stake(const std::uint64_t &arg1, const std::list<T1> &arg2) {
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
std::list<T1> sdrop(const std::uint64_t &arg1, const std::list<T1> &arg2) {
    // sdrop 0 xs = xs
    if (arg1 == 0) {
        auto xs = std::move(arg2);
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
T1 snth(const std::list<T1> &arg1, const std::uint64_t &arg2) {
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
    auto a = std::move(arg1);
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
    auto a = std::move(arg1);
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
    auto a = std::move(arg1);
    auto left = arg2.as_Node().p1();
    auto x = arg2.as_Node().p2();
    auto right = arg2.as_Node().p3();
    return searchtree3(a, left) || (searchtree3(a, right) || (a == x));
}

template<typename T1>
tree<T1> inserttree(const T1 &arg1, const tree<T1> &arg2) {
    // inserttree a Tip = Node Tip a Tip
    if (arg2.is_Tip()) {
        auto a = std::move(arg1);
        auto temp0 = tree<T1>::Node(
            tree<T1>::Tip(),
            a,
            tree<T1>::Tip()
        );
        return temp0;
    }

    // inserttree a (Node left x right) = (if a\<le>x then (Node (inserttree a left) x right)  ...
    auto a = std::move(arg1);
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
        temp0 = temp1.self();
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
    T1 temp0;
    if (right.is_Tip()) {
        temp0 = x;
    } else {
        temp0 = rightest(right);
    }
    return temp0;
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
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = left.self();
    } else {
        temp0 = rightestleft(right);
    }
    return temp0;
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
    tree<T1> temp0;
    if (right.is_Tip()) {
        temp0 = left.self();
    } else {
        tree<T1> temp1;
        if (left.is_Tip()) {
            temp1 = right.self();
        } else {
            auto temp2 = left;
            auto temp3 = left;
            auto temp4 = tree<T1>::Node(
                delRightest(temp2),
                rightest(temp3),
                right
            );
            temp1 = temp4;
        }
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
tree<T1> addtreeroot(const T1 &arg1, const tree<T1> &arg2) {
    // addtreeroot x Tip = Node Tip x Tip
    if (arg2.is_Tip()) {
        auto x = std::move(arg1);
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
            auto x = std::move(arg1);
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
            auto x = std::move(arg1);
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
    auto x = std::move(arg1);
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
    auto a = std::move(arg1);
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
            temp2 = temp3.self();
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
tree<T1> changetree(const T1 &arg1, const T1 &arg2, const tree<T1> &arg3) {
    // changetree a b Tip = Tip
    if (arg3.is_Tip()) {
        return tree<T1>::Tip();
    }

    // changetree a b (Node left x right) = ( if (a=x) then (Node left b right) ...
    auto a = std::move(arg1);
    auto b = std::move(arg2);
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
        temp0 = temp1.self();
    } else {
        tree<T1> temp2;
        if (a < x) {
            auto temp3 = tree<T1>::Node(
                changetree(a, b, left),
                x,
                right
            );
            temp2 = temp3.self();
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
