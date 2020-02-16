
#include <memory>
#include <iostream>

using namespace std;

/* source Isabelle/HOL
datatype natt = zero | succ zero

fun add :: "natt ⇒ natt ⇒ natt" where
  "add zero n = n" |
  "add (succ m) n = succ(add m n)"
*/

template <typename T>
using Ptr = shared_ptr<T>;

// declare the names in datatype definition
struct natt_t;
struct zero_t;
struct succ_t;

// define the subtype in datatype definetion
// generate zero
struct zero_t {};

// generate from succ natt
struct succ_t
{
    Ptr<natt_t> natt;

    succ_t(Ptr<natt_t> natt)
      : natt(natt) {}
};

// generate from zero | succ natt
struct natt_t
{
    Ptr<zero_t> zero;
    Ptr<succ_t> succ;

    natt_t(Ptr<zero_t> zero)
      : zero(zero) {}
    natt_t(Ptr<succ_t> succ)
      : succ(succ) {}
};

// define the function
Ptr<natt_t> add(Ptr<natt_t> m, Ptr<natt_t> n)
{
    if (m->zero) // add zero n
    {
        return n; // = n
    }
    else if (m->succ) // add (succ m) n
    {
        // unpack
        m = m->succ->natt;
        // generate result from "= succ(add m n)"
        return make_shared<natt_t>(
            make_shared<succ_t>(add(m, n)));
    }
    return nullptr;
}

int count(Ptr<natt_t> var)
{
    if (var->zero)
    {
        return 0;
    }
    else
    {
        return 1 + count(var->succ->natt);
    }
}

int main(int argc, char *argv[])
{
    auto zero = make_shared<natt_t>(make_shared<zero_t>());
    auto one = make_shared<natt_t>(make_shared<succ_t>(zero));

    auto two = add(one, one);
    auto four = add(two, two);
    auto six = add(two, four);

    cout << count(six) << endl;

    return 0;
}
