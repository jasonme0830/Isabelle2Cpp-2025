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
    { "end", Token::Type::SkipKeyword },
    { ".", Token::Type::SkipKeyword },
    { ";", Token::Type::SkipKeyword },
    { "\\", Token::Type::SkipKeyword },
    { "!", Token::Type::SkipKeyword },
    { "`", Token::Type::SkipKeyword },

    { "(*", Token::Type::CommentStart },
    { "*)", Token::Type::CommentEnd },

    { "theory", Token::Type::Theory },
    { "imports", Token::Type::Imports },
    { "begin", Token::Type::Begin },

    { "datatype", Token::Type::Datatype },

    { "fun", Token::Type::Function },
    { "function", Token::Type::Function },
    { "primrec", Token::Type::Function },
    { "definition", Token::Type::Function },
    { "where", Token::Type::Where },

    { "if", Token::Type::If },
    { "else", Token::Type::Else },
    { "then", Token::Type::Then },

    { ":", Token::Type::Colon },
    { "::", Token::Type::Colonn },
    { ":=", Token::Type::ColonEquiv },
    { "=", Token::Type::Equiv },
    { "|", Token::Type::Pipe },
    { "\"", Token::Type::Quotation },
    { "(", Token::Type::LParen },
    { ")", Token::Type::RParen },
    { "*", Token::Type::Star },
    { R"(\<Rightarrow>)", Token::Type::Rightarrow },
    { "[", Token::Type::LBracket },
    { "]", Token::Type::RBracket },
    { "{", Token::Type::LBrace },
    { "}", Token::Type::RBrace },
    { ",", Token::Type::Comma },
    { R"(\<or>)", Token::Type::Or },
    { R"(\<and>)", Token::Type::And },
    { R"(\<noteq>)", Token::Type::NotEq },
    { R"(\<le>)", Token::Type::Le },
    { R"(<)", Token::Type::Lt },
    { R"(\<ge>)", Token::Type::Ge },
    { R"(>)", Token::Type::Gt },
    { R"(\<subseteq>)", Token::Type::SubsetEq },
    { R"(\<subset>)", Token::Type::Subset },
    { R"(\<supseteq>)", Token::Type::SupsetEq },
    { R"(\<supset>)", Token::Type::Supset },
    { R"(\<in>)", Token::Type::In },
    { R"(\<notin>)", Token::Type::NotIn },
    { R"(#)", Token::Type::Sharp },
    { R"(@)", Token::Type::At },
    { R"(\<union>)", Token::Type::Union },
    { R"(\<inter>)", Token::Type::Inter },
    { R"(+)", Token::Type::Add },
    { R"(-)", Token::Type::Sub },
    { R"(*)", Token::Type::Mul },
    { R"(/)", Token::Type::Div },
    { R"(div)", Token::Type::Div },
    { R"(mod)", Token::Type::Mod },
    { R"(^)", Token::Type::Pow },
};

Token::Token(string value) noexcept
  : value(move(value)) {
    auto it = localMapping.find(this->value);
    type = (it == localMapping.end() ? Token::Type::Identifier : it->second);
}
} // namespace hol2cpp
