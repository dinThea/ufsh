all: ufsh

CFLAGS = -W -m64

ufsh: main.o interpreter.o registry.o table.o
	g++ -o ufsh main.o interpreter.o registry.o table.o $(CFLAGS)
main.o: main.cpp
	g++ -o main.o -c main.cpp
interpreter.o: src/interpreter.cpp
	g++ -o interpreter.o -c src/interpreter.cpp
registry.o: src/registry.cpp
	g++ -o registry.o -c src/registry.cpp
table.o: src/table.cpp
	g++ -o table.o -c src/table.cpp
clean:
	rm -rf *.o
mrproper: clean
	rm -rf ufsh