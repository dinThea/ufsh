all: ufsh

CFLAGS = -W -m64 -Wall -std=c++1z

ufsh: main.o interpreter.o table.o registry_worker.o table_worker.o
	g++ -o ufsh main.o interpreter.o table.o registry_worker.o table_worker.o $(CFLAGS)
main.o: main.cpp
	g++ -o main.o -c main.cpp $(CFLAGS)
interpreter.o: src/interpreter.cpp
	g++ -o interpreter.o -c src/interpreter.cpp $(CFLAGS)
registry_worker.o: src/registry_worker.cpp
	g++ -o registry_worker.o -c src/registry_worker.cpp $(CFLAGS)
table_worker.o: src/table_worker.cpp
	g++ -o table_worker.o -c src/table_worker.cpp $(CFLAGS)
table.o: src/table.cpp
	g++ -o table.o -c src/table.cpp $(CFLAGS)
clean:
	rm -rf *.o
mrproper: clean
	rm -rf ufsh