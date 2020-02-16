
#include <memory>
#include <iostream>

using namespace std;

/* source Isabelle/HOL
datatype nat = zero | suc zero

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

// define the subtype in datatype definetion
// generate zero
struct zero_t {};

// generate from suc nat
struct suc_t
{
    Ptr<nat_t> nat;

    suc_t(Ptr<nat_t> nat)
      : nat(nat) {}
};

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

// define the function
Ptr<nat_t> add(Ptr<nat_t> m, Ptr<nat_t> n)
{
    if (m->zero) // add zero n
    {
        return n; // = n
    }
    else if (m->suc) // add (suc m) n
    {
        // unpack
        m = m->suc->nat;
        // generate result from "= suc(add m n)"
        return make_shared<nat_t>(
            make_shared<suc_t>(add(m, n)));
    }
    return nullptr;
}

int count(Ptr<nat_t> var)
{
    if (var->zero)
    {
        return 0;
    }
    else
    {
        return 1 + count(var->suc->nat);
    }
}

int main(int argc, char *argv[])
{
    auto zero = make_shared<nat_t>(make_shared<zero_t>());
    auto one = make_shared<nat_t>(make_shared<suc_t>(zero));

    auto two = add(one, one);
    auto four = add(two, two);
    auto six = add(two, four);

    cout << count(six) << endl;

    return 0;
}
