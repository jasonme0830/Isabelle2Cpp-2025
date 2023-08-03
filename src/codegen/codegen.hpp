/**
 * @file codegen.hpp
 * @author Mu00 (mu001999@outlook.com)
 * @brief includes headers that are used in code generating and defines common
 * helpful functions
 * @date 2022-03-16
 */

#pragma once

#include "../ir/ast.hpp"
#include "../ir/code.hpp"
#include "../utility/error.hpp"
#include "../utility/format.hpp"

#include <functional>
#include <string>
#include <vector>

#define assert_true(expr) assert_impl(expr, CodegenError)

namespace hol2cpp {
inline std::set<std::string> thePredefinedDatatypes;
inline std::set<std::string> thePredefinedFunctions;
/*
  These variable is code by myk.
  To store the definitions for the comparation of the isomorphism definitions.
*/
inline std::vector<DatatypeDef> theDefinedDatatypes;
inline std::set<Ptr<FunctionDef>> theDefinedFunctions;
/*
  These variable is code by myk.
  To store the declaritions for the comparation of the same name declararions.
  Have the same use with the variable thePredefinedDatatypes.
  Xubo's variable have some name ambiguity.
*/
// inline std::set<std::string> theDeclaredDatatypes;
// inline std::set<std::string> theDeclaredFunctions;

extern std::map<std::string, std::string> theNormalTypeMapping;
extern std::map<std::string, std::string> theTemplateTypeMapping;
extern std::map<std::string, std::string> theHeaderMapping;

void
warning(const std::string& msg);
bool
is_unit(const std::string& expr);

template<char LParen = '(', char RParen = ')'>
inline std::string
enclose(const std::string& expr)
{
  return LParen + expr + RParen;
}
template<char LParen = '(', char RParen = ')'>
std::string
enclose_if_needed(const std::string& expr)
{
  if (is_unit(expr)) {
    return expr;
  } else {
    return enclose<LParen, RParen>(expr);
  }
}

std::string
unmove_expr(std::string expr);

template<typename T, typename ToStr = std::function<std::string(const T&)>>
inline std::string
join(const std::vector<T>& vec,
     const ToStr& to_str,
     const std::string& sep = ", ")
{
  std::string res;
  for (const auto& item : vec) {
    if (!res.empty()) {
      res += sep;
    }
    res += to_str(item);
  }
  return res;
}
} // namespace hol2cpp
