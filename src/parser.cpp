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

// func_decl_name
// : "fun" blanks identifier blanks "::"
func_decl_name_(
  "fun"_T + blanks_ + identifier_ + blanks_ + "::"_T>>
    [](string, char, string ident, char, string) {
      return ident;
    }
),

// type_term
// : identifier blanks identifier
// | identifier
// | "'" identifier blanks identifier
// | "'" identifier
// | '(' blanks type blanks ')' blanks identifier
// | '(' blanks type blanks ')'
type_term_(
  identifier_ + blanks_ + identifier_ >>
    [](string type1, char, string type2) -> unique_ptr<Type> {
      return make_unique<TemplateType>(type2, make_unique<NormalType>(type1));
    } |
  identifier_ >>
    [](string type) -> unique_ptr<Type> {
      return make_unique<NormalType>(type);
    } |
  '\''_T + identifier_ + blanks_ + identifier_ >>
    [](char, string type1, char, string type2) -> unique_ptr<Type> {
      return make_unique<TemplateType>(type2, make_unique<ArgumentType>(type1));
    } |
  '\''_T + identifier_ >>
    [](char, string type) -> unique_ptr<Type> {
      return make_unique<ArgumentType>(type);
    } |
  '('_T + blanks_ + type_ + blanks_ + ')'_T + identifier_ >>
    [](char, char, unique_ptr<Type> type1, char, char, string type2) -> unique_ptr<Type> {
      return make_unique<TemplateType>(type2, type1);
    } |
  '('_T + blanks_ + type_ + blanks_ + ')'_T >>
    [](char, char, unique_ptr<Type> type, char, char) {
      return type;
    }
),

// func_type_tail
// : "\<Rightarrow>" blanks type_term blanks func_type_tail
// | <eplison>
func_type_tail_(
  R"(\<Rightarrow>)"_T + blanks_ + type_term_ + blanks_ + func_type_tail_ >>
    [](string, char, unique_ptr<Type> type, char, vector<unique_ptr<Type>> types) {
      types.emplace(types.begin(), move(type));
      return types;
    } |
  Token::epsilon<vector<unique_ptr<Type>>>()
),

// func_type
// : type_term blanks "\<Rightarrow>" blanks type_term blanks func_type_tail
func_type_(
  type_term_ + blanks_ + R"(\<Rightarrow>)"_T + blanks_ + type_term_ + blanks_ + func_type_tail_ >>
    [](unique_ptr<Type> type1, char, string, char, unique_ptr<Type> type2, char, vector<unique_ptr<Type>> types) {
      types.emplace(types.begin(), move(type2));
      types.emplace(types.begin(), move(type1));
      return make_unique<FuncType>(move(types));
    }
),

// type
// : func_type
// | type_term
type_(
  func_type_ >>
    [](unique_ptr<FuncType> type) -> unique_ptr<Type> {
      return type;
    } |
  type_term_
),

// func_decl_type
// : '"' func_type '"' blanks "where"
func_decl_type_(
  '"'_T + func_type_ + '"'_T + blanks_ + "where"_T >>
    [](char, unique_ptr<FuncType> type, char, char, string) {
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

// expr
// : term
expr_(
  term_
),

// func_decl_equation
// : '"' blanks cons_term blanks '=' blanks expr blanks '"'
func_decl_equation_(
  '"'_T + blanks_ + cons_term_ + blanks_ + '='_T + blanks_ + expr_ + blanks_ + '"'_T >>
    [](char, char, unique_ptr<ConsExpr> cons, char, char, string, unique_ptr<Expr> expr, char, char) {
      return make_unique<Equation>(move(cons), move(expr));
    }
),

// func_decl_equations
// : func_decl_equation blanks '|' blanks func_decl_equations
// | func_decl_equation
func_decl_equations_(
  func_decl_equation_ + blanks_ + '|'_T + blanks_ + func_decl_equations_ >>
    [](unique_ptr<Equation> equation, char, char, char, vector<unique_ptr<Equation>> equations) {
      equations.emplace(equations.begin(), move(equation));
      return equations;
    } |
  func_decl_equation_ >>
    [](unique_ptr<Equation> equation) {
      vector<unique_ptr<Equation>> equations;
      equations.emplace_back(move(equation));
      return equations;
    }
),

// func_decl
// : func_decl_name blanks func_decl_type blanks func_decl_equations
func_decl_(
  func_decl_name_ + blanks_ + func_decl_type_ + blanks_ + func_decl_equations_ >>
    [](string name, char, unique_ptr<FuncType> type, char, vector<unique_ptr<Equation>> equations) {
      return make_unique<FuncDecl>(move(type), move(equations));
    }
),

// func_decls
// : func_decl blanks func_decls
// | <epsilon>
func_decls_(
  func_decl_ + blanks_ + func_decls_ >>
    [](unique_ptr<FuncDecl> decl, char, vector<unique_ptr<FuncDecl>> decls) {
      decls.emplace(decls.begin(), move(decl));
      return decls;
    } |
  Token::epsilon<vector<unique_ptr<FuncDecl>>>()
)

{}

vector<unique_ptr<FuncDecl>>
Parser::pas_func_decls(const string &str) {
  return func_decls_(str);
}

unique_ptr<FuncDecl>
Parser::pas_func_decl(const string &str) {
  return func_decl_(str);
}
}
