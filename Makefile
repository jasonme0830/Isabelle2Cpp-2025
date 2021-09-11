bin/hol2cpp: src/*.cpp | bin/
	clang++ -W -g -std=c++17 src/*.cpp -o bin/hol2cpp

bin/:
	mkdir bin

.PHONY: clean

clean:
	rm -r bin
