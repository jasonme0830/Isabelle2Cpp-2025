#include <memory>
#include <iostream>
#include <type_traits>

using namespace std;

/* source Isabelle/HOL
datatype 'a List = Nil | Cons 'a " 'a List"

fun app :: "'a List ⇒ 'a List ⇒ 'a List" where
    "app Nil ys = ys" |
    "app (Cons x xs) ys = Cons x (app xs ys)"

fun rev :: "'a List ⇒ 'a List" where
    "rev Nil = Nil"
    |"rev (Cons x xs) = app (rev xs) (Cons x Nil)"
*/

template <typename T>
using Ptr = shared_ptr<T>;

template <typename T>
struct List;
struct Nil_t;
template <typename T>
struct Cons_t;

template <typename T>
struct List
{
    List(Ptr<Nil_t> nil= make_shared<Nil_t>())
      : nil(nil), cons(nullptr) {}

    List(Ptr<Cons_t<T>> cons)
      : nil(nullptr), cons(cons) {}

    Ptr<Nil_t> nil;
    Ptr<Cons_t<T>> cons;
};

struct Nil_t {};

template <typename T>
struct Cons_t
{
    Cons_t(T pos1, List<T> pos2)
      : pos1(pos1), pos2(pos2) {}

    T pos1;
    List<T> pos2;
};

template <typename T>
List<T> app(List<T> pos1, List<T> pos2)
{
    if (pos1.nil)
    {
        auto ys = pos2;
        return pos2;
    }
    else if (pos1.cons)
    {
        auto x = pos1.cons->pos1;
        auto xs = pos1.cons->pos2;
        auto ys = pos2;
        return List<T>(make_shared<Cons_t<T>>(x, app(xs, ys)));
    }
    abort();
}

template <typename T>
List<T> rev(List<T> pos1)
{
    if (pos1.nil)
    {
        return pos1;
    }
    else if (pos1.cons)
    {
        auto x = pos1.cons->pos1;
        auto xs = pos1.cons->pos2;
        return app(
            rev(xs),
            List<T>(make_shared<Cons_t<T>>(x, make_shared<Nil_t>()))
        );
    }
    abort();
}

int main()
{
    List<int> list1,
              list2(make_shared<Cons_t<int>>(1, make_shared<Nil_t>()));
    auto list3 = app(list1, list2);
    auto list4 = rev(list3);
    return 0;
}
