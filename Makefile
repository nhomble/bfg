
CC=cc
FLAGS=-O3 --pedantic -Wall -Werror
PROGS=bf

all: $(PROGS)

bf: bf.c
	$(CC) $(FLAGS) bf.c -o bf

.PHONY: clean

clean:
	rm -f *.o $(PROGS)
