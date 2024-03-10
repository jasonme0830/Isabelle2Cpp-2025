#pragma once

#include <iostream>
#include <string>

namespace hol2cpp {
class FormatString
{
public:
  FormatString(std::string pattern)
    : pattern_(std::move(pattern))
  {
    // ...
  }

  FormatString(const char* str)
    : pattern_(str)
  {
    // ...
  }

  template<typename... Args>
  void outf(std::ostream& out, Args&&... args) const
  {
    out << format(std::forward<Args>(args)...);
  }

  template<typename... Args>
  std::string format(Args&&... args) const
  {
    return format_impl(0, std::forward<Args>(args)...);
  }

private:
  std::string format_impl(std::size_t ind) const
  {
    std::string result;
    for (; ind < pattern_.size(); ++ind) {
      result.push_back(pattern_[ind]);
    }
    return result;
  }

  template<typename Arg, typename... Args>
  std::string format_impl(std::size_t ind, Arg&& arg, Args&&... args) const
  {
    std::string result;

    for (; ind < pattern_.size(); ++ind) {
      if (pattern_[ind] == '$') {
        if constexpr (std::is_arithmetic_v<std::decay_t<Arg>>) {
          result += std::to_string(arg);
        } else {
          result += arg;
        }
        break;
      } else {
        result.push_back(pattern_[ind]);
      }
    }

    if (ind + 1 < pattern_.size()) {
      return result + format_impl(ind + 1, args...);
    }

    return result;
  }

  std::string pattern_;
};

inline FormatString
operator""_fs(const char* str, std::size_t)
{
  return FormatString(str);
}
} // namespace hol2cpp
