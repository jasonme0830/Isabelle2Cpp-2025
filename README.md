# HOL2Cpp

Transformation from Isabelle/HOL to C++, supports HOL at the version on June 9, 2019

## Limitations

Only support cast the recursive definitions of functions from Isabelle/HOL to C++

## Example

Before:

```isabelle
fun app :: "'a list \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "app Nil ys = ys" |
  "app (Cons x xs) ys = Cons x (app xs ys)"

fun rev :: "'a list \<Rightarrow> 'a list" where
  "rev Nil = Nil" |
  "rev (Cons x xs) = app (rev xs) (Cons x Nil)"
```

After:

```cpp
#include <list>
#include <cctypes>

template<typename T0>
std::list<T0> app(std::list<T0> arg1, std::list<T0> arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        auto ys = arg2;
        return ys;
    }
    for (;;) {
        auto temp0 = arg1.front();
        auto x = temp0;
        arg1.pop_front();
        auto temp1 = arg1;
        auto xs = temp1;
        auto ys = arg2;
        auto temp2 = app(xs, ys);
        auto temp3 = temp2;
        temp3.push_front(x);
        return temp3;
    }
}

template<typename T0>
std::list<T0> rev(std::list<T0> arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T0>{};
    }
    for (;;) {
        auto temp0 = arg1.front();
        auto x = temp0;
        arg1.pop_front();
        auto temp1 = arg1;
        auto xs = temp1;
        auto temp2 = rev(xs);
        auto temp3 = std::list<T0>{};
        temp3.push_front(x);
        auto temp4 = app(temp2, temp3);
        return temp4;
    }
}
```
