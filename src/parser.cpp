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

// blanks
// : blank blanks
// | <epsilon>
blanks_(
  blank_ + blanks_ >>
    [](Placeholder, Placeholder) {
      return string(); } |
  Token::epsilon<string>()
),

// identifier
// : alpha identifier
// | <epsilon>
identifier_(
  alpha_ + identifier_ >>
    [](char head, string tail) {
      return head + tail; } |
  Token::epsilon<string>()
),

// fun_decl_name
// : "fun" blanks identifier blanks "::" blanks
fun_decl_name_(
  "fun"_T + blanks_ + identifier_ + blanks_ + "::"_T + blanks_ >>
    [](string, string, string ident, string, string, string) {
      return ident; }
),

// fun_decl_type
// : '"' type '"' blanks "where" blanks
fun_decl_type_(
  '"'_T + type_ + '"'_T + blanks_ + "where"_T + blanks_ >>
    [](char, unique_ptr<Type> type, char, string, string, string) {
      return type; }
),

// fun_decl_patterns
// : fun_decl_pattern '|' fun_decl_patterns
// | fun_decl_pattern
fun_decl_patterns_(
  fun_decl_pattern_ + '|'_T + fun_decl_patterns_ >>
    [](unique_ptr<Pattern> pattern, char, vector<unique_ptr<Pattern>> patterns) {
      patterns.emplace(patterns.begin(), move(pattern));
      return patterns; } |
  fun_decl_pattern_ >>
    [](unique_ptr<Pattern> pattern) {
      vector<unique_ptr<Pattern>> patterns;
      patterns.emplace_back(move(pattern));
      return patterns; }
),

// fun_decl
// : fun_decl_name fun_decl_type fun_decl_patterns
fun_decl_(
  fun_decl_name_ + fun_decl_type_ + fun_decl_patterns_ >>
    [](string name, unique_ptr<Type> type, vector<unique_ptr<Pattern>> patterns) {
      return make_unique<FunDecl>(); }
),

// fun_decls
// : fun_decl fun_decls
// | <epsilon>
fun_decls_(
  fun_decl_ + fun_decls_ >>
    [](unique_ptr<FunDecl> decl, vector<unique_ptr<FunDecl>> decls) {
      decls.emplace(decls.begin(), move(decl));
      return decls; } |
  Token::epsilon<vector<unique_ptr<FunDecl>>>()
)

{}

vector<unique_ptr<FunDecl>>
Parser::pas_fun_decls(const string &str)
{
  return fun_decls_(str);
}

unique_ptr<FunDecl>
Parser::pas_fun_decl(const string &str)
{
  return fun_decl_(str);
}
}
