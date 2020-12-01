bin/hol2cpp: src/*.cpp | bin/
	clang++ -std=c++17 src/*.cpp -o bin/hol2cpp

bin/:
	mkdir bin
