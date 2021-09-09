#include "token.hpp"

#include <map>

using namespace std;

namespace hol2cpp {
Token::Token(Type type) noexcept
  : type(type) {
    // ...
}

Token::Token(Type type, string value) noexcept
  : type(type), value(move(value)) {
    // ...
}

Token::Token(Token &&other) noexcept
  : type(other.type), value(std::move(other.value)) {
    // ...
}

Token::Token(const Token &other) noexcept
  : type(other.type), value(other.value) {
    // ...
}

Token &Token::operator=(Token &&other) noexcept {
    type = other.type;
    value = std::move(other.value);
    return *this;
}

Token &Token::operator=(const Token &other) noexcept {
    type = other.type;
    value = other.value;
    return *this;
}

static map<string, Token::Type> localMapping {
    { "theory", Token::Type::Theory },
    { "imports", Token::Type::Imports },
    { "begin", Token::Type::Begin },
    { "end", Token::Type::End },

    { "datatype", Token::Type::Datatype },

    { "fun", Token::Type::Fun },
    { "function", Token::Type::Function },
    { "primrec", Token::Type::Primrec },
    { "where", Token::Type::Where },

    { ":", Token::Type::Colon },
    { "::", Token::Type::Colonn },
    { ":=", Token::Type::ColonEquiv },
};

Token::Token(string value) noexcept
  : value(move(value)) {
    auto it = localMapping.find(this->value);
    type = (it == localMapping.end() ? Token::Type::Identifier : it->second);
}
} // namespace hol2cpp
