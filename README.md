# HOL2Cpp

Transformation from Isabelle/HOL to C++, supports HOL at the version on June 9, 2019

## Limitations

Only support cast the recursive definitions of functions from Isabelle/HOL to C++

## Example

You can see some examples in [example/](example/)

## TODO

- [ ] Generate function templates in .hpp file
- [ ] Reimplement function to support function currying
- [ ] Provide configurable conversion rules
- [ ] Do some benchmarks between generated cpp codes and native cpp codes
- [ ] Do some benchmarks between generated cpp codes and generated sml codes

## DONE

- [x] Reimplement the conversion based on definitions
- [x] Use more const reference to void copying and make sure copy only happens when constructing values
- [x] Remove unused variable declarations
- [x] Remove unused parens when generating expressions
- [x] Reimplement the function conversion by using if directly instead of for(;;) {}
    ```cpp
    std::uint64_t fib(const std::uint64_t &arg1) {
        // fib 0 = 1
        if (arg1 == 0) {
            return 1;
        }
        ...
    }
    ```
- [x] Support define infix operators
- [x] Support definitions with only one equation
- [x] Support parsing lambda expression
- [x] Supported predefined definitions will be skipped
- [x] Support expressions `[i..j]` and `[i..<j]`
- [x] Pass comments of definitions for proof
- [x] Enable the name of definition could be wrapped in `""`
- [x] Enable write `<=>` and `\<equiv>` which are equal to `=`
- [x] Support let-in and case expressions
- [x] Skip unsupported definitions and provide helpful error outputs
- [x] Add new command line argument `-s` to assign the same output file with input
- [x] Reimplement the parser, support compile complete theory file
- [x] Remove `construct` and use `std::make_shared` directly now
- [x] Add optimization of variables removal
- [x] Try to reuse values as many as possible [Use `const T &` for parameters and update all conversions to make sure that parameters won't be edited]

## Tips

1. Use shared_ptr in order to reuse existed values. For example, in `Cons x xs = Cons x xs`, xs will be reused without copy.
