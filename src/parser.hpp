#pragma once

#include "ast.hpp"
#include "token.hpp"

#include <fstream>
#include <exception>

namespace hol2cpp {
class Parser {
  public:
    Parser(std::ifstream &input) noexcept;

    Theory gen_theory();

  private:
    template<Token::Type type, Token::Type ...types>
    void check(const std::string &err_info) {
        if (current_token_.type != type)
        {
            if constexpr (sizeof...(types)) {
                check<types...>(err_info);
            } else {
                throw std::runtime_error(err_info);
            }
        }
    }
};
} // namespace hol2cpp
