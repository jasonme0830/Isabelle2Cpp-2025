#include <cstdint>
#include <cstdlib>
#include <functional>
#include <list>
#include <optional>
#include <utility>



template<typename T1>
std::list<T1> AddListHead(T1 arg1, std::list<T1> arg2) {
    // AddListHead a xs =a # xs
    auto a = std::move(arg1);
    auto xs = std::move(arg2);
    auto temp0 = std::move(xs);
    temp0.push_front(std::move(a));
    return temp0;
}

template<typename T1>
std::list<T1> AddListTail(T1 arg1, std::list<T1> arg2) {
    // AddListTail a [] =Cons a []
    if (arg2.empty()) {
        auto a = std::move(arg1);
        auto temp0 = std::list<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // AddListTail a (x#xs) = x # (AddListTail a xs )
    auto a = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    auto temp0 = AddListTail(std::move(a), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::list<T1> AddListI(std::uint64_t arg1, T1 arg2, std::list<T1> arg3) {
    // AddListI i a [] = Cons a []
    if (arg3.empty()) {
        auto a = std::move(arg2);
        auto temp0 = std::list<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // AddListI 0 a (x#xs) = AddListHead a (x#xs)
    if (arg1 == 0) {
        if (arg3.size() >= 1) {
            auto a = std::move(arg2);
            auto x = arg3.front();
            arg3.erase(arg3.begin(), std::next(arg3.begin(), 1));
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
    arg3.erase(arg3.begin(), std::next(arg3.begin(), 1));
    auto xs = std::move(arg3);
    auto temp0 = AddListI(std::move(i), std::move(a), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::list<T1> DelListHead(std::list<T1> arg1) {
    // DelListHead (x#xs) = xs
    if (arg1.size() >= 1) {
        arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
        auto xs = std::move(arg1);
        return xs;
    }

    // DelListHead [] = []
    return std::list<T1>();
}

template<typename T1>
std::list<T1> DelListTail(std::list<T1> arg1) {
    // DelListTail [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // DelListTail (Cons a []) = []
    if (arg1.size() == 1) {
        return std::list<T1>();
    }

    // DelListTail (x#xs) =  Cons x (DelListTail xs)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = DelListTail(std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::list<T1> DelListI(std::uint64_t arg1, std::list<T1> arg2) {
    // DelListI i [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // DelListI i (Cons a []) = []
    if (arg2.size() == 1) {
        return std::list<T1>();
    }

    // DelListI 0 (x#xs) = DelListHead (x#xs)
    if (arg1 == 0) {
        if (arg2.size() >= 1) {
            auto x = arg2.front();
            arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
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
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    auto temp0 = DelListI(std::move(i), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
bool SearchList(T1 arg1, std::list<T1> arg2) {
    // SearchList a [] = False
    if (arg2.empty()) {
        return false;
    }

    // SearchList a (x#xs) = (if a=x then True else (SearchList a xs))
    auto a = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
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
std::list<T1> ModifyValue(T1 arg1, T1 arg2, std::list<T1> arg3) {
    // ModifyValue a b [] = []
    if (arg3.empty()) {
        return std::list<T1>();
    }

    // ModifyValue a b (x#xs) = (if x=a then b#(ModifyValue a b xs) else x#(ModifyValue a b xs))
    auto a = std::move(arg1);
    auto b = std::move(arg2);
    auto x = arg3.front();
    arg3.erase(arg3.begin(), std::next(arg3.begin(), 1));
    auto xs = std::move(arg3);
    std::list<T1> temp0;
    if (x == a) {
        auto temp1 = ModifyValue(std::move(a), std::move(b), std::move(xs));
        temp1.push_front(b);
        temp0 = std::move(temp1);
    } else {
        auto temp2 = ModifyValue(std::move(a), std::move(b), std::move(xs));
        temp2.push_front(std::move(x));
        temp0 = std::move(temp2);
    }
    return temp0;
}

template<typename T1>
std::list<T1> ModifyIndex(std::uint64_t arg1, T1 arg2, std::list<T1> arg3) {
    // ModifyIndex n b [] = []
    if (arg3.empty()) {
        return std::list<T1>();
    }

    // ModifyIndex 0 b (x#xs) =(b#xs)
    if (arg1 == 0) {
        if (arg3.size() >= 1) {
            auto b = std::move(arg2);
            arg3.erase(arg3.begin(), std::next(arg3.begin(), 1));
            auto xs = std::move(arg3);
            auto temp0 = std::move(xs);
            temp0.push_front(std::move(b));
            return temp0;
        }
    }

    // ModifyIndex (Suc n) b (x#xs) = (x#(ModifyIndex n b xs))
    auto n = arg1 - 1;
    auto b = std::move(arg2);
    auto x = arg3.front();
    arg3.erase(arg3.begin(), std::next(arg3.begin(), 1));
    auto xs = std::move(arg3);
    auto temp0 = ModifyIndex(std::move(n), std::move(b), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::list<T1> ffilter(std::function<bool(T1 )> arg1, std::list<T1> arg2) {
    // ffilter f [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))
    auto f = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::list<T1> temp0;
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
std::list<T1> app(std::list<T1> arg1, std::list<T1> arg2) {
    // app Nil as =as
    if (arg1.empty()) {
        auto as = std::move(arg2);
        return as;
    }

    // app(Cons a as )bs= Cons a (app as bs)
    auto a = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto as = std::move(arg1);
    auto bs = std::move(arg2);
    auto temp0 = app(std::move(as), std::move(bs));
    temp0.push_front(std::move(a));
    return temp0;
}

template<typename T1>
std::list<T1> Reverse(std::list<T1> arg1) {
    // Reverse Nil =Nil
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // Reverse (Cons a as) = app(Reverse as)(Cons a Nil)
    auto a = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto as = std::move(arg1);
    auto temp0 = Reverse(std::move(as));
    auto temp2 = std::list<T1>();
    temp2.push_front(std::move(a));
    auto temp1 = std::move(temp2);
    return app(std::move(temp0), std::move(temp1));
}

template<typename T1>
std::list<T1> Reverse2(std::list<T1> arg1) {
    // Reverse2 [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // Reverse2 (Cons a []) =Cons a []
    if (arg1.size() == 1) {
        auto a = arg1.front();
        auto temp0 = std::list<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // Reverse2 (x#xs) = ( Reverse2 xs)@ (Cons x Nil)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = std::list<T1>();
    temp0.push_front(std::move(x));
    auto temp1 = Reverse2(std::move(xs));
    auto temp2 = std::move(temp0);
    temp1.splice(temp1.end(), temp2);
    return temp1;
}

template<typename T1>
std::list<T1> Insert(T1 arg1, std::list<T1> arg2) {
    // Insert a [] =Cons a []
    if (arg2.empty()) {
        auto a = std::move(arg1);
        auto temp0 = std::list<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs))
    auto a = std::move(arg1);
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::list<T1> temp0;
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
std::list<T1> InsertSortPart(std::list<T1> arg1, std::list<T1> arg2) {
    // InsertSortPart [] ys=ys
    if (arg1.empty()) {
        auto ys = std::move(arg2);
        return ys;
    }

    // InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto ys = std::move(arg2);
    auto temp0 = std::move(x);
    auto temp1 = std::move(ys);
    return InsertSortPart(std::move(xs), Insert(std::move(temp0), std::move(temp1)));
}

template<typename T1>
std::list<T1> InsertSort(std::list<T1> arg1) {
    // InsertSort xs = InsertSortPart xs Nil
    auto xs = std::move(arg1);
    auto temp0 = std::move(xs);
    auto temp1 = std::list<T1>();
    return InsertSortPart(std::move(temp0), std::move(temp1));
}

template<typename T1>
std::list<T1> Merge(std::list<T1> arg1, std::list<T1> arg2) {
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
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto y = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto ys = std::move(arg2);
    std::list<T1> temp0;
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
std::list<T1> MergeSort(std::list<T1> arg1) {
    // MergeSort [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // MergeSort (Cons a []) = [a]
    if (arg1.size() == 1) {
        auto a = arg1.front();
        return std::list<T1>{std::move(a)};
    }

    // MergeSort xs = Merge (MergeSort(take ((size xs) div 2) xs))  (MergeSort(drop ((size xs) div 2) xs))
    auto xs = std::move(arg1);
    auto temp1 = xs;
    auto temp0 = MergeSort(std::list<T1>(xs.begin(), std::next(xs.begin(), size(std::move(temp1)) / 2)));
    auto temp3 = xs;
    auto temp4 = size(std::move(temp3)) / 2;
    auto temp5 = std::move(xs);
    temp5.erase(temp5.begin(), std::next(temp5.begin(), temp4));
    auto temp2 = MergeSort(std::move(temp5));
    return Merge(std::move(temp0), std::move(temp2));
}

std::optional<std::uint64_t> bs(std::uint64_t arg1, std::list<std::uint64_t> arg2);

template<typename T1>
std::uint64_t slength(std::list<T1> arg1) {
    // slength [] = 0
    if (arg1.empty()) {
        return 0;
    }

    // slength (x # xs) = 1 + slength xs
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    return 1 + slength(std::move(xs));
}

template<typename T1>
std::list<T1> stake(std::uint64_t arg1, std::list<T1> arg2) {
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
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    auto temp0 = stake(std::move(n), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::list<T1> sdrop(std::uint64_t arg1, std::list<T1> arg2) {
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
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    return sdrop(std::move(n), std::move(xs));
}

std::list<std::uint64_t> supto(std::uint64_t arg1, std::uint64_t arg2);

template<typename T1>
T1 snth(std::list<T1> arg1, std::uint64_t arg2) {
    // snth [] n = 0
    if (arg1.empty()) {
        return 0;
    }

    // snth (x#xs) 0 = x
    if (arg1.size() >= 1) {
        if (arg2 == 0) {
            auto x = arg1.front();
            arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
            return x;
        }
    }

    // snth (x#xs) (Suc n) = snth xs n
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto n = arg2 - 1;
    return snth(std::move(xs), std::move(n));
}

// generated by HOL2Cpp
