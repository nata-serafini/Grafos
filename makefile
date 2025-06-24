CC = gcc
CFLAGS = -Wall -O2

all: testegrafo

testegrafo: grafo.o teste.o
	$(CC) $(CFLAGS) -o testegrafo grafo.o teste.o

grafo.o: grafo.c grafo.h
	$(CC) $(CFLAGS) -c grafo.c

teste.o: teste.c grafo.h
	$(CC) $(CFLAGS) -c teste.c

clean:
	rm -f *.o testegrafo
