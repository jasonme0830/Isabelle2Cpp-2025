BIN = bin
target = $(BIN)/hol2cpp

src = src/main.cpp $(wildcard src/*/*.cpp)
obj = $(patsubst src/%.cpp, obj/%.o, $(src))

def = defs.pre

$(target): $(obj) | $(BIN)/
	g++ -o $@ -W -g -std=c++17 $^
	cp src/$(def) $(BIN)/

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ -W -Wno-implicit-fallthrough -g -std=c++17 -c $^ -o $@

bin/:
	mkdir bin

thy = $(wildcard example/*.thy) $(wildcard example/*/*.thy)
hpp = $(patsubst %.thy, %.hpp, $(thy))
cpp = $(patsubst %.thy, %.cpp, $(thy))

.PHONY: clean bench example

clean:
	rm -rf $(target)
	rm -rf $(obj)

cleanall:
	rm -rf $(target)
	rm -rf $(obj)
	rm -rf $(hpp)
	rm -rf $(cpp)
	cd bench && make clean

bench: $(target)
	cd bench && make bm

example: $(cpp)

example/%.cpp: example/%.thy $(target)
	$(target) $< -s --reduce-cond --use-class --predef $(BIN)/$(def)
