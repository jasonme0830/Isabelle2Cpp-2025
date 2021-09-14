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
    ".", ";", "\\", "!", "`",

    ":", "::", ":=", "=", "|", "\"", "(", ")", "*", R"(\<Rightarrow>)",
    "[", "]", "{", "}", ",", R"(\<or>)", R"(\<and>)", R"(\<noteq>)",
    R"(\<le>)", R"(<)", R"(\<ge>)", R"(>)", R"(\<subseteq>)", R"(\<subset>)",
    R"(\<supseteq>)", R"(\<supset>)", R"(\<in>)", R"(\<notin>)", R"(#)", R"(@)",
    R"(\<union>)", R"(\<inter>)", R"(+)", R"(-)", R"(*)", R"(/)", R"(div)", R"(mod)", R"(^)",
};

Tokenizer::Tokenizer(ifstream &input, std::string name) noexcept
  : input_(input), name_of_input_(move(name)), last_input_(' ')
  , cur_location_(1, 0), last_location_(1, 0) {
    // ...
}

void Tokenizer::get_next_input() {
    last_input_ = input_.get();
    if (last_input_ == EOF) {
        return;
    }

    if (last_input_ == '\n') {
        ++cur_location_.first;
        cur_location_.second = 0;
        last_line_ = cur_line_;
        cur_line_.clear();
    } else {
        ++cur_location_.second;
        cur_line_ += last_input_;
    }
}

std::string Tokenizer::next_raw_str() {
    while (std::isspace(last_input_)) {
        get_next_input();
    }

    if (last_input_ != '"') {
        throw error("expected char \"");
    }
    get_next_input();

    string value;
    while (last_input_ != '"') {
        value.push_back(last_input_);
        get_next_input();
    }
    get_next_input(); // eat "

    return value;
}

optional<Token> Tokenizer::try_get_symbol() {
    auto longest_len = max_element(localSymbolSet.begin(), localSymbolSet.end(), [] (const string &lhs, const string &rhs) { return lhs.size() < rhs.size(); })->size();

    vector<char> backups;
    backups.push_back(last_input_);

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

        get_next_input();
        backups.push_back(last_input_);
        if (last_input_ == EOF) {
            break;
        }
    }

    size_t tell_g = input_.tellg();
    if (alts.empty()) {
        input_.seekg(tell_g - i, ios::beg);
        last_input_ = backups[0];
        return {};
    } else {
        auto symbol = *max_element(alts.begin(), alts.end(), [] (const string &lhs, const string &rhs) { return lhs.size() < rhs.size(); });
        input_.seekg(tell_g - i + symbol.size(), ios::beg);
        last_input_ = backups[symbol.size()];
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

                        if (token = try_get_symbol()) {
                            return *token;
                        }
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
    auto line = (cur_location_.first != last_location_.first)
        ? last_line_ : cur_line_
    ;

    auto line_num = last_location_.first,
         char_at_line = last_location_.second
    ;

    return "$ $: \n$\n$$"_fs.format(
        info::strong("$:$:$:"_fs.format(name_of_input_, line_num, char_at_line)),
        info::light_red("error: "), message,
        line, string(char_at_line == 0 ? 0 : char_at_line - 1, ' '), info::light_red("^")
    );
}

TokenizeError Tokenizer::error(const string &message) {
    auto backup = last_location_;

    last_location_ = cur_location_;
    auto err = get_err_info(message);

    last_location_ = backup;
    return TokenizeError(err);
}
} // namespace hol2cpp
