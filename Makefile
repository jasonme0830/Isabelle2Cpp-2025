bin/hol2cpp: src/*.cpp
	clang++ -std=c++17 src/*.cpp -o bin/hol2cpp
