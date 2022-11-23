# HOL2Cpp

Conversion from Isabelle/HOL to C++, supports HOL at the version on June 9, 2019.

## Build

```bash
make # make bin/hol2cpp
make example # make examples in example/

make clean # clean target bin/hol2cpp
make cleanall # clean target and generated example codes

make bench # make benchmarks, not work now
```

## Code Style

### Format

```bash
find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i --style=Mozilla
```

## TODO

- [ ] Support currying.
- [ ] Resolve identifier conflicts.
- [ ] Support recursive function to loop/iteration.
- [ ] Support configurable conversion rules.
- [ ] Add type system and support type inference.

## Example

You can see some examples in [example/](example/).

## ChangeLog

See [ChangeLog.md](ChangeLog.md)
