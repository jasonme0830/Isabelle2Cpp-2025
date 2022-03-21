#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
bool is_unit(const string &expr) {
    if (expr.find(' ') == expr.npos) {
        return true;
    }

    int cnt = 0;
    for (auto chr : expr) {
        if (chr == '(' || chr == '[' || chr == '{') {
            ++cnt;
        } else if (chr == ')' || chr == ']' || chr == '}') {
            --cnt;
        } else if (cnt == 0 && chr == ' ') {
            return false;
        }
    }

    return true;
}

string enclose_expr(const string &expr) {
    if (is_unit(expr)) {
        return expr;
    } else {
        return '(' + expr + ')';
    }
}

string unmove_expr(string expr) {
    if (expr.substr(0, 9) == "std::move") {
        return expr.substr(10, expr.size() - 11);
    } else {
        return expr;
    }
}
} // namespace hol2cpp
