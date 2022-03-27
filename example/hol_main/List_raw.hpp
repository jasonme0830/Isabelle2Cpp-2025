#include <cstdint>
#include <cstdlib>
#include <functional>
#include <list>
#include <optional>
#include <set>
#include <utility>

template<typename T1>
T1 last(std::list<T1> arg1) {
    // last (x # xs) = (if xs = [] then x else last xs)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    T1 temp0;
    if (xs.empty()) {
        temp0 = x;
    } else {
        temp0 = last(std::move(xs));
    }
    return temp0;
}

template<typename T1>
std::list<T1> butlast(std::list<T1> arg1) {
    // butlast [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // butlast (x # xs) = (if xs = [] then [] else x # butlast xs)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    std::list<T1> temp0;
    if (xs.empty()) {
        temp0 = std::list<T1>();
    } else {
        auto temp1 = butlast(std::move(xs));
        temp1.push_front(x);
        temp0 = std::move(temp1);
    }
    return temp0;
}

template<typename T1>
std::list<T1> rev(std::list<T1> arg1) {
    // rev [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // rev (x # xs) = rev xs @ [x]
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = rev(std::move(xs));
    auto temp1 = std::list<T1>{x};
    temp0.splice(temp0.end(), temp1);
    return temp0;
}

template<typename T1>
std::list<T1> filter(const std::function<bool(const T1 &)> &arg1, std::list<T1> arg2) {
    // filter P [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // filter P (x # xs) = (if P x then x # filter P xs else filter P xs)
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::list<T1> temp0;
    if (arg1(x)) {
        auto temp1 = filter(arg1, std::move(xs));
        temp1.push_front(x);
        temp0 = std::move(temp1);
    } else {
        temp0 = filter(arg1, std::move(xs));
    }
    return temp0;
}

template<typename T1, typename T2>
T1 foldl(const std::function<T1(const T1 &, const T2 &)> &arg1, const T1 &arg2, std::list<T2> arg3) {
    // foldl f a [] = a
    if (arg3.empty()) {
        return arg2;
    }

    // foldl f a (x # xs) = foldl f (f a x) xs
    auto x = arg3.front();
    arg3.erase(arg3.begin(), std::next(arg3.begin(), 1));
    auto xs = std::move(arg3);
    return foldl(arg1, arg1(arg2, x), std::move(xs));
}

template<typename T1>
std::list<T1> concat(std::list<std::list<T1>> arg1) {
    // concat [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // concat (x # xs) = x @ concat xs
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = std::move(x);
    auto temp1 = concat(std::move(xs));
    temp0.splice(temp0.end(), temp1);
    return temp0;
}

template<typename T1>
std::list<T1> list_update(std::list<T1> arg1, const std::uint64_t &arg2, const T1 &arg3) {
    // list_update [] i v = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // list_update (x # xs) i v = ...
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = ([&] {
        auto temp1 = arg2;

        // 0 \<Rightarrow> v # xs
        if (temp1 == 0) {
            auto temp2 = std::move(xs);
            temp2.push_front(arg3);
            return std::move(temp2);
        }

        // Suc j \<Rightarrow> x # list_update xs j v
        auto temp3 = list_update(std::move(xs), j, arg3);
        temp3.push_front(x);
        return std::move(temp3);
    })();
    return temp0;
}

template<typename T1>
std::list<T1> takeWhile(const std::function<bool(const T1 &)> &arg1, std::list<T1> arg2) {
    // takeWhile P [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // takeWhile P (x # xs) = (if P x then x # takeWhile P xs else [])
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::list<T1> temp0;
    if (arg1(x)) {
        auto temp1 = takeWhile(arg1, std::move(xs));
        temp1.push_front(x);
        temp0 = std::move(temp1);
    } else {
        temp0 = std::list<T1>();
    }
    return temp0;
}

template<typename T1>
std::list<T1> dropWhile(const std::function<bool(const T1 &)> &arg1, std::list<T1> arg2) {
    // dropWhile P [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // dropWhile P (x # xs) = (if P x then dropWhile P xs else x # xs)
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::list<T1> temp0;
    if (arg1(x)) {
        temp0 = dropWhile(arg1, std::move(xs));
    } else {
        auto temp1 = std::move(xs);
        temp1.push_front(x);
        temp0 = std::move(temp1);
    }
    return temp0;
}

template<typename T1, typename T2>
std::list<std::pair<T1, T2>> zip(std::list<T1> arg1, std::list<T2> arg2) {
    // zip xs [] = []
    if (arg2.empty()) {
        return std::list<std::pair<T1, T2>>();
    }

    // zip xs (y # ys) = ...
    auto y = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto ys = std::move(arg2);
    auto temp0 = ([&] {
        auto temp1 = arg1;

        // [] \<Rightarrow> []
        if (temp1.empty()) {
            return std::list<std::pair<T1, T2>>();
        }

        // z # zs \<Rightarrow> (z, y) # zip zs ys
        auto temp2 = zip(std::move(zs), std::move(ys));
        temp2.push_front(std::make_pair(z, y));
        return std::move(temp2);
    })();
    return temp0;
}

template<typename T1>
std::list<T1> insert(const T1 &arg1, std::list<T1> arg2) {
    // insert x xs = (if x \<in> set xs then xs else x # xs)
    auto temp0 = std::move(arg2);
    std::list<T1> temp1;
    if (std::set(temp0.begin(), temp0.end()).count(arg1)) {
        temp1 = std::move(arg2);
    } else {
        auto temp2 = std::move(arg2);
        temp2.push_front(arg1);
        temp1 = std::move(temp2);
    }
    return temp1;
}

template<typename T1>
std::optional<T1> find(const std::function<bool(const T1 &)> &arg1, std::list<T1> arg2) {
    // find _ [] = None
    if (arg2.empty()) {
        return std::optional<T1>();
    }

    // find P (x#xs) = (if P x then Some x else find P xs)
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::optional<T1> temp0;
    if (arg1(x)) {
        temp0 = std::make_optional<T1>(x);
    } else {
        temp0 = find(arg1, std::move(xs));
    }
    return temp0;
}

template<typename T1>
std::uint64_t count_list(std::list<T1> arg1, const T1 &arg2) {
    // count_list [] y = 0
    if (arg1.empty()) {
        return 0;
    }

    // count_list (x#xs) y = (if x=y then count_list xs y + 1 else count_list xs y)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    std::uint64_t temp0;
    if (x == arg2) {
        temp0 = count_list(std::move(xs), arg2) + 1;
    } else {
        temp0 = count_list(std::move(xs), arg2);
    }
    return temp0;
}

template<typename T1>
std::list<T1> remove1(const T1 &arg1, std::list<T1> arg2) {
    // remove1 x [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // remove1 x (y # xs) = (if x = y then xs else y # remove1 x xs)
    auto y = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::list<T1> temp0;
    if (arg1 == y) {
        temp0 = std::move(xs);
    } else {
        auto temp1 = remove1(arg1, std::move(xs));
        temp1.push_front(y);
        temp0 = std::move(temp1);
    }
    return temp0;
}

template<typename T1>
std::list<T1> removeAll(const T1 &arg1, std::list<T1> arg2) {
    // removeAll x [] = []
    if (arg2.empty()) {
        return std::list<T1>();
    }

    // removeAll x (y # xs) = (if x = y then removeAll x xs else y # removeAll x xs)
    auto y = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    std::list<T1> temp0;
    if (arg1 == y) {
        temp0 = removeAll(arg1, std::move(xs));
    } else {
        auto temp1 = removeAll(arg1, std::move(xs));
        temp1.push_front(y);
        temp0 = std::move(temp1);
    }
    return temp0;
}

template<typename T1>
bool distinct(std::list<T1> arg1) {
    // distinct [] \<longleftrightarrow> True
    if (arg1.empty()) {
        return true;
    }

    // distinct (x # xs) \<longleftrightarrow> x \<notin> set xs \<and> distinct xs
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = xs;
    return !std::set(temp0.begin(), temp0.end()).count(x) && distinct(std::move(xs));
}

template<typename T1>
std::list<T1> remdups(std::list<T1> arg1) {
    // remdups [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // remdups (x # xs) = (if x \<in> set xs then remdups xs else x # remdups xs)
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = std::move(xs);
    std::list<T1> temp1;
    if (std::set(temp0.begin(), temp0.end()).count(x)) {
        temp1 = remdups(std::move(xs));
    } else {
        auto temp2 = remdups(std::move(xs));
        temp2.push_front(x);
        temp1 = std::move(temp2);
    }
    return temp1;
}

template<typename T1>
std::list<T1> remdups_adj(std::list<T1> arg1) {
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
    auto x = arg1.front();
    auto y = *std::next(arg1.begin(), 1);
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 2));
    auto xs = std::move(arg1);
    std::list<T1> temp0;
    if (x == y) {
        auto temp1 = std::move(xs);
        temp1.push_front(x);
        temp0 = remdups_adj(std::move(temp1));
    } else {
        auto temp2 = std::move(xs);
        temp2.push_front(y);
        auto temp3 = remdups_adj(std::move(temp2));
        temp3.push_front(x);
        temp0 = std::move(temp3);
    }
    return temp0;
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
std::list<std::pair<std::uint64_t, T1>> enumerate(const std::uint64_t &arg1, std::list<T1> arg2) {
    // enumerate n xs = zip [n..<n + length xs] xs
    auto temp0 = arg1;
    auto temp1 = arg1 + arg2.size();
    std::list<T1> temp2;
    for (auto temp3 = temp0; temp3 < temp1; ++temp3) {
        temp2.push_back(temp3);
    }
    return zip(temp2, std::move(arg2));
}

template<typename T1>
std::list<T1> rotate1(std::list<T1> arg1) {
    // rotate1 [] = []
    if (arg1.empty()) {
        return std::list<T1>();
    }

    // rotate1 (x # xs) = xs @ [x]
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = std::move(xs);
    auto temp1 = std::list<T1>{x};
    temp0.splice(temp0.end(), temp1);
    return temp0;
}

template<typename T1>
std::list<T1> nths(std::list<T1> arg1, const std::set<std::uint64_t> &arg2) {
    // nths xs A = map fst (filter (\<lambda>p. snd p \<in> A) (zip xs [0..<size xs]))
    std::function temp0 = [=] (const T1 &p) {
        return arg2.count(p.second);
    };
    auto temp1 = 0;
    auto temp2 = size(arg1);
    std::list<T2> temp3;
    for (auto temp4 = temp1; temp4 < temp2; ++temp4) {
        temp3.push_back(temp4);
    }
    return map(fst, filter(temp0, zip(arg1, temp3)));
}

template<typename T1>
std::list<T1> splice(std::list<T1> arg1, std::list<T1> arg2) {
    // splice [] ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // splice (x#xs) ys = x # splice ys xs
    auto x = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = splice(std::move(arg2), std::move(xs));
    temp0.push_front(x);
    return temp0;
}

template<typename T1>
T1 min_list(std::list<T1> arg1) {
    // min_list (x # xs) = (case xs of [] \<Rightarrow> x | _ \<Rightarrow> min x (min_list xs))
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto xs = std::move(arg1);
    auto temp0 = ([&] {
        auto temp1 = xs;

        // [] \<Rightarrow> x
        if (temp1.empty()) {
            return x;
        }

        // _ \<Rightarrow> min x (min_list xs)
        return min(x, min_list(std::move(xs)));
    })();
    return temp0;
}

template<typename T1, typename T2>
T1 arg_min_list(const std::function<T2(const T1 &)> &arg1, std::list<T1> arg2) {
    // arg_min_list f [x] = x
    if (arg2.size() == 1) {
        auto x = *std::next(arg2.begin(), 0);
        return x;
    }

    // arg_min_list f (x#y#zs) = (let m = arg_min_list f (y#zs) in if f x \<le> f m then x else m)
    auto x = arg2.front();
    auto y = *std::next(arg2.begin(), 1);
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 2));
    auto zs = std::move(arg2);
    auto temp1 = std::move(zs);
    temp1.push_front(y);
    auto temp0 = arg_min_list(arg1, std::move(temp1));
    auto m = temp0;
    T1 temp2;
    if (arg1(x) <= arg1(m)) {
        temp2 = x;
    } else {
        temp2 = m;
    }
    return temp2;
}

template<typename T1, typename T2>
std::list<T1> insort_key(const std::function<T2(const T1 &)> &arg1, const T1 &arg2, std::list<T1> arg3) {
    // insort_key f x [] = [x]
    if (arg3.empty()) {
        return std::list<T1>{arg2};
    }

    // insort_key f x (y#ys) = ...
    auto y = arg3.front();
    arg3.erase(arg3.begin(), std::next(arg3.begin(), 1));
    auto ys = std::move(arg3);
    std::list<T1> temp0;
    if (arg1(arg2) <= arg1(y)) {
        auto temp1 = std::move(ys);
        temp1.push_front(y);
        auto temp2 = std::move(temp1);
        temp2.push_front(arg2);
        temp0 = std::move(temp2);
    } else {
        auto temp3 = insort_key(arg1, arg2, std::move(ys));
        temp3.push_front(y);
        temp0 = std::move(temp3);
    }
    return temp0;
}

template<typename T1>
std::pair<std::list<T1>, std::list<T1>> partition(const std::function<bool(const T1 &)> &arg1, std::list<T1> arg2) {
    // partition P [] = ([], [])
    if (arg2.empty()) {
        return std::make_pair(std::list<T1>(), std::list<T1>());
    }

    // partition P (x # xs) = ...
    auto x = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto xs = std::move(arg2);
    auto temp0 = partition(arg1, std::move(xs));
    auto yes = temp0.first;
    auto no = temp0.second;
    std::pair<std::list<T1>, std::list<T1>> temp1;
    if (arg1(x)) {
        auto temp2 = std::move(yes);
        temp2.push_front(x);
        temp1 = std::make_pair(std::move(temp2), std::move(no));
    } else {
        auto temp3 = std::move(no);
        temp3.push_front(x);
        temp1 = std::make_pair(std::move(yes), std::move(temp3));
    }
    return temp1;
}

std::list<std::int64_t> upto_aux(const std::int64_t &arg1, const std::int64_t &arg2, std::list<std::int64_t> arg3);

template<typename T1>
std::set<std::list<T1>> listset(std::list<std::set<T1>> arg1) {
    // listset [] = {[]}
    if (arg1.empty()) {
        return std::set<std::list<T1>>{std::set<std::list<T1>>()};
    }

    // listset (A # As) = set_Cons A (listset As)
    auto A = arg1.front();
    arg1.erase(arg1.begin(), std::next(arg1.begin(), 1));
    auto As = std::move(arg1);
    return set_Cons(A, listset(std::move(As)));
}

template<typename T1, typename T2>
std::list<T2> bind(std::list<T1> arg1, const std::function<std::list<T2>(const T1 &)> &arg2) {
    // bind xs f = concat (map f xs)
    return concat(map(arg2, arg1));
}

template<typename T1, typename T2>
std::list<T2> map_tailrec_rev(const std::function<T2(const T1 &)> &arg1, std::list<T1> arg2, std::list<T2> arg3) {
    // map_tailrec_rev f [] bs = bs
    if (arg2.empty()) {
        return arg3;
    }

    // map_tailrec_rev f (a#as) bs = map_tailrec_rev f as (f a # bs)
    auto a = arg2.front();
    arg2.erase(arg2.begin(), std::next(arg2.begin(), 1));
    auto as = std::move(arg2);
    auto temp0 = std::move(arg3);
    temp0.push_front(arg1(a));
    return map_tailrec_rev(arg1, std::move(as), std::move(temp0));
}

template<typename T1, typename T2>
std::list<T2> map_tailrec(const std::function<T2(const T1 &)> &arg1, std::list<T1> arg2) {
    // map_tailrec f as = rev (map_tailrec_rev f as [])
    return rev(map_tailrec_rev(arg1, std::move(arg2), std::list<T2>()));
}

template<typename T1>
bool member(std::list<T1> arg1, const T1 &arg2) {
    // member xs x \<longleftrightarrow> x \<in> set xs
    auto temp0 = std::move(arg1);
    return std::set(temp0.begin(), temp0.end()).count(arg2);
}

template<typename T1>
bool list_ex(const std::function<bool(const T1 &)> &arg1, std::list<T1> arg2) {
    // list_ex P xs \<longleftrightarrow> Bex (set xs) P
    auto temp0 = std::move(arg2);
    return Bex(std::set(temp0.begin(), temp0.end()), arg1);
}

template<typename T1>
bool null(std::list<T1> arg1) {
    // null xs \<longleftrightarrow> xs = []
    return arg1.empty();
}

template<typename T1, typename T2>
std::list<T2> maps(const std::function<std::list<T2>(const T1 &)> &arg1, std::list<T1> arg2) {
    // maps f xs = concat (map f xs)
    return concat(map(arg1, arg2));
}

template<typename T1>
std::uint64_t gen_length(const std::uint64_t &arg1, std::list<T1> arg2) {
    // gen_length n xs = n + length xs
    return arg1 + arg2.size();
}

// generated by HOL2Cpp
