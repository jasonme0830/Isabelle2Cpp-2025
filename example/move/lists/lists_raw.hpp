#include <cstdint>
#include <cstdlib>
#include <deque>
#include <functional>
#include <optional>



template<typename T1>
std::deque<T1> AddListHead(T1 arg1, std::deque<T1> arg2) {
    // AddListHead a xs =a # xs
    auto temp0 = arg2;
    temp0.push_front(arg1);
    return temp0;
}

template<typename T1>
std::deque<T1> AddListTail(T1 arg1, std::deque<T1> arg2) {
    // AddListTail a [] =Cons a []
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(arg1);
        return temp0;
    }

    // AddListTail a (x#xs) = x # (AddListTail a xs )
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    auto temp0 = AddListTail(arg1, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
std::deque<T1> AddListI(std::uint64_t arg1, T1 arg2, std::deque<T1> arg3) {
    // AddListI i a [] = Cons a []
    if (arg3.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(arg2);
        return temp0;
    }

    // AddListI 0 a (x#xs) = AddListHead a (x#xs)
    if (arg1 == 0) {
        if (!arg3.empty()) {
            auto x = arg3.front();
            auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
            auto temp0 = xs;
            temp0.push_front(x);
            return AddListHead(arg2, temp0);
        }
    }

    // AddListI (Suc i) a (x#xs) = x # (AddListI i a xs)
    auto i = arg1 - 1;
    auto x = arg3.front();
    auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
    auto temp0 = AddListI(i, arg2, xs);
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
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    bool temp0;
    if (arg1 == x) {
        temp0 = true;
    } else {
        temp0 = SearchList(arg1, xs);
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
    auto x = arg3.front();
    auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
    std::deque<T1> temp0;
    if (x == arg1) {
        auto temp1 = Modify1(arg1, arg2, xs);
        temp1.push_front(arg2);
        temp0 = temp1;
    } else {
        auto temp2 = Modify1(arg1, arg2, xs);
        temp2.push_front(x);
        temp0 = temp2;
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
        if (!arg3.empty()) {
            auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
            auto temp0 = xs;
            temp0.push_front(arg2);
            return temp0;
        }
    }

    // Modify2 (Suc n) b (x#xs) = (x#(Modify2 n b xs))
    auto n = arg1 - 1;
    auto x = arg3.front();
    auto xs = std::deque<T1>(arg3.begin() + 1, arg3.end());
    auto temp0 = Modify2(n, arg2, xs);
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
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (arg1(x)) {
        auto temp1 = ffilter(arg1, xs);
        temp1.push_front(x);
        temp0 = temp1;
    } else {
        temp0 = ffilter(arg1, xs);
    }
    return temp0;
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
std::deque<T1> app(std::deque<T1> arg1, std::deque<T1> arg2) {
    // app Nil as =as
    if (arg1.empty()) {
        return arg2;
    }

    // app(Cons a as )bs= Cons a (app as bs)
    auto a = arg1.front();
    auto as = std::deque<T1>(arg1.begin() + 1, arg1.end());
    auto temp0 = app(as, arg2);
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
std::deque<T1> Insert(T1 arg1, std::deque<T1> arg2) {
    // Insert a [] =Cons a []
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(arg1);
        return temp0;
    }

    // Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs))
    auto x = arg2.front();
    auto xs = std::deque<T1>(arg2.begin() + 1, arg2.end());
    std::deque<T1> temp0;
    if (arg1 <= x) {
        auto temp1 = xs;
        temp1.push_front(x);
        auto temp2 = temp1;
        temp2.push_front(arg1);
        temp0 = temp2;
    } else {
        auto temp3 = Insert(arg1, xs);
        temp3.push_front(x);
        temp0 = temp3;
    }
    return temp0;
}

template<typename T1>
std::deque<T1> InsertSortPart(std::deque<T1> arg1, std::deque<T1> arg2) {
    // InsertSortPart [] ys=ys
    if (arg1.empty()) {
        return arg2;
    }

    // InsertSortPart (x#xs) ys=InsertSortPart xs (Insert x ys)
    auto x = arg1.front();
    auto xs = std::deque<T1>(arg1.begin() + 1, arg1.end());
    return InsertSortPart(xs, Insert(x, arg2));
}

template<typename T1>
std::deque<T1> InsertSort(std::deque<T1> arg1) {
    // InsertSort xs = InsertSortPart xs Nil
    return InsertSortPart(arg1, std::deque<T1>());
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
    return Merge(MergeSort(std::deque<T1>(arg1.begin(), arg1.begin() + size(arg1) / 2)), MergeSort(std::deque<T1>(arg1.begin() + size(arg1) / 2, arg1.end())));
}

std::optional<std::uint64_t> bs(std::uint64_t arg1, std::deque<std::uint64_t> arg2);

std::uint64_t fib(std::uint64_t arg1);

// generated by HOL2Cpp
