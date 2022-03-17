bin/hol2cpp: src/codegen/*.cpp src/ir/*.cpp src/parser/*.cpp src/synthesis/*.cpp src/utility/*.cpp src/main.cpp | bin/
	g++ -o bin/hol2cpp -W -Wno-implicit-fallthrough -g -std=c++17 $^

bin/:
	mkdir bin

.PHONY: clean

clean:
	rm -r bin
	rm -r obj
