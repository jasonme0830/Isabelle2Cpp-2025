#include "merge_sort.hpp"

std::list<std::uint64_t> merge(std::list<std::uint64_t> arg1, std::list<std::uint64_t> arg2) {
    // merge xs [] = xs
    if (arg2.empty()) {
        return arg1;
    }

    // merge [] ys = ys
    if (arg1.empty()) {
        return arg2;
    }

    // merge (x # xs) (y # ys) = If (x \<le> y) (x # (merge xs (y # ys))) (y # (merge (x # xs) ys))
    if (!arg1.empty()) {
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = std::move(arg1);

        if (!arg2.empty()) {
            auto y = arg2.front();
            arg2.pop_front();
            auto ys = std::move(arg2);

            std::list<std::uint64_t> temp0;
            if (x <= y) {
                auto temp1 = std::move(ys);
                temp1.push_front(y);
                auto temp2 = merge(std::move(xs), std::move(temp1));
                temp2.push_front(x);
                temp0 = std::move(temp2);
            } else {
                auto temp3 = std::move(xs);
                temp3.push_front(x);
                auto temp4 = merge(std::move(temp3), std::move(ys));
                temp4.push_front(y);
                temp0 = std::move(temp4);
            }
            return temp0;
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

std::list<std::uint64_t> merge_sort(std::list<std::uint64_t> arg1) {
    // merge_sort [] = []
    if (arg1.empty()) {
        return std::list<std::uint64_t>();
    }

    // merge_sort [x] = [x]
    if (arg1.size() == 1) {
        auto x = *std::next(arg1.begin(), 0);
        return std::list<std::uint64_t>{x};
    }

    // merge_sort xs = merge (merge_sort (take ((length xs) div 2) xs)) (merge_sort (drop ((length xs) div 2) xs))
    return merge(merge_sort(decltype(arg1){ arg1.begin(), std::next(arg1.begin(), arg1.size() / 2) }), merge_sort(decltype(arg1){ std::next(arg1.begin(), arg1.size() / 2), arg1.end() }));
}

slist<std::uint64_t> smerge(const slist<std::uint64_t> &arg1, const slist<std::uint64_t> &arg2) {
    // smerge xs sNil = xs
    if (arg2.is_sNil()) {
        return arg1;
    }

    // smerge sNil ys = ys
    if (arg1.is_sNil()) {
        return arg2;
    }

    // smerge (sCons x xs) (sCons y ys) = If (x \<le> y) (sCons x (smerge xs (sCons y ys))) (sCons y (smerge (sCons x xs) ys))
    if (arg1.is_sCons()) {
        auto x = arg1.as_sCons().p1();
        auto xs = arg1.as_sCons().p2();
        if (arg2.is_sCons()) {
            auto y = arg2.as_sCons().p1();
            auto ys = arg2.as_sCons().p2();
            slist<std::uint64_t> temp0;
            if (x <= y) {
                auto temp1 = slist<std::uint64_t>::sCons(
                    y, ys
                );
                auto temp2 = slist<std::uint64_t>::sCons(
                    x, smerge(xs, temp1)
                );
                temp0 = temp2;
            } else {
                auto temp3 = slist<std::uint64_t>::sCons(
                    x, xs
                );
                auto temp4 = slist<std::uint64_t>::sCons(
                    y, smerge(temp3, ys)
                );
                temp0 = temp4;
            }
            return temp0;
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<std::uint64_t> smerge_sort(const slist<std::uint64_t> &arg1) {
    // smerge_sort sNil = sNil
    if (arg1.is_sNil()) {
        return slist<std::uint64_t>::sNil();
    }

    // smerge_sort (sCons x sNil) = sCons x sNil
    if (arg1.is_sCons()) {
        auto x = arg1.as_sCons().p1();
        if (arg1.as_sCons().p2().is_sNil()) {
            auto temp0 = slist<std::uint64_t>::sCons(
                x, slist<std::uint64_t>::sNil()
            );
            return temp0;
        }
    }

    // smerge_sort xs = smerge (smerge_sort (stake ((slength xs) div 2) xs)) (smerge_sort (sdrop ((slength xs) div 2) xs))
    return smerge(smerge_sort(stake(slength(arg1) / 2, arg1)), smerge_sort(sdrop(slength(arg1) / 2, arg1)));
}
