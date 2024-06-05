#include <cstdint>
#include <cstdlib>
#include <deque>
#include <functional>
#include <map>
#include <optional>



template<typename T1>
std::deque<T1> AddListHead(const T1 &arg1, const std::deque<T1> &arg2) {
    // AddListHead a xs =a # xs
    auto temp0 = std::move(arg2);
    temp0.push_front(std::move(arg1));
    return temp0;
}

template<typename T1>
std::deque<T1> AddListTail(T1 arg1, std::deque<T1> arg2) {
    // AddListTail a [] =Cons a []
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(arg1));
        return temp0;
    }

    // AddListTail a (x#xs) = x # (AddListTail a xs )
    auto x = std::move(arg2.front());
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(std::move(arg2));
    auto temp0 = AddListTail(std::move(arg1), std::move(xs));
    temp0.push_front(std::move(x));
    return temp0;
}

template<typename T1>
std::deque<T1> AddListI(std::uint64_t arg1, T1 arg2, std::deque<T1> arg3) {
    // AddListI i a [] = Cons a []
    if (arg3.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(arg2));
        return temp0;
    }

    // AddListI 0 a (x#xs) = AddListHead a (x#xs)
    if (arg1 == 0) {
        if (arg3.size() >= 1) {
            auto x = std::move(arg3.front());
            arg3.erase(arg3.begin(), arg3.begin() + 1);
            auto xs = std::move(std::move(arg3));
            auto temp0 = std::move(arg2);
            auto temp2 = std::move(xs);
            temp2.push_front(std::move(x));
            auto temp1 = std::move(temp2);
            return AddListHead(std::move(temp0), std::move(temp1));
        }
    }

    // AddListI (Suc i) a (x#xs) = x # (AddListI i a xs)
    auto i = std::move(arg1 - 1);
    auto x = std::move(arg3.front());
    arg3.erase(arg3.begin(), arg3.begin() + 1);
    auto xs = std::move(std::move(arg3));
    auto temp0 = AddListI(std::move(i), std::move(arg2), std::move(xs));
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
    auto x = std::move(arg1.front());
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(std::move(arg1));
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
            auto x = std::move(arg2.front());
            arg2.erase(arg2.begin(), arg2.begin() + 1);
            auto xs = std::move(std::move(arg2));
            auto temp1 = std::move(xs);
            temp1.push_front(std::move(x));
            auto temp0 = std::move(temp1);
            return DelListHead(std::move(temp0));
        }
    }

    // DelListI (Suc i) (x#xs) = x # (DelListI i xs)
    auto i = std::move(arg1 - 1);
    auto x = std::move(arg2.front());
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(std::move(arg2));
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
    auto x = std::move(arg2.front());
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(std::move(arg2));
    bool temp0;
    if (arg1 == std::move(x)) {
        temp0 = true;
    } else {
        temp0 = SearchList(std::move(arg1), std::move(xs));
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
    auto x = std::move(arg3.front());
    arg3.erase(arg3.begin(), arg3.begin() + 1);
    auto xs = std::move(std::move(arg3));
    std::deque<T1> temp0;
    if (x == arg1) {
        auto temp1 = Modify1(std::move(arg1), std::move(arg2), std::move(xs));
        temp1.push_front(arg2);
        temp0 = std::move(temp1);
    } else {
        auto temp2 = Modify1(std::move(arg1), std::move(arg2), std::move(xs));
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
            arg3.erase(arg3.begin(), arg3.begin() + 1);
            auto xs = std::move(std::move(arg3));
            auto temp0 = std::move(xs);
            temp0.push_front(std::move(arg2));
            return temp0;
        }
    }

    // Modify2 (Suc n) b (x#xs) = (x#(Modify2 n b xs))
    auto n = std::move(arg1 - 1);
    auto x = std::move(arg3.front());
    arg3.erase(arg3.begin(), arg3.begin() + 1);
    auto xs = std::move(std::move(arg3));
    auto temp0 = Modify2(std::move(n), std::move(arg2), std::move(xs));
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
    auto x = std::move(arg2.front());
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(std::move(arg2));
    std::deque<T1> temp0;
    if (arg1(x)) {
        auto temp1 = ffilter(std::move(arg1), std::move(xs));
        temp1.push_front(std::move(x));
        temp0 = std::move(temp1);
    } else {
        temp0 = ffilter(std::move(arg1), std::move(xs));
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
    if (arg1.size() == 1) {
        auto a = std::move(arg1.front());
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(a));
        return temp0;
    }

    // Reverse2 (x#xs) = ( Reverse2 xs)@ (Cons x Nil)
    auto x = std::move(arg1.front());
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(std::move(arg1));
    auto temp0 = std::deque<T1>();
    temp0.push_front(std::move(x));
    auto temp1 = Reverse2(std::move(xs));
    auto temp2 = std::move(temp0);
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
    auto a = std::move(arg1.front());
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto as = std::move(std::move(arg1));
    auto temp0 = app(std::move(as), std::move(arg2));
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
    auto a = std::move(arg1.front());
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto as = std::move(std::move(arg1));
    auto temp0 = Reverse(std::move(as));
    auto temp2 = std::deque<T1>();
    temp2.push_front(std::move(a));
    auto temp1 = std::move(temp2);
    return app(std::move(temp0), std::move(temp1));
}

template<typename T1>
std::deque<T1> Insert(T1 arg1, std::deque<T1> arg2) {
    // Insert a [] =Cons a []
    if (arg2.empty()) {
        auto temp0 = std::deque<T1>();
        temp0.push_front(std::move(arg1));
        return temp0;
    }

    // Insert a (x#xs) =(if a \<le> x then a#x#xs else x#(Insert a xs))
    auto x = std::move(arg2.front());
    arg2.erase(arg2.begin(), arg2.begin() + 1);
    auto xs = std::move(std::move(arg2));
    std::deque<T1> temp0;
    if (arg1 <= x) {
        auto temp1 = std::move(xs);
        temp1.push_front(std::move(x));
        auto temp2 = std::move(temp1);
        temp2.push_front(std::move(arg1));
        temp0 = std::move(temp2);
    } else {
        auto temp3 = Insert(std::move(arg1), std::move(xs));
        temp3.push_front(std::move(x));
        temp0 = std::move(temp3);
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
    auto x = std::move(arg1.front());
    arg1.erase(arg1.begin(), arg1.begin() + 1);
    auto xs = std::move(std::move(arg1));
    auto temp0 = std::move(x);
    auto temp1 = std::move(arg2);
    return InsertSortPart(std::move(xs), Insert(std::move(temp0), std::move(temp1)));
}

template<typename T1>
std::deque<T1> InsertSort(const std::deque<T1> &arg1) {
    // InsertSort xs = InsertSortPart xs Nil
    auto temp0 = std::move(arg1);
    auto temp1 = std::deque<T1>();
    return InsertSortPart(std::move(temp0), std::move(temp1));
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
        auto temp1 = arg1;
        auto temp0 = MergeSort(std::deque<T1>(arg1.begin(), arg1.begin() + size(std::move(temp1)) / 2));
        auto temp3 = arg1;
        auto temp4 = size(std::move(temp3)) / 2;
        auto temp5 = std::move(arg1);
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

// generated by HOL2Cpp
