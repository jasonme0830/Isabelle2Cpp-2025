#pragma once

#include <any>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace hol2cpp {
class Argument
{
public:
  using ActionType = std::function<std::any(const std::string&)>;

  Argument()
    : action_([](const std::string& value) { return value; })
  {
  }

  Argument& help(std::string info)
  {
    help_info_ = std::move(info);
    return *this;
  }

  template<typename T>
  Argument& default_value(const T&& value)
  {
    value_ = value;
    return *this;
  }

  template<typename T>
  Argument& implict_value(const T&& value)
  {
    implict_value_ = value;
    return *this;
  }

  Argument& action(ActionType action)
  {
    action_ = std::move(action);
    return *this;
  }

  void consume(const std::string& value) { value_ = action_(value); }

  void set_implict_value() { value_ = implict_value_; }

  template<typename T = std::string>
  T get()
  {
    return std::any_cast<T>(value_);
  }

  const std::string& help_info() const { return help_info_; }

private:
  std::any value_;
  std::any implict_value_;
  std::string help_info_;
  ActionType action_;
};

class ArgumentParser
{
public:
  ArgumentParser() = default;
  ArgumentParser(std::string program)
    : program_(std::move(program))
  {
  }

  void parse(int argc, char* argv[])
  {
    if (program_.empty()) {
      program_ = argv[0];
    }

    int i = 1;
    auto pit = positional_arguments_.begin();
    while (i < argc) {
      std::string value = argv[i++];
      if (value[0] != '-') {
        arguments_[pit++->first].consume(value);
      } else if (i < argc and argv[i][0] != '-') {
        arguments_[keys_[get_main(value)]].consume(argv[i++]);
      } else {
        arguments_[keys_[get_main(value)]].set_implict_value();
      }
    }
  }

  void print_help(std::ostream& out = std::cout)
  {
    out << "Usage: " << program_ << " [options]";

    for (auto& [_, key] : positional_arguments_) {
      out << " " << key;
    }

    out << "\n\nPositional arguments:" << std::endl;

    for (auto& [ind, key] : positional_arguments_) {
      out << key << "\t" << arguments_[ind].help_info() << std::endl;
    }

    out << "\nOptional arguments:" << std::endl;

    for (auto& [ind, keys] : optional_arguments_) {
      for (std::size_t i = 0; i < keys.size(); ++i) {
        if (i) {
          out << ", ";
        }
        out << keys[i];
      }
      out << "\t" << arguments_[ind].help_info() << std::endl;
    }
  }

  template<typename... Ts>
  Argument& add_argument(const std::string& key, const Ts&... keys)
  {
    if (key.empty()) {
      throw std::invalid_argument("key cannot be empty");
    }

    if (key[0] != '-') {
      return add_positional_argument(key);
    }
    auto k = arguments_.size();
    optional_arguments_[k].push_back(key);
    keys_[get_main(key)] = k;
    if constexpr (sizeof...(Ts)) {
      add_optional_argument(keys...);
    }
    arguments_[k] = std::move(Argument());
    return arguments_[k];
  }

  template<typename V = std::string>
  V get(const std::string& key)
  {
    try {
      return arguments_[keys_[key]].get<V>();
    } catch (const std::exception& e) {
      throw std::runtime_error("not give the value of parameter " + key);
    }
  }

private:
  Argument& add_positional_argument(const std::string& key)
  {
    auto k = arguments_.size();
    positional_arguments_[k] = key;
    keys_[positional_arguments_[k]] = k;
    arguments_[k] = std::move(Argument());
    return arguments_[k];
  }

  template<typename... Ts>
  void add_optional_argument(const std::string& key, const Ts&... keys)
  {
    auto k = arguments_.size();
    optional_arguments_[k].push_back(key);
    keys_[get_main(key)] = k;
    if constexpr (sizeof...(Ts) > 0) {
      add_positional_argument(keys...);
    }
  }

  // assume arg starts with '-'
  std::string get_main(const std::string& arg)
  {
    if (arg[1] == '-') {
      return arg.substr(2);
    }
    return arg.substr(1);
  }

  std::string program_;
  std::map<std::string, std::size_t> keys_;
  std::map<std::size_t, std::string> positional_arguments_;
  std::map<std::size_t, std::vector<std::string>> optional_arguments_;
  std::map<std::size_t, Argument> arguments_;
};
} // namespace hol2cpp
