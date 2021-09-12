#include <cstdint>
#include <cstdlib>
#include <list>

std::uint64_t add(const std::uint64_t &arg1, const std::uint64_t &arg2);

template<typename T1>
std::list<T1> app(const std::list<T1> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> rev(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> rev2(const std::list<T1> &arg1);

