#include "rule.hpp"

std::optional<std::uint64_t> bs(std::uint64_t arg1, std::deque<std::uint64_t> arg2) {
    // bs x [] = None
    if (arg2.empty()) {
        return std::optional<std::uint64_t>();
    }

    // bs x [y] = If (x = y) (Some 0) None
    if (arg2.size() == 1) {
        auto x = std::move(arg1);
        auto y = arg2[0];
        std::optional<std::uint64_t> temp0;
        if (x == y) {
            temp0 = std::make_optional<std::uint64_t>(0);
        } else {
            temp0 = std::optional<std::uint64_t>();
        }
        return temp0;
    }

    // bs x ys = (let m = (length ys) div 2 in ...
    auto x = std::move(arg1);
    auto ys = std::move(arg2);
    auto temp0 = ys.size() / 2;
    auto m = temp0;
    auto temp2 = ys;
    auto temp1 = temp2[m];
    auto y = temp1;
    std::optional<std::uint64_t> temp3;
    if (y == x) {
        temp3 = std::make_optional<std::uint64_t>(std::move(m));
    } else {
        std::optional<std::uint64_t> temp4;
        if (y < x) {
            auto temp5 = ([&] {
                auto temp6 = m + 1;
                auto temp7 = std::move(ys);
                temp7.erase(temp7.begin(), std::next(temp7.begin(), temp6));
                auto temp8 = bs(std::move(x), std::move(temp7));

                // Some n \<Rightarrow> Some (m + n + 1)
                if (temp8.has_value()) {
                    auto n = temp8.value();
                    return std::make_optional<std::uint64_t>((m + n) + 1);
                }

                // None \<Rightarrow> None
                return std::optional<std::uint64_t>();
            })();
            temp4 = temp5;
        } else {
            auto temp9 = std::move(m);
            auto temp10 = std::move(ys);
            temp10.erase(std::next(temp10.begin(), temp9), temp10.end());
            temp4 = bs(std::move(x), std::move(temp10));
        }
        temp3 = std::move(temp4);
    }
    return temp3;
}

std::uint64_t two_fib(std::uint64_t arg1) {
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
}

std::uint64_t three_fib(std::uint64_t arg1) {
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
}

std::uint64_t four_fib(std::uint64_t arg1) {
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
}

std::uint64_t Jacobsthal(std::uint64_t arg1) {
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
}

std::uint64_t Narayana(std::uint64_t arg1) {
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
}

std::uint64_t Leonardo(std::uint64_t arg1) {
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
}

std::uint64_t Padua(std::uint64_t arg1) {
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
}

std::uint64_t Pell(std::uint64_t arg1) {
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
}

std::uint64_t Lucas(std::uint64_t arg1) {
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
}

std::deque<std::uint64_t> supto(std::uint64_t arg1, std::uint64_t arg2) {
    // supto i j = (if i \<ge> j then [] else i # supto (i + 1) j)
    auto i = std::move(arg1);
    auto j = std::move(arg2);
    std::deque<std::uint64_t> temp0;
    if (i >= j) {
        temp0 = std::deque<std::uint64_t>();
    } else {
        auto temp1 = supto(i + 1, std::move(j));
        temp1.push_front(i);
        temp0 = std::move(temp1);
    }
    return temp0;
}

// generated by HOL2Cpp
