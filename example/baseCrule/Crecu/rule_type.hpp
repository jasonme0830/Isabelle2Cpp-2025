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
std::deque<T1> AddListHead(const T1 &arg1, const std::deque<T1> &arg2) {
    // AddListHead a xs =a # xs
    auto a = arg1;
    auto xs = arg2;
    auto temp0 = std::move(xs);
    temp0.push_front(std::move(a));
    return temp0;
}

template<typename T1>
std::deque<T1> AddListTail(T1 arg1, std::deque<T1> arg2) {
    // AddListTail a [] =Cons a []
    if (arg2.empty()) {
        auto a = std::move(arg1);
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // AddListTail a (x#xs) = x # (AddListTail a xs )
    auto a = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    auto temp0 = AddListTail(std::move(a), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::deque<T1> AddListI(std::uint64_t arg1, T1 arg2, std::deque<T1> arg3) {
    // AddListI i a [] = Cons a []
    if (arg3.empty()) {
        auto a = std::move(arg2);
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // AddListI 0 a (x#xs) = AddListHead a (x#xs)
    if (arg1 == 0) {
        if (arg3.size() >= 1) {
            auto a = std::move(arg2);
            auto x = arg3.front();
            arg3.erase(arg3.begin(), arg3.begin() + 1);
            auto xs = std::move(arg3);
            auto temp0 = std::move(a);
            auto temp2 = std::move(xs);
            temp2.push_front(std::move(x));
            auto temp1 = std::move(temp2);
            return AddListHead(std::move(temp0), std::move(temp1));
        }
    }

    // AddListI (Suc i) a (x#xs) = x # (AddListI i a xs)
    auto i = arg1 - 1;
    auto a = std::move(arg2);
    auto x = arg3.front();
    arg3.erase(arg3.begin(), arg3.begin() + 1);
    auto xs = std::move(arg3);
    auto temp0 = AddListI(std::move(i), std::move(a), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::deque<T1> DelListHead(const std::deque<T1> &arg1) {
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
    if (arg1.size() == 1) {
        return std::deque<T1>();
    }

    // DelListTail (x#xs) =  Cons x (DelListTail xs)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = DelListTail(std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::deque<T1> DelListI(std::uint64_t arg1, std::deque<T1> arg2) {
    // DelListI i [] = []
    if (arg2.empty()) {
        return std::deque<T1>();
    }

    // DelListI i (Cons a []) = []
    if (arg2.size() == 1) {
        return std::deque<T1>();
    }

    // DelListI 0 (x#xs) = DelListHead (x#xs)
    if (arg1 == 0) {
        if (arg2.size() >= 1) {
            auto x = arg2.front();
            arg2.erase(arg2.begin(), arg2.begin() + 1);
            auto xs = std::move(arg2);
            auto temp1 = std::move(xs);
            temp1.push_front(std::move(x));
            auto temp0 = std::move(temp1);
            return DelListHead(std::move(temp0));
        }
    }

    // DelListI (Suc i) (x#xs) = x # (DelListI i xs)
    auto i = arg1 - 1;
    auto x = arg2.front();
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    auto temp0 = DelListI(std::move(i), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
bool SearchList(T1 arg1, std::deque<T1> arg2) {
    // SearchList a [] = False
    if (arg2.empty()) {
        return false;
    }

    // SearchList a (x#xs) = (if a=x then True else (SearchList a xs))
    auto a = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    bool temp0;
    if (a == x) {
        temp0 = true;
    } else {
        temp0 = SearchList(std::move(a), std::move(xs));
    }
    return temp0;
}

template<typename T1>
std::deque<T1> Modify1(T1 arg1, T1 arg2, std::deque<T1> arg3) {
    // Modify1 a b [] = []
    if (arg3.empty()) {
        return std::deque<T1>();
    }

    // Modify1 a b (x#xs) = (if x=a then b#(Modify1 a b xs) else x#(Modify1 a b xs))
    auto a = std::move(arg1);
    auto b = std::move(arg2);
    auto x = arg3.front();
    arg3.erase(arg3.begin(), arg3.begin() + 1);
    auto xs = std::move(arg3);
    std::deque<T1> temp0;
    if (x == a) {
        auto temp1 = Modify1(std::move(a), std::move(b), std::move(xs));
        temp1.push_front(b);
        temp0 = std::move(temp1);
    } else {
        auto temp2 = Modify1(std::move(a), std::move(b), std::move(xs));
        temp2.push_front(std::move(x));
        temp0 = std::move(temp2);
    }
    return temp0;
}

template<typename T1>
std::deque<T1> Modify2(std::uint64_t arg1, T1 arg2, std::deque<T1> arg3) {
    // Modify2 n b [] = []
    if (arg3.empty()) {
        return std::deque<T1>();
    }

    // Modify2 0 b (x#xs) =(b#xs)
    if (arg1 == 0) {
        if (arg3.size() >= 1) {
            auto b = std::move(arg2);
            arg3.erase(arg3.begin(), arg3.begin() + 1);
            auto xs = std::move(arg3);
            auto temp0 = std::move(xs);
            temp0.push_front(std::move(b));
            return temp0;
        }
    }

    // Modify2 (Suc n) b (x#xs) = (x#(Modify2 n b xs))
    auto n = arg1 - 1;
    auto b = std::move(arg2);
    auto x = arg3.front();
    arg3.erase(arg3.begin(), arg3.begin() + 1);
    auto xs = std::move(arg3);
    auto temp0 = Modify2(std::move(n), std::move(b), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::deque<T1> ffilter(std::function<bool(T1 )> arg1, std::deque<T1> arg2) {
    // ffilter f [] = []
    if (arg2.empty()) {
        return std::deque<T1>();
    }

    // ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))
    auto f = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    std::deque<T1> temp0;
    if (f(x)) {
        auto temp1 = ffilter(std::move(f), std::move(xs));
        temp1.push_front(std::move(x));
        temp0 = std::move(temp1);
    } else {
        temp0 = ffilter(std::move(f), std::move(xs));
    }
    return temp0;
}

template<typename T1>
std::deque<T1> app(std::deque<T1> arg1, std::deque<T1> arg2) {
    // app Nil as =as
    if (arg1.empty()) {
        auto as = std::move(arg2);
        return as;
    }

    // app(Cons a as )bs= Cons a (app as bs)
    auto a = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto as = std::move(arg1);
    auto bs = std::move(arg2);
    auto temp0 = app(std::move(as), std::move(bs));
    temp0.push_front(std::move(a));
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
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto as = std::move(arg1);
    auto temp0 = Reverse(std::move(as));
    auto temp2 = std::deque<T1>();
    temp2.push_front(std::move(a));
    auto temp1 = std::move(temp2);
    return app(std::move(temp0), std::move(temp1));
}

template<typename T1>
std::deque<T1> Reverse2(std::deque<T1> arg1) {
    // Reverse2 [] = []
    if (arg1.empty()) {
        return std::deque<T1>();
    }

    // Reverse2 (Cons a []) =Cons a []
    if (arg1.size() == 1) {
        auto a = arg1.front();
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // Reverse2 (x#xs) = ( Reverse2 xs)@ (Cons x Nil)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto temp0 = std::deque<T1>();
    temp0.push_front(std::move(x));
    auto temp1 = Reverse2(std::move(xs));
    auto temp2 = std::move(temp0);
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
}

template<typename T1>
std::deque<T1> Insert(T1 arg1, std::deque<T1> arg2) {
    // Insert a [] =Cons a []
    if (arg2.empty()) {
        auto a = std::move(arg1);
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs))
    auto a = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    std::deque<T1> temp0;
    if (a <= x) {
        auto temp1 = std::move(xs);
        temp1.push_front(std::move(x));
        auto temp2 = std::move(temp1);
        temp2.push_front(std::move(a));
        temp0 = std::move(temp2);
    } else {
        auto temp3 = Insert(std::move(a), std::move(xs));
        temp3.push_front(std::move(x));
        temp0 = std::move(temp3);
    }
    return temp0;
}

template<typename T1>
std::deque<T1> InsertSortPart(std::deque<T1> arg1, std::deque<T1> arg2) {
    // InsertSortPart [] ys=ys
    if (arg1.empty()) {
        auto ys = std::move(arg2);
        return ys;
    }

    // InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto ys = std::move(arg2);
    auto temp0 = std::move(x);
    auto temp1 = std::move(ys);
    return InsertSortPart(std::move(xs), Insert(std::move(temp0), std::move(temp1)));
}

template<typename T1>
std::deque<T1> InsertSort(const std::deque<T1> &arg1) {
    // InsertSort xs = InsertSortPart xs Nil
    auto xs = arg1;
    auto temp0 = std::move(xs);
    auto temp1 = std::deque<T1>();
    return InsertSortPart(std::move(temp0), std::move(temp1));
}

template<typename T1>
std::uint64_t ssize(std::deque<T1> arg1) {
    // ssize Nil = 0
    if (arg1.empty()) {
        return 0;
    }

    // ssize (Cons x Nil) = (Suc 0)
    if (arg1.size() == 1) {
        return 0 + 1;
    }

    // ssize (Cons x xs) = Suc (ssize xs)
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    return ssize(std::move(xs)) + 1;
}

template<typename T1>
std::uint64_t slength(std::deque<T1> arg1) {
    // slength Nil = 0
    if (arg1.empty()) {
        return 0;
    }

    // slength (Cons x Nil) = (Suc 0)
    if (arg1.size() == 1) {
        return 0 + 1;
    }

    // slength (Cons x xs) = (Suc (slength xs))
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    return slength(std::move(xs)) + 1;
}

template<typename T1>
std::deque<T1> stake(std::uint64_t arg1, std::deque<T1> arg2) {
    // stake 0 xs = Nil
    if (arg1 == 0) {
        return std::deque<T1>();
    }

    // stake (Suc n) Nil = Nil
    if (arg1 != 0) {
        if (arg2.empty()) {
            return std::deque<T1>();
        }
    }

    // stake (Suc n) (Cons x xs) =(Cons x (stake n xs))
    auto n = arg1 - 1;
    auto x = arg2.front();
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    auto temp0 = stake(std::move(n), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::deque<T1> sdrop(std::uint64_t arg1, std::deque<T1> arg2) {
    // sdrop 0 xs = xs
    if (arg1 == 0) {
        auto xs = std::move(arg2);
        return xs;
    }

    // sdrop (Suc n) Nil = Nil
    if (arg1 != 0) {
        if (arg2.empty()) {
            return std::deque<T1>();
        }
    }

    // sdrop (Suc n) (Cons x xs) = (sdrop n xs)
    auto n = arg1 - 1;
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    return sdrop(std::move(n), std::move(xs));
}

template<typename T1>
std::deque<T1> Merge(std::deque<T1> arg1, std::deque<T1> arg2) {
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
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto y = arg2.front();
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto ys = std::move(arg2);
    std::deque<T1> temp0;
    if (x <= y) {
        auto temp1 = std::move(ys);
        temp1.push_front(std::move(y));
        auto temp2 = Merge(std::move(xs), std::move(temp1));
        temp2.push_front(std::move(x));
        temp0 = std::move(temp2);
    } else {
        auto temp3 = std::move(xs);
        temp3.push_front(std::move(x));
        auto temp4 = Merge(std::move(temp3), std::move(ys));
        temp4.push_front(std::move(y));
        temp0 = std::move(temp4);
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
        auto xs = arg1;
        auto temp1 = xs;
        auto temp0 = MergeSort(std::deque<T1>(xs.begin(), xs.begin() + size(std::move(temp1)) / 2));
        auto temp3 = xs;
        auto temp4 = size(std::move(temp3)) / 2;
        auto temp5 = std::move(xs);
        temp5.erase(temp5.begin(), std::next(temp5.begin(), temp4));
        auto temp2 = MergeSort(std::move(temp5));
        return Merge(std::move(temp0), std::move(temp2));
    };

    static std::map<std::tuple<std::deque<T1>>, std::deque<T1>> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::optional<std::uint64_t> bs(std::uint64_t arg1, std::deque<std::uint64_t> arg2);

std::uint64_t fib(const std::uint64_t &arg1);

template<typename T1>
std::uint64_t slength(std::deque<T1> arg1) {
    // slength [] = 0
    if (arg1.empty()) {
        return 0;
    }

    // slength (x # xs) = 1 + slength xs
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    return 1 + slength(std::move(xs));
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
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    auto temp0 = stake(std::move(n), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::deque<T1> sdrop(std::uint64_t arg1, std::deque<T1> arg2) {
    // sdrop 0 xs = xs
    if (arg1 == 0) {
        auto xs = std::move(arg2);
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
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(arg2);
    return sdrop(std::move(n), std::move(xs));
}

std::deque<std::uint64_t> supto(std::uint64_t arg1, std::uint64_t arg2);

template<typename T1>
T1 snth(std::deque<T1> arg1, std::uint64_t arg2) {
    // snth [] n = 0
    if (arg1.empty()) {
        return 0;
    }

    // snth (x#xs) 0 = x
    if (arg1.size() >= 1) {
        if (arg2 == 0) {
            auto x = arg1.front();
            arg1.erase(arg1.begin(), arg1.begin() + 1);
            return x;
        }
    }

    // snth (x#xs) (Suc n) = snth xs n
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(arg1);
    auto n = arg2 - 1;
    return snth(std::move(xs), std::move(n));
}

template<typename T1>
tree<T1> copy_tree(const tree<T1> &arg1) {
    auto impl = [&]() -> tree<T1> {
        // copy_tree (Node left x right) = Node (copy_tree left) x (copy_tree right)
        if (arg1.is_Node()) {
            auto left = arg1.as_Node().p1();
            auto x = arg1.as_Node().p2();
            auto right = arg1.as_Node().p3();
            auto temp0 = tree<T1>::Node(
                copy_tree(std::move(left)),
                std::move(x),
                copy_tree(std::move(right))
            );
            return temp0;
        }

        // copy_tree Tip = Tip
        return tree<T1>::Tip();
    };

    static std::map<std::tuple<tree<T1>>, tree<T1>> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
bool searchtree1(const T1 &arg1, const tree<T1> &arg2) {
    auto impl = [&]() -> bool {
        // searchtree1 a Tip = False
        if (arg2.is_Tip()) {
            return false;
        }

        // searchtree1 a (Node left x right) = (a= x \<or> (searchtree1 a left)\<or> (searchtree1 a right))
        auto a = arg1;
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        return (a == x) || (searchtree1(a, std::move(left)) || searchtree1(std::move(a), std::move(right)));
    };

    static std::map<std::tuple<T1, tree<T1>>, bool> cache;
    auto args = std::make_tuple(arg1, arg2);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
bool searchtree2(const T1 &arg1, const tree<T1> &arg2) {
    auto impl = [&]() -> bool {
        // searchtree2 a Tip = False
        if (arg2.is_Tip()) {
            return false;
        }

        // searchtree2 a (Node left x right) = ( (searchtree2 a left) \<or> (a=x) \<or>  (searchtree2 a right))
        auto a = arg1;
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        return searchtree2(a, std::move(left)) || ((a == x) || searchtree2(std::move(a), std::move(right)));
    };

    static std::map<std::tuple<T1, tree<T1>>, bool> cache;
    auto args = std::make_tuple(arg1, arg2);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

template<typename T1>
bool searchtree3(const T1 &arg1, const tree<T1> &arg2) {
    auto impl = [&]() -> bool {
        // searchtree3 a Tip = False
        if (arg2.is_Tip()) {
            return false;
        }

        // searchtree3 a (Node left x right) = ( (searchtree3 a left)\<or> (searchtree3 a right) \<or> (a=x) )
        auto a = arg1;
        auto left = arg2.as_Node().p1();
        auto x = arg2.as_Node().p2();
        auto right = arg2.as_Node().p3();
        return searchtree3(a, std::move(left)) || (searchtree3(a, std::move(right)) || (a == x));
    };

    static std::map<std::tuple<T1, tree<T1>>, bool> cache;
    auto args = std::make_tuple(arg1, arg2);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
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
std::deque<T1> transtolist(const tree<T1> &arg1) {
    auto impl = [&]() -> std::deque<T1> {
        // transtolist Tip = []
        if (arg1.is_Tip()) {
            return std::deque<T1>();
        }

        // transtolist (Node left a right) =( a # (transtolist left)@(transtolist right))
        auto left = arg1.as_Node().p1();
        auto a = arg1.as_Node().p2();
        auto right = arg1.as_Node().p3();
        auto temp0 = transtolist(std::move(left));
        auto temp1 = transtolist(std::move(right));
        temp0.insert(temp0.end(), temp1.begin(), temp1.end());
        auto temp2 = std::move(temp0);
        temp2.push_front(std::move(a));
        return temp2;
    };

    static std::map<std::tuple<tree<T1>>, std::deque<T1>> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
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
tree<T1> deltreeroot(const tree<T1> &arg1) {
    // deltreeroot Tip = Tip
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
    auto temp1 = tree<T1>::Node(
        std::move(left),
        std::move(x),
        std::move(right)
    );
    auto temp0 = transtolist(std::move(temp1));
    return MergeSort(std::move(temp0));
}

// generated by HOL2Cpp
