#include <cctype>
#include "parser.hpp"

using namespace std;
using namespace parsec;

namespace hol2cpp
{
// the grammer is tentative now
Parser::Parser() :

digit_(Token::by(::isdigit)),
alpha_(Token::by(::isalpha)),
blank_(Token::by(::isblank)),

// blanks := blank blanks | blank
blanks_((blank_ + blanks_ >> [](char blk, std::string blks) {
    return std::string();
}) | (blank_ >> [](char blk) { return std::string(); })),

// identifier := alpha identifier | alpha
identifier_((alpha_ + identifier_ >> [](char head, std::string tail) {
    return head + tail;
}) | (alpha_) >> [](char alpha) { return std::string(0, alpha); }),

// fun_decl_pre := "fun" blanks identifier blanks "::" blanks
fun_decl_pre_("fun"_T + blanks_ + identifier_ + blanks_ + "::"_T + blanks_ >>
    [](string, string, string ident, string, string, string) { return ident; })

{}

unique_ptr<FunDecl>
Parser::pas_fun_decl(const string &str)
{
    return fun_decl_(str);
}
}
