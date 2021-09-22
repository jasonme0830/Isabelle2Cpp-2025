# HOL2Cpp

Transformation from Isabelle/HOL to C++, supports HOL at the version on June 9, 2019

## Limitations

Only support cast the recursive definitions of functions from Isabelle/HOL to C++

## Example

You can see some examples in [example/](example/)

## TODO

- [ ] Implement function bind
- [ ] Provide configurable conversion rules
- [ ] Do some benchmarks between generated cpp codes and native cpp codes
- [ ] Do some benchmarks between generated cpp codes and generated sml codes

## DONE

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
