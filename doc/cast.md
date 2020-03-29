# Cast by Default

> Support the main types, functions and syntax provided by theory *Main*, at the version on June 9, 2019

## Types Cast

Isabelle/HOL | C++
- | -
nat | std::uint64_t
int | std::int64_t
'a set | std::set\<T\>
'a option | std::optional
'a list | std::list\<T\>

## Patterns Cast

> The *prev* means the last intermediate result

Isabelle/HOL | C++
- | -
Nil | 0
Suc remain | remain =/== (prev) - 1

## Expressions Cast

Isabelle/HOL | C++
- | -
Suc expr | (expr) + 1
