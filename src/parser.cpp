#include <cctype>
#include "parser.hpp"

using namespace std;
using namespace parsec;

namespace hol2cpp {
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
    [](Placeholder, Placeholder) -> char {
      return 0;
    } |
  Token::epsilon<char>()
),

// identifier
// : alpha identifier
// | <epsilon>
identifier_(
  alpha_ + identifier_ >>
    [](char, char head, string tail) {
      return head + tail;
    } |
  Token::epsilon<string>()
),

// fun_decl_name
// : "fun" blanks identifier blanks "::"
fun_decl_name_(
  "fun"_T + blanks_ + identifier_ + blanks_ + "::"_T>>
    [](string, char, string ident, char, string) {
      return ident;
    }
),

// fun_decl_type
// : '"' type '"' blanks "where"
fun_decl_type_(
  '"'_T + type_ + '"'_T + blanks_ + "where"_T >>
    [](char, unique_ptr<Type> type, char, char, string) {
      return type;
    }
),

// miniterm
// : identifier
// : '(' blanks expr blanks ')'
miniterm_(
  identifier_ >>
    [](string ident) -> unique_ptr<Expr> {
      return make_unique<VarExpr>(move(ident));
    } |
  '('_T + blanks_ + expr_ + blanks_ + ')'_T >>
    [](char, char, unique_ptr<Expr> expr, char, char) {
      return expr;
    }
),

// miniterms
// : miniterm blanks miniterms
// | <epsilon>
miniterms_(
  miniterm_ + blanks_ + miniterms_ >>
    [](unique_ptr<Expr> miniterm, char, vector<unique_ptr<Expr>> miniterms) {
      miniterms.emplace(miniterms.begin(), move(miniterm));
      return miniterms;
    } |
  Token::epsilon<vector<unique_ptr<Expr>>>()
),

// cons_term
// : identifier blanks miniterm blanks miniterms
cons_term_(
  identifier_ + blanks_ + miniterm_ + blanks_ + miniterms_ >>
    [](string name, char, unique_ptr<Expr> miniterm, char, vector<unique_ptr<Expr>> miniterms) {
      miniterms.emplace(miniterms.begin(), move(miniterm));
      return make_unique<ConsExpr>(move(name), move(miniterms));
    }
),

// var_term
// : identifier
var_term_(
  identifier_ >>
    [](string name) {
      return make_unique<VarExpr>(move(name));
    }
),

// term
// : cons_term
// | var_term
term_(
  cons_term_ >>
    [](unique_ptr<Expr> expr) {
      return expr;
    } |
  var_term_ >>
    [](unique_ptr<Expr> expr) {
      return expr;
    }
),

// fun_decl_equation
// : '"' blanks cons_term blanks '=' blanks expr blanks '"'
fun_decl_equation_(
  '"'_T + blanks_ + cons_term_ + blanks_ + '='_T + blanks_ + expr_ + blanks_ + '"'_T >>
    [](char, char, unique_ptr<ConsExpr> cons, char, char, string, unique_ptr<Expr> expr, char, char) {
      return make_unique<Equation>(move(cons), move(expr));
    }
),

// fun_decl_equations
// : fun_decl_equation blanks '|' blanks fun_decl_equations
// | fun_decl_equation
fun_decl_equations_(
  fun_decl_equation_ + blanks_ + '|'_T + blanks_ + fun_decl_equations_ >>
    [](unique_ptr<Equation> equation, char, char, char, vector<unique_ptr<Equation>> equations) {
      equations.emplace(equations.begin(), move(equation));
      return equations;
    } |
  fun_decl_equation_ >>
    [](unique_ptr<Equation> equation) {
      vector<unique_ptr<Equation>> equations;
      equations.emplace_back(move(equation));
      return equations;
    }
),

// fun_decl
// : fun_decl_name blanks fun_decl_type blanks fun_decl_equations
fun_decl_(
  fun_decl_name_ + blanks_ + fun_decl_type_ + blanks_ + fun_decl_equations_ >>
    [](string name, char, unique_ptr<Type> type, char, vector<unique_ptr<Equation>> equations) {
      return make_unique<FunDecl>();
    }
),

// fun_decls
// : fun_decl blanks fun_decls
// | <epsilon>
fun_decls_(
  fun_decl_ + blanks_ + fun_decls_ >>
    [](unique_ptr<FunDecl> decl, char, vector<unique_ptr<FunDecl>> decls) {
      decls.emplace(decls.begin(), move(decl));
      return decls;
    } |
  Token::epsilon<vector<unique_ptr<FunDecl>>>()
)

{}

vector<unique_ptr<FunDecl>>
Parser::pas_fun_decls(const string &str) {
  return fun_decls_(str);
}

unique_ptr<FunDecl>
Parser::pas_fun_decl(const string &str) {
  return fun_decl_(str);
}
}
