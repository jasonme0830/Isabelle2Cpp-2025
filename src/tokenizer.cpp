#include "tokenizer.hpp"

#include <set>
#include <vector>
#include <cassert>
#include <optional>
#include <exception>

using namespace std;

namespace hol2cpp {
static set<string> localSymbolSet {
    ":", "::", ":=", "=", "|", "\"", "(", ")", "*", R"(\<Rightarrow>)",
    "[", "]", "{", "}", ","
};

Tokenizer::Tokenizer(ifstream &input) noexcept
  : input_(input), last_input_(' ') {
    // ...
}

void Tokenizer::get_next_input() {
    last_input_ = input_.get();
}

std::string Tokenizer::next_raw_str() {
    while (std::isspace(last_input_)) {
        get_next_input();
    }

    assert(last_input_ == '"');
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
    auto longest_len = localSymbolSet.rbegin()->size();

    auto alts = localSymbolSet;
    for (size_t i = 0; i < longest_len; ++i) {
        set<string> temp;
        for (const auto &symbol : alts) {
            if (symbol.size() <= i || symbol[i] == last_input_) {
                temp.insert(symbol);
            }
        }
        alts = temp;
        get_next_input();
    }

    if (alts.empty()) {
        input_.seekg(input_.tellg() - longest_len);
        return {};
    } else {
        auto symbol = *alts.rbegin();
        input_.seekg(input_.tellg() - longest_len + symbol.size());
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

    auto state = State::Begin;

    std::optional<Token> token;
    string value;

    while (!token) {
        switch (state) {
            case State::Begin:
                if (auto token = try_get_symbol()) {
                    return *token;
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
                    throw std::runtime_error("meet unexpected char "s + last_input_);
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
                if (isalpha(last_input_) || last_input_ == '_') {
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
} // namespace hol2cpp
