#pragma once

#include <string>
#include <exception>

#define assertt(expr) if (!(expr)) { throw std::runtime_error(__FILE__ ":" + std::to_string(__LINE__) + ": Assertion `" #expr "' failed."); }
