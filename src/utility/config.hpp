#pragma once

#include <string>

namespace hol2cpp {
class Config
{
public:
  const std::string& input_file() const { return input_file_; }
  const std::string& output_file() const { return output_file_; }
  const std::string& predef_file() const { return predef_file_; }
  bool print_type() const { return print_type_; }

  // move list as much as possible
  bool close_move() const { return close_move_; }
  bool close_recu() const { return close_recu_; }
  bool close_memo() const { return close_memo_; }
  // removes the conditions for the last pattern for total
  // functions (not use option `nonexhaustive`)
  bool reduce_cond() const { return reduce_cond_; }
  // use std::deque instead of std::list as the target type for list
  bool use_deque() const { return use_deque_; }
  // to close the isomor datatype based on rule
  bool close_isomor() const { return close_isomor_; }

  // generates classes instead of structs for datatypes
  bool use_class() const { return use_class_; }
  // returns lambda-expression if the arguments are less than
  // parameters of the callee
  bool uncurry() const { return uncurry_; }

public:
  void input_file(std::string input_file) { input_file_ = input_file; }
  void output_file(std::string output_file) { output_file_ = output_file; }
  void predef_file(std::string predef_file) { predef_file_ = predef_file; }
  void print_type(bool enable) { print_type_ = enable; }

  void close_move(bool enable); // implemented in src/optimization/move_list.cpp
  void close_recu(bool enable) { close_recu_ = enable; }
  void close_memo(bool enable) { close_memo_ = enable; }
  void reduce_cond(bool enable) { reduce_cond_ = !enable; }
  void use_deque(bool enable); // implemented in src/optimization/use_deque.cpp
  void close_isomor(bool enable) {close_isomor_ = enable; };

  void use_class(bool enable) { use_class_ = !enable; }
  void uncurry(bool enable) { uncurry_ = enable; }

private:
  // common parameters
  std::string input_file_;
  std::string output_file_;
  std::string predef_file_;
  bool print_type_;

  // optimization options
  bool close_move_;
  bool close_recu_;
  bool close_memo_;
  bool reduce_cond_;
  bool use_deque_;
  bool close_isomor_;

  // experimental options that do not improve performance
  bool use_class_;
  bool uncurry_;
};

inline Config theConfig;
} // namespace hol2cpp
