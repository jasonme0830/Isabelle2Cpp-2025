
#include <iostream>

using namespace std;

/* source Isabelle/HOL
datatype nat = zero | suc nat

fun add :: "nat ⇒ nat ⇒ nat" where
  "add zero n = n" |
  "add (suc m) n = suc(add m n)"
*/

struct zero_t {};

template <typename T>
struct suc_t;

template <>
struct suc_t<zero_t>
{
    suc_t() = default;
    suc_t(zero_t) {}
};

template <typename T>
struct suc_t<suc_t<T>>
{
    suc_t() = default;
    suc_t(suc_t<T>) {}
};

template <typename T> suc_t(T) -> suc_t<zero_t>;
template <typename T> suc_t(suc_t<T>) -> suc_t<suc_t<T>>;

template <typename T1, typename T2>
auto add(T1 m, T2 n) {}

template <typename T>
auto add(zero_t, T n)
{
    return n;
}

template <typename T1, typename T2>
auto add(suc_t<T1> m, T2 n)
{
    return suc_t(add(T1(), n));
}

template <typename T>
int count(T v) {}

template <>
int count(zero_t)
{
    return 0;
}

template <typename T>
int count(suc_t<T>)
{
    return 1 + count(T());
}

int main(int argc, char *argv[])
{
    zero_t zero;

    auto one = suc_t(zero);
    auto two = add(one, one);
    auto three = add(two, one);

    cout << count(three) << endl;

    return 0;
}
