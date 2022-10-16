#pragma once

#include <string>
#include <exception>

#define assert_impl(expr, E) if (!(expr)) { throw E(info::strong("$:$: "_fs.format(__FILE__, __LINE__)) + "Assertion `" #expr "' failed."); }

namespace hol2cpp {
namespace info {
std::string strong(const std::string &str);

std::string red(const std::string &str);
std::string blue(const std::string &str);
std::string green(const std::string &str);

std::string light_red(const std::string &str);
std::string light_blue(const std::string &str);
std::string light_green(const std::string &str);
} // namespace;

class TokenizeError : public std::exception {
  public:
    TokenizeError(std::string err);
    TokenizeError &operator=(const TokenizeError &other) noexcept;

    const char *what() const noexcept override;

  private:
    std::string err_;
};

enum class PEType {
    Datatype, Function,
};

class ParseError : public std::exception {
  public:
    ParseError(std::string err);
    ParseError(const ParseError &other, PEType type);
    ParseError &operator=(const ParseError &other) noexcept;

    const char *what() const noexcept override;

    PEType error_type() const { return type_; }

  private:
    std::string err_;
    PEType type_;
};

class CodegenError : public std::exception {
  public:
    CodegenError(std::string err);
    CodegenError &operator=(const CodegenError &other) noexcept;

    const char *what() const noexcept override;

  private:
    std::string err_;
};
} // namespace hol2cpp
