sssp: main.o
	g++ -pthread -std=c++11 -Wall -o sssp main.o

main.o: main.cpp
	g++ -c -pthread -std=c++11 -Wall -o main.o main.cpp

clean:
	rm -f main.o 
	rm -f sssp
