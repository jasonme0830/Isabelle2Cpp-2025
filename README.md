# Isabelle2Cpp-2025

The first version hol2cpp of Isabelle2Cpp, converts Isabelle/HOL specifications to C++ codes, supports HOL on June 9, 2019.

The second version of Isabelle2Cpp, adds type system and supports type inference.

The third version of Isabelle2Cpp, adds the isomorphic type module.

Now the 2025 version of Isabelle2Cpp, supports deep copy for secondary development, maintains shallow copy in the definition-based conversion, adds move in the rule-based conversion, and implements memoization for int repetitive computation functions.

## Build

```bash
make # make bin/hol2cpp
make example # make examples in example/

make clean # clean target bin/hol2cpp
make cleanall # clean target and generated example codes
```

## Input examples

/example/optimize

raw: for the codes generated from the previous version Isabelle2Cpp

after: for the codes generated from the Isabelle2Cpp-2025

## Optimization options

## for test type

--close-typeCons   ## close shollow copy in the definition-based conversion

## for test function 

--close-moveStd   ## to close the move optimization for std containers
--close-memo   ## to close the memoization in functions
--close-isomorType    
--close-reduceCond

# target type selection
--use-deque
##--close-class
--use-struct

## Commands for conversion
/example/optimize/MYK_readme

```bash
bin/hol2cpp example/test_before.thy -s --predef bin/defs.pre
```

### Format

```bash
find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i --style=Mozilla
```

## Benchmark
To compare the generated codes of different version Isabelle2Cpp:
https://github.com/jasonme0830/Isabelle2Cpp-2025-test.git

To compare the codes generated from other generator: 
https://github.com/jasonme0830/Isabelle2Cpp-2025-Benchmark.git 


## TODO

- [ ] Support currying.
- [ ] Resolve identifier conflicts.
- [ ] Support recursive function to loop/iteration.
- [ ] Support configurable conversion rules.
- [ ] Support lambda expression.

## JSON

{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-gcc-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        }
    ],
    "version": 4
}

