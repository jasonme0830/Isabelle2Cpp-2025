#pragma once

#include "token.hpp"
#include "error.hpp"

#include <vector>
#include <fstream>
#include <optional>

namespace hol2cpp {
/**
 * fst for line_num, snd for char_at_line
*/
using Location = std::pair<std::size_t, std::size_t>;

class Tokenizer {
  public:
    static std::optional<Token::Type> add_token(const std::string &literal);

  public:
    Tokenizer(std::ifstream &input, std::string name) noexcept;

    Token next_token();
    // this is special for (infix "..." <NUM>)
    std::string next_raw_str();

    void get_next_input();

    std::string get_err_info(const std::string &message) const;

    Location last_location() const;
    const std::vector<std::string> &file_content() const;

  private:
    TokenizeError error(const std::string &message);
    std::optional<Token> try_get_symbol();

    std::ifstream &input_;
    std::string name_of_input_;
    char last_input_;

    Location cur_location_;
    Location last_location_;
    std::vector<std::string> file_content_;
};
} // namespace hol2cpp
