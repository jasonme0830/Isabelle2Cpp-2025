target: a.out b.out c.out d.out
	time ./a.out > /dev/null
	time ./b.out > /dev/null
	time ./c.out > /dev/null
	time ./d.out > /dev/null

a.out: mainA.cpp
	g++ -std=c++17 -O2 mainA.cpp -o a.out

b.out: mainB.cpp
	g++ -std=c++17 -O2 mainB.cpp -o b.out

c.out: mainC.cpp
	g++ -std=c++17 -O2 mainC.cpp -o c.out

d.out: mainD.cpp
	g++ -std=c++17 -O2 mainD.cpp -o d.out
