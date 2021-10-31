#include "merge_sort.hpp"

std::list<std::uint64_t> merge(const std::list<std::uint64_t> &arg1, const std::list<std::uint64_t> &arg2) {
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
        auto xs = decltype(arg1){std::next(arg1.begin()), arg1.end()};
        if (!arg2.empty()) {
            auto y = arg2.front();
            auto ys = decltype(arg2){std::next(arg2.begin()), arg2.end()};
            std::list<std::uint64_t> temp0;
            if (x <= y) {
                auto temp1 = ys;
                temp1.push_front(y);
                auto temp2 = merge(xs, temp1);
                temp2.push_front(x);
                temp0 = temp2;
            } else {
                auto temp3 = xs;
                temp3.push_front(x);
                auto temp4 = merge(temp3, ys);
                temp4.push_front(y);
                temp0 = temp4;
            }
            return temp0;
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

std::list<std::uint64_t> merge_sort(const std::list<std::uint64_t> &arg1) {
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

template<typename T1>
std::uint64_t slength(const slist<T1> &arg1) {
    // slength sNil = 0
    if (arg1->cons == slistCons::sNil) {
        return 0;
    }

    // slength (sCons x xs) = Suc (slength xs)
    if (arg1->cons == slistCons::sCons) {
        auto xs = arg1->get_c2().p2;
        return (slength(xs)) + 1;
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> stake(const std::uint64_t &arg1, const slist<T1> &arg2) {
    // stake n sNil = sNil
    if (arg2->cons == slistCons::sNil) {
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }

    // stake 0 _ = sNil
    if (arg1 == 0) {
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }

    // stake (Suc m) (sCons x xs) = sCons x (stake m xs)
    if (arg1 != 0) {
        auto m = arg1 - 1;
        if (arg2->cons == slistCons::sCons) {
            auto x = arg2->get_c2().p1;
            auto xs = arg2->get_c2().p2;
            slist<T1> temp0 = std::make_shared<slistElem<T1>>(slistCons::sCons);
            temp0->set_c2(x, stake(m, xs));
            return temp0;
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

template<typename T1>
slist<T1> sdrop(const std::uint64_t &arg1, const slist<T1> &arg2) {
    // sdrop n sNil = sNil
    if (arg2->cons == slistCons::sNil) {
        return std::make_shared<slistElem<T1>>(slistCons::sNil);
    }

    // sdrop 0 xs = xs
    if (arg1 == 0) {
        return arg2;
    }

    // sdrop (Suc m) (sCons x xs) = sdrop m xs
    if (arg1 != 0) {
        auto m = arg1 - 1;
        if (arg2->cons == slistCons::sCons) {
            auto xs = arg2->get_c2().p2;
            return sdrop(m, xs);
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<std::uint64_t> smerge(const slist<std::uint64_t> &arg1, const slist<std::uint64_t> &arg2) {
    // smerge xs sNil = xs
    if (arg2->cons == slistCons::sNil) {
        return arg1;
    }

    // smerge sNil ys = ys
    if (arg1->cons == slistCons::sNil) {
        return arg2;
    }

    // smerge (sCons x xs) (sCons y ys) = If (x \<le> y) (sCons x (smerge xs (sCons y ys))) (sCons y (smerge (sCons x xs) ys))
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        auto xs = arg1->get_c2().p2;
        if (arg2->cons == slistCons::sCons) {
            auto y = arg2->get_c2().p1;
            auto ys = arg2->get_c2().p2;
            slist<std::uint64_t> temp0;
            if (x <= y) {
                slist<std::uint64_t> temp1 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
                slist<std::uint64_t> temp2 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
                temp2->set_c2(y, ys);
                temp1->set_c2(x, smerge(xs, temp2));
                temp0 = temp1;
            } else {
                slist<std::uint64_t> temp3 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
                slist<std::uint64_t> temp4 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
                temp4->set_c2(x, xs);
                temp3->set_c2(y, smerge(temp4, ys));
                temp0 = temp3;
            }
            return temp0;
        }
    } else { // auto-generated for -Wreturn-type
        std::abort();
    }
}

slist<std::uint64_t> smerge_sort(const slist<std::uint64_t> &arg1) {
    // smerge_sort sNil = sNil
    if (arg1->cons == slistCons::sNil) {
        return std::make_shared<slistElem<std::uint64_t>>(slistCons::sNil);
    }

    // smerge_sort (sCons x sNil) = sCons x sNil
    if (arg1->cons == slistCons::sCons) {
        auto x = arg1->get_c2().p1;
        if (arg1->get_c2().p2->cons == slistCons::sNil) {
            slist<std::uint64_t> temp0 = std::make_shared<slistElem<std::uint64_t>>(slistCons::sCons);
            temp0->set_c2(x, std::make_shared<slistElem<std::uint64_t>>(slistCons::sNil));
            return temp0;
        }
    }

    // smerge_sort xs = smerge (smerge_sort (stake ((slength xs) div 2) xs)) (smerge_sort (sdrop ((slength xs) div 2) xs))
    return smerge(smerge_sort(stake(slength(arg1) / 2, arg1)), smerge_sort(sdrop(slength(arg1) / 2, arg1)));
}

