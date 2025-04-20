#include "num.hpp"

std::uint64_t two_fib(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return two_fib(n - 1) + two_fib(n - 2);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t three_fib(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return (three_fib(n - 1) + three_fib(n - 2)) + three_fib(n - 3);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t four_fib(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return ((four_fib(n - 1) + four_fib(n - 2)) + four_fib(n - 3)) + four_fib(n - 4);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Jacobsthal(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return Jacobsthal(n - 1) + (2 * Jacobsthal(n - 2));
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Narayana(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return Narayana(n - 1) + Narayana(n - 3);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Leonardo(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return (Leonardo(n - 1) + Leonardo(n - 2)) + 1;
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Padua(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return Padua(n - 2) + Padua(n - 3);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Pell(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return (Pell(n - 1) * 2) + Pell(n - 2);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

std::uint64_t Lucas(std::uint64_t arg1) {
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
        auto n = std::move(arg1);
        return Lucas(n - 1) + Lucas(n - 2);
    };

    static std::map<std::tuple<std::uint64_t>, std::uint64_t> cache;
    auto args = std::make_tuple(arg1);
    auto it = cache.find(args);
    return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);
}

// generated by HOL2Cpp
