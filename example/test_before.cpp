#include "test_before.hpp"

std::optional<std::uint64_t> bs(const std::uint64_t &arg1, std::list<std::uint64_t> arg2) {
    // bs x [] = None
    if (arg2.empty()) {
        return std::optional<std::uint64_t>();
    }

    // bs x [y] = If (x = y) (Some 0) None
    if (arg2.size() == 1) {
        auto x = arg1;
        auto y = *std::next(arg2.begin(), 0);
        std::optional<std::uint64_t> temp2;
        if (x == y) {
            temp2 = std::make_optional<std::uint64_t>(0);
        } else {
            temp2 = std::optional<std::uint64_t>();
        }
        return temp2;
    }

    // bs x ys = (let m = (length ys) div 2 in ...
    auto x = arg1;
    auto ys = std::move(arg2);
    auto temp0 = ys.size() / 2;
    auto m = temp0;
    auto temp4 = ys;
    auto temp3 = *std::next(temp4.begin(), m);
    auto y = temp3;
    std::optional<std::uint64_t> temp7;
    if (y == x) {
        temp7 = std::make_optional<std::uint64_t>(m);
    } else {
        std::optional<std::uint64_t> temp10;
        if (y < x) {
            auto temp11 = ([&] {
                auto temp15 = m + 1;
                auto temp16 = std::move(ys);
                temp16.erase(temp16.begin(), std::next(temp16.begin(), temp15));
                auto temp12 = std::move(temp16);
                auto temp17 = bs(x, std::move(temp12));

                // Some n \<Rightarrow> Some (m + n + 1)
                if (temp17.has_value()) {
                    auto n = temp17.value();
                    return std::make_optional<std::uint64_t>((m + n) + 1);
                }

                // None \<Rightarrow> None
                return std::optional<std::uint64_t>();
            })();
            temp10 = temp11;
        } else {
            auto temp25 = m;
            auto temp26 = std::move(ys);
            temp26.erase(std::next(temp26.begin(), temp25), temp26.end());
            auto temp24 = std::move(temp26);
            temp10 = bs(x, std::move(temp24));
        }
        temp7 = temp10;
    }
    return temp7;
}

std::uint64_t two_fib(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // two_fib 0 = 1
        if (arg1 == 0) {
            return 1;
        }

        // two_fib (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // two_fib n = (two_fib (n-1)) + (two_fib (n-2))
        auto n = arg1;
        return two_fib(n - 1) + two_fib(n - 2);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t three_fib(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // three_fib 0 = 1
        if (arg1 == 0) {
            return 1;
        }

        // three_fib (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // three_fib (Suc (Suc 0)) = 1
        if (arg1 != 0) {
            if (arg1 - 1 != 0) {
                if ((arg1 - 1) - 1 == 0) {
                    return 1;
                }
            }
        }

        // three_fib n = (three_fib (n-1) + three_fib (n-2) + three_fib(n-3))
        auto n = arg1;
        return (three_fib(n - 1) + three_fib(n - 2)) + three_fib(n - 3);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t four_fib(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // four_fib 0 = 1
        if (arg1 == 0) {
            return 1;
        }

        // four_fib (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // four_fib (Suc (Suc 0)) = 1
        if (arg1 != 0) {
            if (arg1 - 1 != 0) {
                if ((arg1 - 1) - 1 == 0) {
                    return 1;
                }
            }
        }

        // four_fib (Suc (Suc (Suc 0))) = 1
        if (arg1 != 0) {
            if (arg1 - 1 != 0) {
                if ((arg1 - 1) - 1 != 0) {
                    if (((arg1 - 1) - 1) - 1 == 0) {
                        return 1;
                    }
                }
            }
        }

        // four_fib n = (four_fib(n-1) + four_fib(n-2) + four_fib(n-3) + four_fib(n-4))
        auto n = arg1;
        return ((four_fib(n - 1) + four_fib(n - 2)) + four_fib(n - 3)) + four_fib(n - 4);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Jacobsthal(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // Jacobsthal 0 = 2
        if (arg1 == 0) {
            return 2;
        }

        // Jacobsthal (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // Jacobsthal n = (Jacobsthal (n-1)) + 2*(Jacobsthal (n-2))
        auto n = arg1;
        return Jacobsthal(n - 1) + (2 * Jacobsthal(n - 2));
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Narayana(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // Narayana 0 = 1
        if (arg1 == 0) {
            return 1;
        }

        // Narayana (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // Narayana (Suc (Suc 0)) = 1
        if (arg1 != 0) {
            if (arg1 - 1 != 0) {
                if ((arg1 - 1) - 1 == 0) {
                    return 1;
                }
            }
        }

        // Narayana n = Narayana(n-1) + Narayana(n-3)
        auto n = arg1;
        return Narayana(n - 1) + Narayana(n - 3);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Leonardo(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // Leonardo 0 = 1
        if (arg1 == 0) {
            return 1;
        }

        // Leonardo (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // Leonardo n = Leonardo(n-1) + Leonardo(n-2) + 1
        auto n = arg1;
        return (Leonardo(n - 1) + Leonardo(n - 2)) + 1;
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Padua(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // Padua 0 = 1
        if (arg1 == 0) {
            return 1;
        }

        // Padua (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // Padua (Suc (Suc 0)) = 1
        if (arg1 != 0) {
            if (arg1 - 1 != 0) {
                if ((arg1 - 1) - 1 == 0) {
                    return 1;
                }
            }
        }

        // Padua n = Padua(n-2) + Padua(n-3)
        auto n = arg1;
        return Padua(n - 2) + Padua(n - 3);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Pell(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // Pell 0 = 0
        if (arg1 == 0) {
            return 0;
        }

        // Pell (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // Pell n = (Pell(n-1)*2)+Pell(n-2)
        auto n = arg1;
        return (Pell(n - 1) * 2) + Pell(n - 2);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Lucas(const std::uint64_t &arg1) {
    auto impl = [&]() -> std::uint64_t {
        // Lucas 0 = 2
        if (arg1 == 0) {
            return 2;
        }

        // Lucas (Suc 0) = 1
        if (arg1 != 0) {
            if (arg1 - 1 == 0) {
                return 1;
            }
        }

        // Lucas n = Lucas(n-1) + Lucas(n-2)
        auto n = arg1;
        return Lucas(n - 1) + Lucas(n - 2);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::list<std::uint64_t> supto(const std::uint64_t &arg1, const std::uint64_t &arg2) {
    // supto i j = (if i \<ge> j then [] else i # supto (i + 1) j)
    auto i = arg1;
    auto j = arg2;
    std::list<std::uint64_t> temp2;
    if (i >= j) {
        temp2 = std::list<std::uint64_t>();
    } else {
        auto temp5 = supto(i + 1, j);
        temp5.push_front(i);
        temp2 = std::move(temp5);
    }
    return temp2;
}

// generated by HOL2Cpp
