all: ufsh
ufsh: commands.o main.o 
	# O compilador faz a ligação entre os dois objetos
	gcc -o ufsh commands.o main.o
#-----> Distancia com o botão TAB ### e não com espaços
commands.o: commands/commands.c
	gcc -o commands.o -c commands/commands.c -W -Wall
main.o: main.c commands/commands.h
	gcc -o main.o -c main.c -W -Wall
clean:
	rm -rf *.o
mrproper: clean
	rm -rf ufsh