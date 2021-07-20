# HOL2Cpp

Transformation from Isabelle/HOL to C++, supports HOL at the version on June 9, 2019

## Limitations

Only support cast the recursive definitions of functions from Isabelle/HOL to C++

## Example

You can see some examples in [example/](example/)

## TODO

- [ ] Reimplement the parser
- [ ] Provide configurable conversion rules
- [ ] Do some benchmarks between generated cpp codes and native cpp codes
- [ ] Do some benchmarks between generated cpp codes and generated sml codes

## DONE

- [x] Add optimization of variables removal
- [x] Try to reuse values as many as possible [Use `const T &` for parameters and update all conversions to make sure that parameters won't be edited]
