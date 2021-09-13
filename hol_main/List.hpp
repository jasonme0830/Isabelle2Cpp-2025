#include <cstdint>
#include <cstdlib>
#include <functional>
#include <list>
#include <optional>
#include <set>

template<typename T1>
std::list<T1> butlast(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> rev(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> filter(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> concat(const std::list<std::list<T1>> &arg1);

template<typename T1>
std::list<T1> takeWhile(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> dropWhile(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> insert(const T1 &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> union(const std::list<T1> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::optional<T1> find(const std::function<bool(T1)> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> remove1(const T1 &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> removeAll(const T1 &arg1, const std::list<T1> &arg2);

template<typename T1>
std::list<T1> remdups(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> remdups_adj(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> rotate1(const std::list<T1> &arg1);

template<typename T1>
std::list<std::list<T1>> subseqs(const std::list<T1> &arg1);

template<typename T1>
std::list<T1> splice(const std::list<T1> &arg1, const std::list<T1> &arg2);

template<typename T1, typename T2>
std::list<T1> insort_key(const std::function<T2(T1)> &arg1, const T1 &arg2, const std::list<T1> &arg3);

template<typename T1, typename T2>
std::list<T1> sort_key(const std::function<T2(T1)> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::set<std::list<T1>> listset(const std::list<std::set<T1>> &arg1);

template<typename T1, typename T2>
std::list<T2> bind(const std::list<T1> &arg1, const std::function<std::list<T2>(T1)> &arg2);

template<typename T1, typename T2>
std::list<T2> map_tailrec_rev(const std::function<T2(T1)> &arg1, const std::list<T1> &arg2, const std::list<T2> &arg3);

template<typename T1, typename T2>
std::list<T2> map_tailrec(const std::function<T2(T1)> &arg1, const std::list<T1> &arg2);

template<typename T1>
std::uint64_t gen_length(const std::uint64_t &arg1, const std::list<T1> &arg2);

