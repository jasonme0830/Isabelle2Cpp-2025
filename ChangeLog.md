## ChangeLog for HOL2Cpp

### Unreleased
#### Added
- Support type inference for function definitions, now every expression and its sub-expressions of the function definition has its expression type.
- Support virtual clone for class `Type`.
- Add new token `\<pre-def>` and its type `PreDefined`.
- New class `PreFunctionDef` is added, in order to support the resolution of pre-defined functions.
- Support the resolution of pre-defined functions, which have the type only.
- New option `--print-type` is supported, which enables type print after the type inference.
- Support the resolution of multiple types of variables.
- The parameters' type of the lambda expressions are now instantiated with a function call or type construction.

#### Fixed

- Fix the bug of the type inference for case-expressions, which lacks the type of `expr` unexpectedly.
- Function definitions and datatype definitions that do not currently support curring will throw an exception and skip the resolution of the current definition.

#### Changed

- `Ptr` is `std::shared_ptr<T>` now.

### 0.0.1 - 2022/10/10

#### Added

- New option `--use-deque` is supported, which enables generating `std::deque` instead of `std::list` for the datatype `list` and improves the performance most of the time.

#### Fixed

- Fix the bug of the conversion for case-expressions, which discards declarations unexpectedly.

#### Changed

- `--reduce-cond` does not reduce conditions and the `std::abort` if the function defined by `primrec` is followed by the option `nonexhaustive`. Furthermore, `--reduce-cond` assumes functions are total by default.

#### Detail

##### `-reduce-cond`

The behavior of the option is changed according to the [the nonexhaustive option](https://isabelle.in.tum.de/dist/doc/datatypes.pdf) in `primrec`:

> The nonexhaustive option indicates that the functions are not necessarily specified for all constructors. It can be used to suppress the warning that is normally emitted when some constructors are missing.

### 0.0.0 - 2022/03/27

#### Happened

- Fix the bug of the generation for let-in expressions, which discards declarations unexpectedly.
- Operator `!` and `nth` is supported.
- Support generating classes optionally (by using the flag `--use-class`) instead of structs for the user-defined datatypes.
- `reduce-cond` is added as an optimization option which removes last unnecessary conditions and the call of `abort`.
- Variables are checked if movable or not and unexpected move operations are fixed.
- `move-list` is supported and as an optimization option by the flag `--move-list`.
- Declarations are delayed after all conditions.
- Directory structure is reorganized.
- Use `std::set::merge` instead trivial traversal for `\<inter>` and `\<union>` of `set`.
- Use `std::list::splice` instead of `std::list::insert` for `xs @ ys`.
- Function templates are generated in the `.hpp` file.
- Use static constructors with `std::make_shared` inside.
- Recursive types are converted to one C++ struct instead two types which include an indirect auxiliary type.
- (**Unstable**) More const reference are used to void copying and make sure copy only happens when constructing values.
- Eliminate redundant variable declarations. `argN` is used directly in expressions if possible.
- Eliminate needless parens when generating expressions.
- Use if-statements directly instead of `for(;;) {}` in the function conversion.
    ```cpp
    std::uint64_t fib(const std::uint64_t &arg1) {
        // fib 0 = 1
        if (arg1 == 0) {
            return 1;
        }
        ...
    }
    ```
- Support function definitions with infix operators.
- Support function definitions with only one equation.
- Lambda expressions can be parsed.
- Predefined definitions are skipped (but counted).
- Support expressions `[i..j]` and `[i..<j]`.
- Comments are skipped.
- Enable the name of definition could be wrapped in `""`.
- Enable symbols `<=>` and `\<equiv>` which are equal to `=`.
- Support let-in and case expressions.
- Skip unsupported definitions and provide helpful error outputs.
- Add new command line argument `-s` to assign the same output file with input.
- Reimplement the parser, support compile complete theory file.
- ~~Remove `construct` and use `std::make_shared` directly now.~~
