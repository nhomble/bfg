
CC=cc
FLAGS=-O3 --pedantic -Wall -Werror
PROGS=encoder

all: $(PROGS)

encode: encoder.c
	$(CC) $(FLAGS) encoder.c -o bf

.PHONY: clean

clean:
	rm -f *.o $(PROGS)
