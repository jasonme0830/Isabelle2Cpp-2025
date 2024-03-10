#include <cstdint>
#include <cstdlib>
#include <functional>
#include <list>
#include <optional>
#include <set>
#include <utility>

template<typename T1>
T1 last(const std::list<T1> &arg1) {
    // last (x # xs) = (if xs = [] then x else last xs)
    if (!arg1.empty()) {
        auto x = arg1.front();
        auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
        T1 temp0;
        if (xs.empty()) {
            temp0 = x;
        } else {
            temp0 = last(xs);
        }
        return temp0;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
std::list<T1> butlast(const std::list<T1> &arg1) {
    // butlast [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // butlast (x # xs) = (if xs = [] then [] else x # butlast xs)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    std::list<T1> temp0;
    if (xs.empty()) {
        temp0 = std::list<T1>();
    } else {
        auto temp1 = butlast(xs);
        temp1.push_front(x);
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
std::list<T1> rev(const std::list<T1> &arg1) {
    // rev [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // rev (x # xs) = rev xs @ [x]
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = rev(xs);
    auto temp1 = std::list<T1>{x};
    temp0.splice(temp0.end(), temp1);
    return temp0;
}

template<typename T1>
std::list<T1> filter(const std::function<bool(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // filter P [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // filter P (x # xs) = (if P x then x # filter P xs else filter P xs)
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (arg1(x)) {
        auto temp1 = filter(arg1, xs);
        temp1.push_front(x);
        temp0 = temp1;
    } else {
        temp0 = filter(arg1, xs);
    }
    return temp0;
}

template<typename T1, typename T2>
T1 foldl(const std::function<T1(const T1 &, const T2 &)> &arg1, const T1 &arg2, const std::list<T2> &arg3) {
    // foldl f a [] = a
    if (arg3.empty()) {
        return arg2;
    }

    // foldl f a (x # xs) = foldl f (f a x) xs
    auto x = arg3.front();
    auto xs = std::list<T2>(std::next(arg3.begin()), arg3.end());
    return foldl(arg1, arg1(arg2, x), xs);
}

template<typename T1>
std::list<T1> list_update(const std::list<T1> &arg1, const std::uint64_t &arg2, const T1 &arg3) {
    // list_update [] i v = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // list_update (x # xs) i v = ...
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = ([&] {
        auto temp1 = arg2;

        // 0 \<Rightarrow> v # xs
        if (temp1 == 0) {
            auto temp2 = xs;
            temp2.push_front(arg3);
            return temp2;
        }

        // Suc j \<Rightarrow> x # list_update xs j v
        auto j = temp1 - 1;
        auto temp3 = list_update(xs, j, arg3);
        temp3.push_front(x);
        return temp3;
    })();
    return temp0;
}

template<typename T1>
std::list<T1> takeWhile(const std::function<bool(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // takeWhile P [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // takeWhile P (x # xs) = (if P x then x # takeWhile P xs else [])
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (arg1(x)) {
        auto temp1 = takeWhile(arg1, xs);
        temp1.push_front(x);
        temp0 = temp1;
    } else {
        temp0 = std::list<T1>();
    }
    return temp0;
}

template<typename T1>
std::list<T1> dropWhile(const std::function<bool(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // dropWhile P [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // dropWhile P (x # xs) = (if P x then dropWhile P xs else x # xs)
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (arg1(x)) {
        temp0 = dropWhile(arg1, xs);
    } else {
        auto temp1 = xs;
        temp1.push_front(x);
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1, typename T2>
std::list<std::pair<T1, T2>> zip(const std::list<T1> &arg1, const std::list<T2> &arg2) {
    // zip xs [] = []
    if (arg2.empty()) {
        return std::list<std::pair<T1, T2>>();
    }

    // zip xs (y # ys) = ...
    auto y = arg2.front();
    auto ys = std::list<T2>(std::next(arg2.begin()), arg2.end());
    auto temp0 = ([&] {
        auto temp1 = arg1;

        // [] \<Rightarrow> []
        if (temp1.empty()) {
            return std::list<std::pair<T1, T2>>();
        }

        // z # zs \<Rightarrow> (z, y) # zip zs ys
        auto z = temp1.front();
        auto zs = std::list<T1>(std::next(temp1.begin()), temp1.end());
        auto temp2 = zip(zs, ys);
        temp2.push_front(std::make_pair(z, y));
        return temp2;
    })();
    return temp0;
}

template<typename T1, typename T2, typename T3>
std::list<T3> map2(const std::function<T3(const T1 &, const T2 &)> &arg1, const std::list<T1> &arg2, const std::list<T2> &arg3) {
    // map2 f xs ys \<equiv> map (\<lambda>(x,y). f x y) (zip xs ys)
    std::function temp0 = [=] (const std::pair<T1, T2> &x, const T2 &y) {
        return arg1(x, y);
    };
    return map(temp0, zip(arg2, arg3));
}

template<typename T1>
std::list<T1> insert(const T1 &arg1, const std::list<T1> &arg2) {
    // insert x xs = (if x \<in> set xs then xs else x # xs)
    auto temp0 = arg2;
    std::list<T1> temp1;
    if (std::set<T1>(temp0.begin(), temp0.end()).count(arg1)) {
        temp1 = arg2;
    } else {
        auto temp2 = arg2;
        temp2.push_front(arg1);
        temp1 = temp2;
    }
    return temp1;
}

template<typename T1>
std::optional<T1> find(const std::function<bool(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // find _ [] = None
    if (arg2.empty()) {
        return std::optional<T1>();
    }

    // find P (x#xs) = (if P x then Some x else find P xs)
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::optional<T1> temp0;
    if (arg1(x)) {
        temp0 = std::make_optional<T1>(x);
    } else {
        temp0 = find(arg1, xs);
    }
    return temp0;
}

template<typename T1>
std::uint64_t count_list(const std::list<T1> &arg1, const T1 &arg2) {
    // count_list [] y = 0
    if (arg1.empty()) {
        return 0;
    }

    // count_list (x#xs) y = (if x=y then count_list xs y + 1 else count_list xs y)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    std::uint64_t temp0;
    if (x == arg2) {
        temp0 = count_list(xs, arg2) + 1;
    } else {
        temp0 = count_list(xs, arg2);
    }
    return temp0;
}

template<typename T1>
std::list<T1> remove1(const T1 &arg1, const std::list<T1> &arg2) {
    // remove1 x [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // remove1 x (y # xs) = (if x = y then xs else y # remove1 x xs)
    auto y = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (arg1 == y) {
        temp0 = xs;
    } else {
        auto temp1 = remove1(arg1, xs);
        temp1.push_front(y);
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
std::list<T1> removeAll(const T1 &arg1, const std::list<T1> &arg2) {
    // removeAll x [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // removeAll x (y # xs) = (if x = y then removeAll x xs else y # removeAll x xs)
    auto y = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    std::list<T1> temp0;
    if (arg1 == y) {
        temp0 = removeAll(arg1, xs);
    } else {
        auto temp1 = removeAll(arg1, xs);
        temp1.push_front(y);
        temp0 = temp1;
    }
    return temp0;
}

template<typename T1>
bool distinct(const std::list<T1> &arg1) {
    // distinct [] \<longleftrightarrow> True
    if (arg1.empty()) {
        return true;
    }

    // distinct (x # xs) \<longleftrightarrow> x \<notin> set xs \<and> distinct xs
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = xs;
    return !std::set<T1>(temp0.begin(), temp0.end()).count(x) && distinct(xs);
}

template<typename T1>
std::list<T1> remdups(const std::list<T1> &arg1) {
    // remdups [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // remdups (x # xs) = (if x \<in> set xs then remdups xs else x # remdups xs)
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = xs;
    std::list<T1> temp1;
    if (std::set<T1>(temp0.begin(), temp0.end()).count(x)) {
        temp1 = remdups(xs);
    } else {
        auto temp2 = remdups(xs);
        temp2.push_front(x);
        temp1 = temp2;
    }
    return temp1;
}

template<typename T1>
std::list<T1> remdups_adj(const std::list<T1> &arg1) {
    // remdups_adj [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // remdups_adj [x] = [x]
    if (arg1.size() == 1) {
        auto x = *std::next(arg1.begin(), 0);
        return std::list<T1>{x};
    }

    // remdups_adj (x # y # xs) = (if x = y then remdups_adj (x # xs) else x # remdups_adj (y # xs))
    std::list<T1> temp0(std::next(arg1.begin()), arg1.end());
    auto x = arg1.front();
    auto y = temp0.front();
    auto xs = std::list<T1>(std::next(temp0.begin()), temp0.end());
    std::list<T1> temp1;
    if (x == y) {
        auto temp2 = xs;
        temp2.push_front(x);
        temp1 = remdups_adj(temp2);
    } else {
        auto temp3 = xs;
        temp3.push_front(y);
        auto temp4 = remdups_adj(temp3);
        temp4.push_front(x);
        temp1 = temp4;
    }
    return temp1;
}

template<typename T1>
std::list<T1> replicate(const std::uint64_t &arg1, const T1 &arg2) {
    // replicate 0 x = []
    if (arg1 == 0) {
        return std::list<T1>();
    }

    // replicate (Suc n) x = x # replicate n x
    auto n = arg1 - 1;
    auto temp0 = replicate(n, arg2);
    temp0.push_front(arg2);
    return temp0;
}

template<typename T1>
std::list<std::pair<std::uint64_t, T1>> enumerate(const std::uint64_t &arg1, const std::list<T1> &arg2) {
    // enumerate n xs = zip [n..<n + length xs] xs
    auto temp0 = arg1;
    auto temp1 = arg1 + arg2.size();
    std::list<std::uint64_t> temp2;
    for (auto temp3 = temp0; temp3 < temp1; ++temp3) {
        temp2.push_back(temp3);
    }
    return zip(temp2, arg2);
}

template<typename T1>
std::list<T1> rotate1(const std::list<T1> &arg1) {
    // rotate1 [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // rotate1 (x # xs) = xs @ [x]
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = xs;
    auto temp1 = std::list<T1>{x};
    temp0.splice(temp0.end(), temp1);
    return temp0;
}

template<typename T1>
std::list<std::list<T1>> n_lists(const std::uint64_t &arg1, const std::list<T1> &arg2) {
    // n_lists 0 xs = [[]]
    if (arg1 == 0) {
        return std::list<std::list<T1>>{std::list<T1>()};
    }

    // n_lists (Suc n) xs = concat (map (\<lambda>ys. map (\<lambda>y. y # ys) xs) (n_lists n xs))
    auto n = arg1 - 1;
    std::function temp0 = [=] (const std::list<T1> &ys) {
        std::function temp1 = [=] (const T1 &y) {
            auto temp2 = ys;
            temp2.push_front(y);
            return temp2;
        };
        return map(temp1, arg2);
    };
    return concat(map(temp0, n_lists(n, arg2)));
}

template<typename T1>
std::list<T1> splice(const std::list<T1> &arg1, const std::list<T1> &arg2) {
    // splice [] ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // splice (x#xs) ys = x # splice ys xs
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = splice(arg2, xs);
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
T1 min_list(const std::list<T1> &arg1) {
    // min_list (x # xs) = (case xs of [] \<Rightarrow> x | _ \<Rightarrow> min x (min_list xs))
    auto x = arg1.front();
    auto xs = std::list<T1>(std::next(arg1.begin()), arg1.end());
    auto temp0 = ([&] {
        auto temp1 = xs;

        // [] \<Rightarrow> x
        if (temp1.empty()) {
            return x;
        }

        // _ \<Rightarrow> min x (min_list xs)
        return min(x, min_list(xs));
    })();
    return temp0;
}

template<typename T1, typename T2>
T1 arg_min_list(const std::function<T2(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // arg_min_list f [x] = x
    if (arg2.size() == 1) {
        auto x = *std::next(arg2.begin(), 0);
        return x;
    }

    // arg_min_list f (x#y#zs) = (let m = arg_min_list f (y#zs) in if f x \<le> f m then x else m)
    std::list<T1> temp0(std::next(arg2.begin()), arg2.end());
    auto x = arg2.front();
    auto y = temp0.front();
    auto zs = std::list<T1>(std::next(temp0.begin()), temp0.end());
    auto temp2 = zs;
    temp2.push_front(y);
    auto temp1 = arg_min_list(arg1, temp2);
    auto m = temp1;
    T1 temp3;
    if (arg1(x) <= arg1(m)) {
        temp3 = x;
    } else {
        temp3 = m;
    }
    return temp3;
}

template<typename T1, typename T2>
std::list<T1> insort_key(const std::function<T2(const T1 &)> &arg1, const T1 &arg2, const std::list<T1> &arg3) {
    // insort_key f x [] = [x]
    if (arg3.empty()) {
        return std::list<T1>{arg2};
    }

    // insort_key f x (y#ys) = ...
    auto y = arg3.front();
    auto ys = std::list<T1>(std::next(arg3.begin()), arg3.end());
    std::list<T1> temp0;
    if (arg1(arg2) <= arg1(y)) {
        auto temp1 = ys;
        temp1.push_front(y);
        auto temp2 = temp1;
        temp2.push_front(arg2);
        temp0 = temp2;
    } else {
        auto temp3 = insort_key(arg1, arg2, ys);
        temp3.push_front(y);
        temp0 = temp3;
    }
    return temp0;
}

template<typename T1>
std::pair<std::list<T1>, std::list<T1>> partition(const std::function<bool(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // partition P [] = ([], [])
    if (arg2.empty()) {
        return std::make_pair(std::list<T1>(), std::list<T1>());
    }

    // partition P (x # xs) = ...
    auto x = arg2.front();
    auto xs = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = partition(arg1, xs);
    auto yes = temp0.first;
    auto no = temp0.second;
    std::pair<std::list<T1>, std::list<T1>> temp1;
    if (arg1(x)) {
        auto temp2 = yes;
        temp2.push_front(x);
        temp1 = std::make_pair(temp2, no);
    } else {
        auto temp3 = no;
        temp3.push_front(x);
        temp1 = std::make_pair(yes, temp3);
    }
    return temp1;
}

std::list<std::int64_t> upto_aux(const std::int64_t &arg1, const std::int64_t &arg2, const std::list<std::int64_t> &arg3);

template<typename T1>
std::set<std::list<T1>> listset(const std::list<std::set<T1>> &arg1) {
    // listset [] = {[]}
    if (arg1.empty()) {
        return std::set<std::list<T1>>{std::list<T1>()};
    }

    // listset (A # As) = set_Cons A (listset As)
    auto A = arg1.front();
    auto As = std::list<std::set<T1>>(std::next(arg1.begin()), arg1.end());
    return set_Cons(A, listset(As));
}

template<typename T1, typename T2>
std::list<T2> bind(const std::list<T1> &arg1, const std::function<std::list<T2>(const T1 &)> &arg2) {
    // bind xs f = concat (map f xs)
    return concat(map(arg2, arg1));
}

template<typename T1, typename T2>
std::list<T2> map_tailrec_rev(const std::function<T2(const T1 &)> &arg1, const std::list<T1> &arg2, const std::list<T2> &arg3) {
    // map_tailrec_rev f [] bs = bs
    if (arg2.empty()) {
        return arg3;
    }

    // map_tailrec_rev f (a#as) bs = map_tailrec_rev f as (f a # bs)
    auto a = arg2.front();
    auto as = std::list<T1>(std::next(arg2.begin()), arg2.end());
    auto temp0 = arg3;
    temp0.push_front(arg1(a));
    return map_tailrec_rev(arg1, as, temp0);
}

template<typename T1, typename T2>
std::list<T2> map_tailrec(const std::function<T2(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // map_tailrec f as = rev (map_tailrec_rev f as [])
    return rev(map_tailrec_rev(arg1, arg2, std::list<T2>()));
}

template<typename T1>
bool member(const std::list<T1> &arg1, const T1 &arg2) {
    // member xs x \<longleftrightarrow> x \<in> set xs
    auto temp0 = arg1;
    return std::set<T1>(temp0.begin(), temp0.end()).count(arg2);
}

template<typename T1>
bool list_ex(const std::function<bool(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // list_ex P xs \<longleftrightarrow> Bex (set xs) P
    auto temp0 = arg2;
    return Bex(std::set<T1>(temp0.begin(), temp0.end()), arg1);
}

template<typename T1>
bool null(const std::list<T1> &arg1) {
    // null xs \<longleftrightarrow> xs = []
    return arg1.empty();
}

template<typename T1, typename T2>
std::list<T2> maps(const std::function<std::list<T2>(const T1 &)> &arg1, const std::list<T1> &arg2) {
    // maps f xs = concat (map f xs)
    return concat(map(arg1, arg2));
}

template<typename T1>
std::uint64_t gen_length(const std::uint64_t &arg1, const std::list<T1> &arg2) {
    // gen_length n xs = n + length xs
    return arg1 + arg2.size();
}

// generated by HOL2Cpp
