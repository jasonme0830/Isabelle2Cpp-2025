/**
 * @file codegen.hpp
 * @author Mu00 (mu001999@outlook.com)
 * @brief includes headers that are used in code generating and defines common helpful functions
 * @date 2022-03-16
 */

#pragma once

#include "../ir/ast.hpp"
#include "../ir/code.hpp"
#include "../utility/error.hpp"
#include "../utility/format.hpp"

#define assert_true(expr) assert_impl(expr, CodegenError)

namespace hol2cpp {
extern const std::map<std::string, std::string> theNormalTypeMapping;
extern const std::map<std::string, std::string> theTemplateTypeMapping;

void warning(const std::string &msg);
bool is_unit(const std::string &expr);
std::string enclose_expr(const std::string &expr);
std::string unmove_expr(std::string expr);
} // namespace hol2cpp
