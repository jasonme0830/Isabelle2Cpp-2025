#pragma once

#include <string>
#include <exception>

#define ERROR 0
#define assertt(expr) if (!(expr)) { throw ERROR(info::strong("$:$: "_fs.format(__FILE__, __LINE__)) + "Assertion `" #expr "' failed."); }

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

class ParseError : public std::exception {
  public:
    ParseError(std::string err);
    ParseError &operator=(const ParseError &other) noexcept;

    const char *what() const noexcept override;

  private:
    std::string err_;
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
