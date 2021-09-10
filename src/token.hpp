#pragma once

#include <string>
#include <utility>

namespace hol2cpp {
struct Token {
  public:
    enum class Type : std::size_t {
        Theory,         // theory
        Imports,        // imports
        Begin,          // begin
        End,            // end

        Datatype,       // datatype

        Fun,            // fun
        Function,       // function
        Primrec,        // primrec
        Where,          // where

        Integer,        // [0-9]+
        Identifier,     // [_a-zA-Z]+
        TypeVariable,   // '[_a-zA-Z]+

        If,             // if
        Else,           // else
        Then,           // then

        Colon,          // :
        Colonn,         // ::
        ColonEquiv,     // :=
        Equiv,          // =
        Pipe,           // |
        Quotation,      // "
        LParen,         // (
        RParen,         // )
        Star,           // *
        Rightarrow,     // \<Rightarrow>
        LBracket,       // [
        RBracket,       // ]
        LBrace,         // {
        RBrace,         // }
        Comma,          // ,

        EndOfFile       // new symbol will be greater than EndOfFile
    };

    Type type;
    std::string value;

  public:
    Token(Type type) noexcept;
    Token(Type type, std::string value) noexcept;
    Token(std::string value) noexcept;

    Token(Token &&) noexcept;
    Token(const Token &) noexcept;

    Token &operator=(Token &&) noexcept;
    Token &operator=(const Token &) noexcept;
};
} // namespace hol2cpp
