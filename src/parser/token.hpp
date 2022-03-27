#pragma once

#include <string>
#include <utility>
#include <optional>

namespace hol2cpp {
struct Token {
  public:
    enum class Type : std::size_t {
        SkipKeyword,    // such as lemma `

        Comment,        // \<comment>
        CommentStart,   // (*
        CommentEnd,     // *)
        // pass now
        Open,           // \<open>
        Close,          // \<close>

        Theory,         // theory
        Imports,        // imports
        Begin,          // begin

        Datatype,       // datatype

        Function,       // fun function primrec definition abbreviation
        Where,          // where

        Integer,        // [0-9]+
        Identifier,     // [_a-zA-Z]+
        TypeVariable,   // '[_a-zA-Z]+

        Infixl,         // infixl
        Infixr,         // infixr

        If,             // if
        Else,           // else
        Then,           // then

        Let,            // let
        KeyIn,          // in

        Case,           // case
        Of,             // of

        Lambda,         // lambda %
        Dot,            // .

        Doot,           // ..
        DootLt,         // ..<

        Colon,          // :
        Colonn,         // ::
        ColonEquiv,     // :=
        Equiv,          // = \<longleftrightarrow> \<equiv>
        Pipe,           // |
        Quotation,      // "
        LParen,         // (
        RParen,         // )
        Rightarrow,     // \<Rightarrow> =>
        LBracket,       // [
        RBracket,       // ]
        LBrace,         // {
        RBrace,         // }
        Comma,          // ,
        Or,             // \<or>
        And,            // \<and>
        NotEq,          // \<noteq>
        Le,             // \<le>
        Lt,             // <
        Ge,             // \<ge>
        Gt,             // >
        SubsetEq,       // \<subseteq>
        Subset,         // \<subset>
        SupsetEq,       // \<supseteq>
        Supset,         // \<supset>
        In,             // \<in>
        NotIn,          // \<notin>
        Sharp,          // #
        At,             // @
        Excl,           // !
        Union,          // \<union>
        Inter,          // \<inter>
        Add,            // +
        Sub,            // -
        Mul,            // * \<times>
        Div,            // / or div
        Mod,            // mod
        Pow,            // ^

        EndOfFile       // new symbol will be greater than EndOfFile
    };

    Type type;
    std::string value;

  public:
    static std::optional<Type> add_token(const std::string &value);

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
