bin/hol2cpp: src/* | bin/
	g++ -W -Wno-implicit-fallthrough -g -std=c++17 src/*.cpp -o bin/hol2cpp

bin/:
	mkdir bin

.PHONY: clean

clean:
	rm -r bin
