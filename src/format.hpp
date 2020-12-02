#pragma once

#include <string>
#include <iostream>

namespace hol2cpp
{
class FormatString
{
  public:
    constexpr FormatString(const char *str, std::size_t size)
      : pattern_(str), size_(size)
    {
        // ...
    }

    template<typename ...Args>
    void outf(std::ostream &out, Args &&...args)
    {
        out << format(std::forward<Args>(args)...);
    }

    template<typename ...Args>
    std::string format(Args &&...args)
    {
        return format_impl(0, std::forward<Args>(args)...);
    }

  private:
    std::string format_impl(std::size_t ind)
    {
        std::string result;
        for (; ind < size_; ++ind)
        {
            result.push_back(pattern_[ind]);
        }
        return result;
    }

    template<typename Arg, typename ...Args>
    std::string format_impl(std::size_t ind, Arg &&arg, Args &&...args)
    {
        std::string result;

        for (; ind < size_; ++ind)
        {
            if (pattern_[ind] == '$')
            {
                if constexpr (std::is_same_v<std::decay_t<Arg>, std::string>)
                {
                    result += arg;
                }
                else
                {
                    result += std::to_string(arg);
                }
                break;
            }
            else
            {
                result.push_back(pattern_[ind]);
            }
        }

        if (ind + 1 < size_)
        {
            return result + format_impl(ind + 1, args...);
        }

        return result;
    }

    const char *pattern_;
    std::size_t size_;
};

constexpr FormatString operator""_fs(const char *str, std::size_t size)
{
    return FormatString(str, size);
}
}
