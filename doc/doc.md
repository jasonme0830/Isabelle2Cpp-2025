# Supporting Document

> Support the main types provided by theory *Main*, at the version on June 9, 2019

## Types Cast by Default

Isabelle/HOL | C++
- | -
nat | std::uint64_t
int | std::int64_t
't set | std::set\<T0\>
't list | std::list\<T0\>
't option | std::optional\<T0\>
't0 * 't1 | std::pair\<T0, T1\>

And type variables also can be replaced by concrete types, for example, `('t0 * 't1) list` will be translated to `std::list<std::pair<T0, T1>>` in C++

## Patterns Mapping

> `prev` means the previous variable for patterns, for example, `prev` is `arg0`, the first parameter in the generated declaration, when generating code for `Cons x xs` in `name-of-function (Cons x xs)`

> You will see `($n)` later, `($n)` means it is the nth argument of the given constructor

### Variable

A single name stands for a variable if it is not supported like Nil, None and so on. It will generate a declaration statement for the variable like:

```cpp
auto var-name = prev;
```

### *Numeral*

#### Numeral Literal

As a condition like:

```cpp
// 0 or other numeral literals
if (prev != 0) {
    break;
}
```

#### Suc ($1)

`(prev) - 1` as the `prev` of `($1)`

### Boolean

As a condition like:

```cpp
if (prev != true) {
    break;
}
```

or

```cpp
if (prev != false) {
    break;
}
```

### Set



### List

#### Nil

As a condition like:

```cpp
if (!prev.empty()) {
    break;
}
```

#### Cons ($1) ($2)

```cpp
if (prev.empty()) {
    break;
}

// prev.front() as the prev of ($1)
// then generate code for ($1)
prev.pop_front();
// prev as the prev of ($2)
// then generate code for ($2)
```

### Option

#### None

As a condition like:

```cpp
if (prev.has_value()) {
    break;
}
```

#### Some ($1)

```cpp
if (!prev.has_value()) {
    break;
}
// prev.value() as prev of ($1)
```

### Pair

#### Pair ($1) ($2)

`prev.first` as the `prev` of `($1)`

`prev.second` as the `prev` of `($2)`

## Expressions Mapping

> *temp* is the result of the given expression

### *Numeral*

just as the numeral's value like `auto temp = numeral`

### Nil

{} of the type like `auto temp = std::list<T>{}`

### Cons x xs

```cpp
auto temp = xs;
temp.push_front(x);
```
