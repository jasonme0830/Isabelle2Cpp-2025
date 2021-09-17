#include "error.hpp"
#include "format.hpp"
#include "tokenizer.hpp"

#include <set>
#include <vector>
#include <optional>
#include <algorithm>

using namespace std;

namespace hol2cpp {
static set<string> localSymbolSet {
    ".", ";", "\\", "!", "`", "?",

    R"((*)", R"(*))", R"(\<open>)", R"(\<close>)",

    ":", "::", ":=", "=", R"(\<longleftrightarrow>)", "|", "\"", "(", ")",
    R"(\<Rightarrow>)", "=>", "[", "]", "{", "}", ",", R"(\<or>)", R"(\<and>)", R"(\<noteq>)",
    R"(\<le>)", R"(<)", R"(\<ge>)", R"(>)", R"(\<subseteq>)", R"(\<subset>)",
    R"(\<supseteq>)", R"(\<supset>)", R"(\<in>)", R"(\<notin>)", R"(#)", R"(@)",
    R"(\<union>)", R"(\<inter>)", R"(+)", R"(-)", R"(*)", R"(\<times>)", R"(/)", R"(div)", R"(mod)", R"(^)",
};

Tokenizer::Tokenizer(ifstream &input, std::string name) noexcept
  : input_(input), name_of_input_(move(name)), last_input_(' ')
  , cur_location_(1, 0), last_location_(1, 0) {
      string cur_line;
      while (!input_.eof()) {
          auto chr = input_.get();
          if (chr == '\n') {
              file_content_.push_back(cur_line);
              cur_line.clear();
          } else {
              cur_line.push_back(chr);
          }
      }
      if (!cur_line.empty()) {
          file_content_.push_back(move(cur_line));
      }

      input_.clear();
      input_.seekg(ios::beg);
}

void Tokenizer::get_next_input() {
    last_input_ = input_.get();
    if (last_input_ == EOF) {
        return;
    }

    if (last_input_ == '\n') {
        ++cur_location_.first;
        cur_location_.second = 0;
    } else {
        ++cur_location_.second;
    }
}

std::string Tokenizer::next_raw_str() {
    string value;
    while (last_input_ != '"') {
        value.push_back(last_input_);
        get_next_input();
    }
    get_next_input(); // eat "

    return value;
}

optional<Token> Tokenizer::try_get_symbol() {
    auto tellg = input_.tellg();
    auto backup = last_input_;

    auto cmp = [] (const string &lhs, const string &rhs) { return lhs.size() < rhs.size(); };
    auto longest_len = max_element(localSymbolSet.begin(), localSymbolSet.end(), cmp)->size();

    auto alts = localSymbolSet;
    size_t i = 0;
    for (; i < longest_len; ++i) {
        bool increasable = false;

        set<string> temp;
        for (const auto &symbol : alts) {
            if (symbol.size() <= i) {
                temp.insert(symbol);
            } else if (symbol[i] == last_input_) {
                temp.insert(symbol);
                increasable = true;
            }
        }

        swap(alts, temp);
        if (!increasable) {
            break;
        }

        last_input_ = input_.get();
        if (last_input_ == EOF) {
            break;
        }
    }

    input_.clear();
    input_.seekg(tellg);
    last_input_ = backup;
    if (alts.empty()) {
        return {};
    } else {
        auto symbol = *max_element(alts.begin(), alts.end(), cmp);
        for (size_t i = 0; i < symbol.size(); ++i) {
            get_next_input();
        }
        return Token(symbol);
    }
}

Token Tokenizer::next_token() {
    enum class State {
        Begin,

        InInteger,
        InIdentifier,
        InTypeVariable,
    };

    while (std::isspace(last_input_)) {
        get_next_input();
    }

    if (last_input_ == EOF) {
        return Token(Token::Type::EndOfFile);
    }

    last_location_ = cur_location_;
    auto state = State::Begin;

    std::optional<Token> token;
    string value;

    while (!token) {
        switch (state) {
            case State::Begin:
                if (auto token = try_get_symbol()) {
                    if (token->type == Token::Type::CommentStart) {
                        while (!token.has_value() || token->type != Token::Type::CommentEnd) {
                            while (last_input_ != '*' && last_input_ != EOF) {
                                get_next_input();
                            }
                            token = try_get_symbol();
                            if (!token.has_value()) {
                                get_next_input();
                            }
                        }
                        return next_token();
                    } else if (token->type == Token::Type::Open) {
                        size_t cnt = 1;
                        while (cnt) {
                            while (last_input_ != '\\' && last_input_ != EOF) {
                                get_next_input();
                            }
                            token = try_get_symbol();
                            if (!token.has_value()) {
                                get_next_input();
                            } else if (token->type == Token::Type::Open) {
                                ++cnt;
                            } else if (token->type == Token::Type::Close) {
                                --cnt;
                            }
                        }
                        return next_token();
                    } else {
                        return *token;
                    }
                }

                if (isdigit(last_input_)) {
                    state = State::InInteger;
                    value.push_back(last_input_);
                } else if (isalpha(last_input_) || last_input_ == '_') {
                    state = State::InIdentifier;
                    value.push_back(last_input_);
                } else if (last_input_ == '\'') {
                    state = State::InTypeVariable;
                } else {
                    throw error("meet unexpected char "s + last_input_);
                }
                break;

            case State::InInteger:
                if (isdigit(last_input_)) {
                    value.push_back(last_input_);
                } else {
                    return Token(Token::Type::Integer, value);
                }
                break;

            case State::InIdentifier:
                if (isalpha(last_input_) || last_input_ == '_' || isdigit(last_input_)) {
                    value.push_back(last_input_);
                } else {
                    return Token(value);
                }
                break;

            case State::InTypeVariable:
                if (isalpha(last_input_) || last_input_ == '_') {
                    value.push_back(last_input_);
                } else {
                    return Token(Token::Type::TypeVariable, value);
                }
                break;
        }

        if (last_input_ == EOF) {
            break;
        } else {
            get_next_input();
        }
    }

    if (!token) {
        return Token(Token::Type::EndOfFile);
    } else {
        return *token;
    }
}

string Tokenizer::get_err_info(const string &message) const {
    const auto &line_num = last_location_.first,
        &char_at_line = last_location_.second
    ;

    return "`$$$\n`$\n`$$"_fs.format(
        info::strong("$:$:$: "_fs.format(name_of_input_, line_num, char_at_line)),
        info::light_red("error: "), message, file_content_[line_num - 1],
        string(char_at_line == 0 ? 0 : char_at_line - 1, ' '), info::light_red("^")
    );
}

TokenizeError Tokenizer::error(const string &message) {
    return TokenizeError(get_err_info(message));
}
} // namespace hol2cpp
