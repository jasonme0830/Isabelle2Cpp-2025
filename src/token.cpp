#include "token.hpp"

using namespace std;

namespace
{
static constexpr size_t
operator""_hash(const char *str, size_t len)
{
    return *str ? (*str * 137 + operator""_hash(str + 1, len - 1)) % 57731751ULL : 0;
}
}

namespace hol2cpp
{
Token Token::gen_token_by_value(const string &value)
{
    switch (operator""_hash(value.c_str(), value.size()))
    {
    case "if"_hash:
        return Token(Type::TTif);
    case "then"_hash:
        return Token(Type::TTthen);
    case "else"_hash:
        return Token(Type::TTelse);
    default:
        return Token(Type::TTidentifier, value);
    }
}
}
