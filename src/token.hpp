#pragma once

#include <string>

namespace hol2cpp
{
struct Token
{
    enum class Type
    {
        TTif,
        TTthen,
        TTelse,

        TTidentifier
    };

    Token(Type type, std::string value = "")
      : type(type), value(std::move(value)) {}

    const Type type;
    const std::string value;

    static Token
    gen_token_by_value(const std::string &value);
};
}
