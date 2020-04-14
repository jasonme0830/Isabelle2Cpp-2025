#include <cctype>
#include "parser.hpp"

using namespace std;
using namespace parsec;

namespace hol2cpp
{
// the grammer is tentative now
Parser::Parser()
{
digit_ = Token::by(::isdigit);
alpha_ = Token::by(::isalpha);
blank_ = Token::by(::isspace);

// blanks
// : blank blanks
// | <epsilon>
blanks_ =
  blank_ + blanks_ >>
    [](char, char) -> char
    {
        return 0;
    } |
  Token::epsilon<char>();

// identifier
// : (alpha | digit) identifier
// | (alpha | digit)
identifier_ =
  (alpha_ | digit_) + identifier_ >>
    [](char head, string tail)
    {
        return head + tail;
    } |
  (alpha_ | digit_) >>
    [](char ch)
    {
        return string(1, ch);
    };

// func_decl_name
// : "fun" blanks identifier blanks "::"
func_decl_name_ =
  "fun"_T + blanks_ + identifier_ + blanks_ + "::"_T >>
    [](string, char, string ident, char, string)
    {
        return ident;
    };

// type_term
// : identifier blanks identifier
// | identifier
// | "'" identifier blanks identifier
// | "'" identifier
// | '(' blanks type blanks ')' blanks identifier
// | '(' blanks type blanks ')'
type_term_ =
  identifier_ + blanks_ + identifier_ >>
    [](string type1, char, string type2) -> Ptr<Type>
    {
        return make_unique<TemplateType>(type2, make_unique<NormalType>(type1));
    } |
  identifier_ >>
    [](string type) -> Ptr<Type>
    {
        return make_unique<NormalType>(type);
    } |
  '\''_T + identifier_ + blanks_ + identifier_ >>
    [](char, string type1, char, string type2) -> Ptr<Type>
    {
        return make_unique<TemplateType>(type2, make_unique<ArgumentType>(type1));
    } |
  '\''_T + identifier_ >>
    [](char, string type) -> Ptr<Type>
    {
        return make_unique<ArgumentType>(type);
    } |
  '('_T + blanks_ + type_ + blanks_ + ')'_T + identifier_ >>
    [](char, char, Ptr<Type> &&type1, char, char, string type2) -> Ptr<Type>
    {
        return make_unique<TemplateType>(type2, move(type1));
    } |
  '('_T + blanks_ + type_ + blanks_ + ')'_T >>
    [](char, char, Ptr<Type> type, char, char)
    {
        return type;
    };

// func_type_tail
// : "\<Rightarrow>" blanks type_term blanks func_type_tail
// | <eplison>
func_type_tail_ =
  R"(\<Rightarrow>)"_T + blanks_ + type_term_ + blanks_ + func_type_tail_ >>
    [](string, char, Ptr<Type> &&type, char, vector<Ptr<Type>> types)
    {
        types.insert(types.begin(), move(type));
        return types;
    } |
  Token::epsilon<vector<Ptr<Type>>>();

// func_type
// : type_term blanks "\<Rightarrow>" blanks type_term blanks func_type_tail
func_type_ =
  type_term_ + blanks_ + R"(\<Rightarrow>)"_T + blanks_ + type_term_ + blanks_ + func_type_tail_ >>
    [](Ptr<Type> &&type1, char, string, char, Ptr<Type> &&type2, char, vector<Ptr<Type>> &&types)
    {
        types.insert(types.begin(), move(type2));
        types.insert(types.begin(), move(type1));
        return make_unique<FuncType>(move(types));
    };

// type
// : func_type
// | type_term
type_ =
  func_type_ >>
    [](Ptr<FuncType> type) -> Ptr<Type>
    {
        return type;
    } |
  type_term_;

// func_decl_type
// : '"' blanks func_type blanks '"' blanks "where"
func_decl_type_ =
  '"'_T + blanks_ + func_type_ + blanks_ + '"'_T + blanks_ + "where"_T >>
    [](char, char, Ptr<FuncType> type, char, char, char, string)
    {
        return type;
    };

// miniterm
// : identifier
// : '(' blanks expr blanks ')'
miniterm_ =
  identifier_ >>
    [](string ident) -> Ptr<Expr>
    {
        return make_unique<VarExpr>(move(ident));
    } |
  '('_T + blanks_ + expr_ + blanks_ + ')'_T >>
    [](char, char, Ptr<Expr> expr, char, char)
    {
        return expr;
    };

// miniterms
// : miniterm blanks miniterms
// | <epsilon>
miniterms_ =
  miniterm_ + blanks_ + miniterms_ >>
    [](Ptr<Expr> &&miniterm, char, vector<Ptr<Expr>> miniterms)
    {
        miniterms.insert(miniterms.begin(), move(miniterm));
        return miniterms;
    } |
  Token::epsilon<vector<Ptr<Expr>>>();

// cons_term
// : identifier blanks miniterm blanks miniterms
cons_term_ =
  identifier_ + blanks_ + miniterm_ + blanks_ + miniterms_ >>
    [](string name, char, Ptr<Expr> &&miniterm, char, vector<Ptr<Expr>> &&miniterms)
    {
        miniterms.insert(miniterms.begin(), move(miniterm));
        return make_unique<ConsExpr>(move(name), move(miniterms));
    };

// var_term
// : identifier
var_term_ =
  identifier_ >>
    [](string name)
    {
        return make_unique<VarExpr>(move(name));
    };

// term
// : cons_term
// | var_term
term_ =
  cons_term_ >>
    [](Ptr<Expr> expr)
    {
        return expr;
    } |
  var_term_ >>
    [](Ptr<Expr> expr)
    {
        return expr;
    };

// expr9
// : term blanks '^' blanks expr9
// | term
expr9_ =
  term_ + blanks_ + '^'_T + blanks_ + expr9_ >>
    [](Ptr<Expr> &&term, char, char, char, Ptr<Expr> &&expr)
    {
        return make_unique<BinaryOpExpr>(BOp::NumPow, move(term), move(expr));
    } |
  term_;

// expr8_tail
// : '*' blanks expr9 blanks expr8_tail
// | '/' blanks expr9 blanks expr8_tail
// | "div" blanks expr9 blanks expr8_tail
// | "mod" blanks expr9 blanks expr8_tail
// | <epsilon>
expr8_tail_ =
  '*'_T + blanks_ + expr9_ + blanks_ + expr8_tail_ >>
    [](char, char, Ptr<Expr> &&expr, char, Ptr<BinaryOpTailExpr> &&tail)
    {
        return make_unique<BinaryOpTailExpr>(BOp::NumMul,
            move(expr), move(tail));
    } |
  ('/'_T | "div"_T) + blanks_ + expr9_ + blanks_ + expr8_tail_ >>
    [](Placeholder, char, Ptr<Expr> &&expr, char, Ptr<BinaryOpTailExpr> &&tail)
    {
        return make_unique<BinaryOpTailExpr>(BOp::NumDiv,
            move(expr), move(tail));
    } |
  "mod"_T + blanks_ + expr9_ + blanks_ + expr8_tail_ >>
    [](string, char, Ptr<Expr> &&expr, char, Ptr<BinaryOpTailExpr> &&tail)
    {
        return make_unique<BinaryOpTailExpr>(BOp::NumMod,
            move(expr), move(tail));
    } |
  Token::epsilon([]() -> Ptr<BinaryOpTailExpr> { return nullptr; });

// expr8
// : expr9 blanks expr8_tail
expr8_ =
  expr9_ + blanks_ + expr8_tail_ >>
  [](Ptr<Expr> expr, char, Ptr<BinaryOpTailExpr> tail)
    -> Ptr<Expr>
  {
      while (tail)
      {
          expr = make_unique<BinaryOpExpr>(tail->op,
              move(expr), move(tail->expr));
          tail = move(tail->tail);
      }
      return expr;
  };

// expr7_tail
// : '+' blanks expr8 blanks expr7_tail
// | '-' blanks expr8 blanks expr7_tail
// | <epsilon>

// expr7
// : expr8 blanks expr7_tail

// expr6_tail
// : "\<inter>" blanks expr7 blanks expr6_tail
// | <epsilon>

// expr6
// : expr7 blanks expr6_tail

// expr5_tail
// : "\<union>" blanks expr6 blanks expr5_tail
// | <epsilon>

// expr5
// : expr6 blanks '#' blanks expr5
// | expr6 blanks '@' blanks expr5
// | expr6 blanks expr5_tail

// expr4
// : expr5 blanks "\<subseteq>" blanks expr4
// | expr5 blanks "\<subset>" blanks expr4
// | expr5 blanks "\<supseteq>" blanks expr4
// | expr5 blanks "\<supset>" blanks expr4
// | expr5 blanks "\<in>" blanks expr4
// | expr5 blanks "\<notin>" blanks expr4

// expr3
// : expr4 blanks "\<le>" blanks expr3
// | expr4 blanks '<' blanks expr3
// | expr4 blanks "\<ge>" blanks expr3
// | expr4 blanks '>' blanks expr3

// expr2_tail
// : '=' blanks expr3 blanks expr2_tail
// | "\<noteq>" blanks expr3 blanks expr2_tail
// | <epsilon>

// expr2
// : expr3 blanks expr2_tail

// expr1
// : expr2 blanks "\<and>" blanks expr1
// | expr2

// expr
// : expr1 blanks "\<or>" blanks expr
// | expr1
expr_ = term_;

// func_decl_equation
// : '"' blanks cons_term blanks '=' blanks expr blanks '"'
func_decl_equation_ =
  '"'_T + blanks_ + cons_term_ + blanks_ + '='_T + blanks_ + expr_ + blanks_ + '"'_T >>
    [](char, char, Ptr<ConsExpr> &&cons, char, char, char, Ptr<Expr> &&expr, char, char)
    {
        return make_unique<Equation>(move(cons), move(expr));
    };

// func_decl_equations
// : func_decl_equation blanks '|' blanks func_decl_equations
// | func_decl_equation
func_decl_equations_ =
  func_decl_equation_ + blanks_ + '|'_T + blanks_ + func_decl_equations_ >>
    [](Ptr<Equation> &&equation, char, char, char, vector<Ptr<Equation>> &&equations)
    {
        equations.insert(equations.begin(), move(equation));
        return equations;
    } |
  func_decl_equation_ >>
    [](Ptr<Equation> &&equation)
    {
        vector<Ptr<Equation>> equations;
        equations.push_back(move(equation));
        return equations;
    };

// func_decl
// : func_decl_name blanks func_decl_type blanks func_decl_equations
func_decl_ =
  func_decl_name_ + blanks_ + func_decl_type_ + blanks_ + func_decl_equations_ >>
    [](string name, char, Ptr<FuncType> &&type, char, vector<Ptr<Equation>> &&equations)
    {
        return make_unique<FuncDecl>(move(name), move(type), move(equations));
    };

// func_decls
// : blanks func_decl blanks func_decls
// | <epsilon>
func_decls_ =
  blanks_ + func_decl_ + blanks_ + func_decls_ >>
    [](char, Ptr<FuncDecl> &&decl, char, vector<Ptr<FuncDecl>> &&decls)
    {
        decls.insert(decls.begin(), move(decl));
        return decls;
    } |
  Token::epsilon<vector<Ptr<FuncDecl>>>();

}

vector<Ptr<FuncDecl>>
Parser::pas_func_decls(const string &str)
{
    return func_decls_(str);
}

Ptr<FuncDecl>
Parser::pas_func_decl(const string &str)
{
    return func_decl_(str);
}
}
