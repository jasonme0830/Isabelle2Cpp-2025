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

static size_t localEndOfToken = size_t(Token::Type::EndOfFile);
static map<string, Token::Type> localTokenMapping {
    { "end", Token::Type::SkipKeyword },
    { ";", Token::Type::SkipKeyword },
    { "\\", Token::Type::SkipKeyword },
    { "`", Token::Type::SkipKeyword },
    { "?", Token::Type::SkipKeyword },

    { R"(\<comment>)", Token::Type::Comment },
    { "(*", Token::Type::CommentStart },
    { "*)", Token::Type::CommentEnd },
    { R"(\<open>)", Token::Type::Open },
    { R"(\<close>)", Token::Type::Close },

    { "theory", Token::Type::Theory },
    { "imports", Token::Type::Imports },
    { "begin", Token::Type::Begin },

    { "datatype", Token::Type::Datatype },

    { "fun", Token::Type::Function },
    { "function", Token::Type::Function },
    { "primrec", Token::Type::Function },
    { "definition", Token::Type::Function },
    { "abbreviation", Token::Type::Function },
    { "where", Token::Type::Where },

    { "infixl", Token::Type::Infixl },
    { "infixr", Token::Type::Infixr },

    { "if", Token::Type::If },
    { "else", Token::Type::Else },
    { "then", Token::Type::Then },

    { "let", Token::Type::Let },
    { "in", Token::Type::KeyIn },

    { "case", Token::Type::Case },
    { "of", Token::Type::Of },

    { R"(\<lambda>)", Token::Type::Lambda },
    { R"(%)", Token::Type::Lambda },
    { ".", Token::Type::Dot },

    { "..", Token::Type::Doot },
    { "..<", Token::Type::DootLt },

    { ":", Token::Type::Colon },
    { "::", Token::Type::Colonn },
    { ":=", Token::Type::ColonEquiv },
    { "=", Token::Type::Equiv },
    { R"(\<longleftrightarrow>)", Token::Type::Equiv },
    { R"(\<equiv>)", Token::Type::Equiv },
    { "|", Token::Type::Pipe },
    { "\"", Token::Type::Quotation },
    { "(", Token::Type::LParen },
    { ")", Token::Type::RParen },
    { R"(\<Rightarrow>)", Token::Type::Rightarrow },
    { R"(=>)", Token::Type::Rightarrow },
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
    { R"(!)", Token::Type::Excl },
    { R"(\<union>)", Token::Type::Union },
    { R"(\<inter>)", Token::Type::Inter },
    { R"(+)", Token::Type::Add },
    { R"(-)", Token::Type::Sub },
    { R"(*)", Token::Type::Mul },
    { R"(\<times>)", Token::Type::Mul },
    { R"(/)", Token::Type::Div },
    { R"(div)", Token::Type::Div },
    { R"(mod)", Token::Type::Mod },
    { R"(^)", Token::Type::Pow },
};

Token::Token(string value) noexcept
  : value(move(value)) {
    auto it = localTokenMapping.find(this->value);
    type = (it == localTokenMapping.end() ? Token::Type::Identifier : it->second);
}

optional<Token::Type> Token::add_token(const string &literal) {
    if (localTokenMapping.count(literal)) {
        return {};
    } else {
        return localTokenMapping[literal] = Token::Type(++localEndOfToken);
    }
}
} // namespace hol2cpp
