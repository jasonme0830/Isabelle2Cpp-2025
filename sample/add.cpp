
#include <memory>
#include <iostream>

using namespace std;

/* source Isabelle/HOL
datatype nat = zero | suc nat

fun add :: "nat ⇒ nat ⇒ nat" where
  "add zero n = n" |
  "add (suc m) n = suc(add m n)"
*/

template <typename T>
using Ptr = shared_ptr<T>;

// declare the names in datatype definition
struct nat_t;
struct zero_t;
struct suc_t;

// generate from zero | suc nat
struct nat_t
{
    Ptr<zero_t> zero;
    Ptr<suc_t> suc;

    nat_t(Ptr<zero_t> zero)
      : zero(zero) {}
    nat_t(Ptr<suc_t> suc)
      : suc(suc) {}
};

// define the subtype in datatype definetion
// generate zero
struct zero_t {};

// generate from suc nat
struct suc_t
{
    nat_t nat;

    suc_t(nat_t nat)
      : nat(nat) {}
};

// define the function
nat_t add(nat_t m, nat_t n)
{
    if (m.zero) // add zero n
    {
        return n; // = n
    }
    else if (m.suc) // add (suc m) n
    {
        // unpack
        m = m.suc->nat;
        // generate result from "= suc(add m n)"
        return nat_t(make_shared<suc_t>(suc_t(add(m, n))));
    }
    abort();
}

int count(nat_t var)
{
    if (var.zero)
    {
        return 0;
    }
    else
    {
        return 1 + count(var.suc->nat);
    }
}

int main(int argc, char *argv[])
{
    auto zero = nat_t(make_shared<zero_t>());
    auto one = nat_t(make_shared<suc_t>(zero));

    auto two = add(one, one);
    auto four = add(two, two);
    auto six = add(two, four);

    cout << count(six) << endl;

    return 0;
}
