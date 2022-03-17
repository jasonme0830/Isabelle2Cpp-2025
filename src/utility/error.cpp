#include "error.hpp"

using namespace std;

namespace hol2cpp {
namespace info {
string strong(const string &str) {
    return "\033[1m" + str + "\033[0m";
}

string red(const string &str) {
    return "\033[31m" + str + "\033[0m";
}

string blue(const string &str) {
    return "\033[34m" + str + "\033[0m";
}

string green(const string &str) {
    return "\033[32m" + str + "\033[0m";
}

string light_red(const string &str) {
    return "\033[1;31m" + str + "\033[0m";
}

string light_blue(const string &str) {
    return "\033[1;34m" + str + "\033[0m";
}

string light_green(const string &str) {
    return "\033[1;32m" + str + "\033[0m";
}
} // namespace info

TokenizeError::TokenizeError(string err) : err_(std::move(err)) {}
TokenizeError &TokenizeError::operator=(const TokenizeError &other) noexcept {
    err_ = other.err_;
    return *this;
}

const char *TokenizeError::what() const noexcept {
    return err_.c_str();
}

ParseError::ParseError(string err) : err_(std::move(err)) {}
ParseError::ParseError(const ParseError &other, PEType type) : err_(other.err_), type_(type) {}
ParseError &ParseError::operator=(const ParseError &other) noexcept {
    err_ = other.err_;
    return *this;
}

const char *ParseError::what() const noexcept {
    return err_.c_str();
}

CodegenError::CodegenError(string err) : err_(std::move(err)) {}
CodegenError &CodegenError::operator=(const CodegenError &other) noexcept {
    err_ = other.err_;
    return *this;
}

const char *CodegenError::what() const noexcept {
    return err_.c_str();
}
} // namespace hol2cpp
