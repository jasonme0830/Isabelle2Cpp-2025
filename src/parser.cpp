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
blanks_(
  (blank_ + blanks_ >>
    [](Placeholder, Placeholder) {
      return std::string(); }) |
  (blank_ >>
    [](Placeholder) {
      return std::string(); })
),

// identifier := alpha identifier | alpha
identifier_(
  (alpha_ + identifier_ >>
    [](char head, std::string tail) {
        return head + tail; }) |
  (alpha_ >>
    [](char alpha) {
      return std::string(0, alpha); })
),

// fun_decl_name := "fun" blanks identifier blanks "::" blanks
fun_decl_name_(
  "fun"_T + blanks_ + identifier_ + blanks_ + "::"_T + blanks_ >>
    [](string, string, string ident, string, string, string) {
      return ident; }
),

// fun_decl_type := '"' type '"' blanks "where" blanks
fun_decl_type_(
  '"'_T + type_ + '"'_T + blanks_ + "where"_T + blanks_ >>
    [](char, unique_ptr<Type> type, char, string, string, string) {
      return type; }
)

{}

unique_ptr<FunDecl>
Parser::pas_fun_decl(const string &str)
{
  return fun_decl_(str);
}
}
