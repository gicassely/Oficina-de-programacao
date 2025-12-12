PROG=crescente
CC=gcc -Wall -g

all: cartas.o main.o jogo.o interface.o
	${CC} -o bin/${PROG} cartas.o main.o jogo.o interface.o
cartas.o: src/cartas.c src/cartas.h
	${CC} -c src/cartas.c
jogo.o: src/jogo.c src/jogo.h src/cartas.h
	${CC} -c src/jogo.c
interface.o: src/interface.c src/interface.h
	${CC} -c src/interface.c
main.o: src/main.c src/jogo.h src/cartas.h src/interface.h
	${CC} -c src/main.c
clean:
	rm -rf *~ *.o ./\#*
