bin/hol2cpp: src/codegen/*.cpp \
			 src/ir/*.cpp \
			 src/parser/*.cpp \
			 src/synthesis/*.cpp \
			 src/utility/*.cpp \
			 src/optimizer/*.cpp \
			 src/main.cpp \
			 | bin/
	g++ -o bin/hol2cpp -W -Wno-implicit-fallthrough -g -std=c++17 $^

bin/:
	mkdir bin

.PHONY: clean example

clean:
	rm -r bin

example: bin/hol2cpp
	for file in `find example/*.thy`; \
	do \
		echo "<CONVERT> $$file"; \
		bin/hol2cpp $$file -s --move-list --reduce-cond --use-class; \
		echo ""; \
	done; \
	bin/hol2cpp example/hol_main/List_raw.thy -s --move-list --reduce-cond --use-class
