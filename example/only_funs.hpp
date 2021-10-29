#include <cstdint>
#include <cstdlib>
#include <list>
#include <optional>
#include <set>
#include <utility>

std::uint64_t add(const std::uint64_t &arg1, const std::uint64_t &arg2);

template<typename T1>
std::list<T1> app(const std::list<T1> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> rev(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> rev2(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> rev3(const std::list<T1> &arg1);

template<typename T1>
std::optional<std::list<T1>> testoption(const std::optional<T1> &arg1);

template<typename T1>
std::set<T1> testset(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> testifelse(const std::list<T1> &arg1, const T1 &arg2);

template<typename T1, typename T2>
std::pair<T1, T2> pair(const T1 &arg1, const T2 &arg2);

template<typename T1, typename T2>
T1 fst(const std::pair<T1, T2> &arg1);

template<typename T1, typename T2>
T2 snd(const std::pair<T1, T2> &arg1);

bool evn(const std::uint64_t &arg1);

std::uint64_t fib(const std::uint64_t &arg1);

bool altrue(const std::uint64_t &arg1);

