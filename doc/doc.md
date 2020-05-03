# Supporting Document

> Support the main types provided by theory *Main*, at the version on June 9, 2019

## Types Cast by Default

Isabelle/HOL | C++
- | -
nat | std::uint64_t
int | std::int64_t
'a set | std::set\<T0\>
'a list | std::list\<T0\>
'a option | std::optional\<T0\>
'a * 'b | std::pair\<T0, T1\>

## Patterns Mapping

> for example, *prev* is `arg1` in `rev (Cons x xs)`

### *Numeral*

as a condiation like `prev != numeral`

### Nil

as a condiation like `!prev.empty()`

### Cons x xs

```cpp
auto x = prev.front();
prev.pop_front();
auto xs = prev;
```

## Expressions Mapping

> *temp* is the result of the given expression

### *Numeral*

just as the numeral's value like `auto temp = numeral`

### Nil

{} of the result type like `auto temp = std::list<T>{}`

### Cons x xs

```cpp
auto temp = xs;
temp.push_front(x);
```
