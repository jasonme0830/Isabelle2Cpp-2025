
#include <iostream>

using namespace std;

/* source Isabelle/HOL
datatype nat = 0 | Suc nat

fun add :: "nat ⇒ nat ⇒ nat" where
  "add 0 n = n" |
  "add (Suc m) n = Suc(add m n)"
*/

struct Zero_t {};

template <typename T>
struct Suc;

template <>
struct Suc<Zero_t>
{
    Suc() = default;
    Suc(Zero_t) {}
};

template <typename T>
struct Suc<Suc<T>>
{
    Suc() = default;
    Suc(Suc<T>) {}
};

template <typename T> Suc(T) -> Suc<Zero_t>;
template <typename T> Suc(Suc<T>) -> Suc<Suc<T>>;

template <typename T1, typename T2>
auto add(T1 m, T2 n) {}

template <typename T>
auto add(Zero_t, T n)
{
    return n;
}

template <typename T1, typename T2>
auto add(Suc<T1> m, T2 n)
{
    return Suc(add(T1(), n));
}

template <typename T>
int count(T v) {}

template <>
int count(Zero_t)
{
    return 0;
}

template <typename T>
int count(Suc<T>)
{
    return 1 + count(T());
}

int main(int argc, char *argv[])
{
    Zero_t zero;

    auto one = Suc(zero);
    auto two = add(one, one);
    auto three = add(two, one);

    cout << count(three) << endl;

    return 0;
}
