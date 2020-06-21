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
#include <cstdint>
#include <list>

template<typename T0>
std::list<T0>
app(std::list<T0> arg1, std::list<T0> arg2) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        auto ys = arg2;
        return ys;
    }
    for (;;) {
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto ys = arg2;
        auto temp0 = app(xs, ys);
        temp0.push_front(x);
        return temp0;
    }
}

template<typename T0>
std::list<T0>
rev(std::list<T0> arg1) {
    for (;;) {
        if (!arg1.empty()) {
            break;
        }
        return std::list<T0>();
    }
    for (;;) {
        auto x = arg1.front();
        arg1.pop_front();
        auto xs = arg1;
        auto temp0 = std::list<decltype(x)>{x};
        return app(rev(xs), temp0);
    }
}
```

## To Do

- [ ] Record all defined functions' types to reduce expressions' types in call expression
