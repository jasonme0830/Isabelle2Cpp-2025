#pragma once

#include "token.hpp"

#include <fstream>
#include <optional>

namespace hol2cpp {
class Tokenizer {
  public:
    Tokenizer(std::ifstream &input) noexcept;

    Token next_token();
    // this is special for (infix "..." <NUM>)
    std::string next_raw_str();

    void get_next_input();

  private:
    std::optional<Token> try_get_symbol();

    std::ifstream &input_;
    char last_input_;
};
} // namespace hol2cpp
