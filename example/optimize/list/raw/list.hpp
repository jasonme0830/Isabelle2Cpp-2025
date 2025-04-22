#include <cstdint>
#include <cstdlib>
#include <functional>
#include <list>
#include <optional>
#include <utility>



template<typename T1>
std::list<T1> AddListHead(T1 arg1, std::list<T1> arg2) {
    // AddListHead a xs =a # xs
    auto a = arg1;
    auto xs = arg2;
    auto temp0 = xs;
    temp0.push_front(a);
    return temp0;
}

template<typename T1>
std::list<T1> AddListTail(T1 arg1, std::list<T1> arg2) {
    // AddListTail a [] =Cons a []
    if (arg2.empty()) {
        auto a = arg1;
        auto temp0 = std::list<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // AddListTail a (x#xs) = x # (AddListTail a xs )
    auto a = arg1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = AddListTail(a, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> AddListI(std::uint64_t arg1, T1 arg2, std::list<T1> arg3) {
    // AddListI i a [] = Cons a []
    if (arg3.empty()) {
        auto a = arg2;
        auto temp0 = std::list<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // AddListI 0 a (x#xs) = AddListHead a (x#xs)
    if (arg1 == 0) {
        if (!arg3.empty()) {
            auto a = arg2;
            auto x = arg3.front();
            auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
            auto temp0 = xs;
            temp0.push_front(x);
            return AddListHead(a, temp0);
        }
    }

    // AddListI (Suc i) a (x#xs) = x # (AddListI i a xs)
    auto i = arg1 - 1;
    auto a = arg2;
    auto x = arg3.front();
    auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
    auto temp0 = AddListI(i, a, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> DelListHead(std::list<T1> arg1) {
    // DelListHead (x#xs) = xs
    if (!arg1.empty()) {
        auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
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
    if (!arg1.empty()) {
        if (std::list<T1>(std::next(arg1.begin()), arg1.end()).empty()) {
            return std::list<T1>();
        }
    }

    // DelListTail (x#xs) =  Cons x (DelListTail xs)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = DelListTail(xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> DelListI(std::uint64_t arg1, std::list<T1> arg2) {
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
            auto temp0 = xs;
            temp0.push_front(x);
            return DelListHead(temp0);
        }
    }

    // DelListI (Suc i) (x#xs) = x # (DelListI i xs)
    auto i = arg1 - 1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = DelListI(i, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
bool SearchList(T1 arg1, std::list<T1> arg2) {
    // SearchList a [] = False
    if (arg2.empty()) {
        return false;
    }

    // SearchList a (x#xs) = (if a=x then True else (SearchList a xs))
    auto a = arg1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    bool temp0;
    if (a == x) {
        temp0 = true;
    } else {
        temp0 = SearchList(a, xs);
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
    auto a = arg1;
    auto b = arg2;
    auto x = arg3.front();
    auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
    std::list<T1> temp0;
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
std::list<T1> ModifyIndex(std::uint64_t arg1, T1 arg2, std::list<T1> arg3) {
    // ModifyIndex n b [] = []
    if (arg3.empty()) {
        return std::list<T1>();
    }

    // ModifyIndex 0 b (x#xs) =(b#xs)
    if (arg1 == 0) {
        if (!arg3.empty()) {
            auto b = arg2;
            auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
            auto temp0 = xs;
            temp0.push_front(b);
            return temp0;
        }
    }

    // ModifyIndex (Suc n) b (x#xs) = (x#(ModifyIndex n b xs))
    auto n = arg1 - 1;
    auto b = arg2;
    auto x = arg3.front();
    auto xs = std::list<T1>(std::next(arg3.begin()), arg3.end());
    auto temp0 = ModifyIndex(n, b, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> ffilter(std::function<bool(T1 )> arg1, std::list<T1> arg2) {
    // ffilter f [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // ffilter f(x#xs) = (if (f x) then (x#(ffilter f xs))else(ffilter f xs ))
    auto f = arg1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
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
std::list<T1> app(std::list<T1> arg1, std::list<T1> arg2) {
    // app Nil as =as
    if (arg1.empty()) {
        auto as = arg2;
        return as;
    }

    // app(Cons a as )bs= Cons a (app as bs)
    auto a = arg1.front();
    auto as = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto bs = arg2;
    auto temp0 = app(as, bs);
    temp0.push_front(a);
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
    auto as = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = std::list<T1>();
    temp0.push_front(a);
    return app(Reverse(as), temp0);
}

template<typename T1>
std::list<T1> Reverse2(std::list<T1> arg1) {
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
    auto temp1 = Reverse2(xs);
    auto temp2 = temp0;
    temp1.splice(temp1.end(), temp2);
    return temp1;
}

template<typename T1>
std::list<T1> Insert(T1 arg1, std::list<T1> arg2) {
    // Insert a [] =Cons a []
    if (arg2.empty()) {
        auto a = arg1;
        auto temp0 = std::list<T1>();
        temp0.push_front(a);
        return temp0;
    }

    // Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs))
    auto a = arg1;
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
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
std::list<T1> InsertSortPart(std::list<T1> arg1, std::list<T1> arg2) {
    // InsertSortPart [] ys=ys
    if (arg1.empty()) {
        auto ys = arg2;
        return ys;
    }

    // InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto ys = arg2;
    return InsertSortPart(xs, Insert(x, ys));
}

template<typename T1>
std::list<T1> InsertSort(std::list<T1> arg1) {
    // InsertSort xs = InsertSortPart xs Nil
    auto xs = arg1;
    return InsertSortPart(xs, std::list<T1>());
}

template<typename T1>
std::list<T1> Merge(std::list<T1> arg1, std::list<T1> arg2) {
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
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto y = arg2.front();
    auto ys = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
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
std::list<T1> MergeSort(std::list<T1> arg1) {
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
    auto xs = arg1;
    return Merge(MergeSort(std::list<T1>(xs.begin(), std::next(xs.begin(), size(xs) / 2))), MergeSort(std::list<T1>(std::next(xs.begin(), size(xs) / 2), xs.end())));
}

std::optional<std::uint64_t> bs(std::uint64_t arg1, std::list<std::uint64_t> arg2);

template<typename T1>
std::uint64_t slength(std::list<T1> arg1) {
    // slength [] = 0
    if (arg1.empty()) {
        return 0;
    }

    // slength (x # xs) = 1 + slength xs
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    return 1 + slength(xs);
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
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = stake(n, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::list<T1> sdrop(std::uint64_t arg1, std::list<T1> arg2) {
    // sdrop 0 xs = xs
    if (arg1 == 0) {
        auto xs = arg2;
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
    return sdrop(n, xs);
}

std::list<std::uint64_t> supto(std::uint64_t arg1, std::uint64_t arg2);

template<typename T1>
T1 snth(std::list<T1> arg1, std::uint64_t arg2) {
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
    return snth(xs, n);
}

// generated by HOL2Cpp
