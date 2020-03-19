#include <cctype>
#include "parser.hpp"

using namespace std;
using namespace parsec;

namespace hol2cpp
{
// the grammer is tentative now
Parser::Parser()
  : digit_(Token::by(::isdigit)),
    alpha_(Token::by(::isalpha)),
    blank_(Token::by(::isblank)),

    // blanks := blank blanks | blank
    blanks_((blank_ + blanks_ >> [](char blk, std::string blks) {
        return std::string();
    }) | (blank_ >> [](char blk) { return std::string(); })),

    // identifier := alpha identifier | alpha
    identifier_((alpha_ + identifier_ >> [](char head, std::string tail) {
        return head + tail;
    }) | (alpha_) >> [](char alpha) { return std::string(0, alpha); })
{

}

unique_ptr<FunDecl>
Parser::pas_fun_decl()
{

}
}
