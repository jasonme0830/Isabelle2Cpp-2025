# HOL2Cpp

Conversion from Isabelle/HOL to C++, supports HOL at the version on June 9, 2019.

## Example

You can see some examples in [example/](example/).

## TODO

- [ ] Support recursive function to loop/iteration.
- [ ] Resolve identifier conflicts.
- [ ] Reimplement function conversion to support function currying.
- [ ] Support configurable conversion rules.
- [ ] Design benchmarks between generated cpp codes and native cpp codes.
- [ ] Design benchmarks between generated cpp codes and generated sml codes.

## ChangeLog

- [x] Reorganize directory structure
- [x] Use `std::set::merge` instead trivial traversal for `\<inter>` and `\<union>` of `set`.
- [x] Use `std::list::splice` instead of `std::list::insert` for `xs @ ys`.
- [x] Function templates are generated in the `.hpp` file.
- [x] Use static constructors with `std::make_shared` inside.
- [x] Recursive types are converted to one C++ struct instead two types which include an indirect auxiliary type.
- [x] (**Unstable**) More const reference are used to void copying and make sure copy only happens when constructing values.
- [x] Eliminate redundant variable declarations. `argN` is used directly in expressions if possible.
- [x] Eliminate needless parens when generating expressions.
- [x] Use if-statements directly instead of `for(;;) {}` in the function conversion.
    ```cpp
    std::uint64_t fib(const std::uint64_t &arg1) {
        // fib 0 = 1
        if (arg1 == 0) {
            return 1;
        }
        ...
    }
    ```
- [x] Support function definitions with infix operators.
- [x] Support function definitions with only one equation.
- [x] Lambda expressions can be parsed.
- [x] Predefined definitions are skipped (but counted).
- [x] Support expressions `[i..j]` and `[i..<j]`.
- [x] Comments are skipped.
- [x] Enable the name of definition could be wrapped in `""`.
- [x] Enable symbols `<=>` and `\<equiv>` which are equal to `=`.
- [x] Support let-in and case expressions.
- [x] Skip unsupported definitions and provide helpful error outputs.
- [x] Add new command line argument `-s` to assign the same output file with input.
- [x] Reimplement the parser, support compile complete theory file.
- [x] ~~Remove `construct` and use `std::make_shared` directly now.~~
