CC=gcc
C_FLAGS=-std=c11

ARTIFACTS=

ARTIFACTS+=main
ARTIFACTS+=util.o
ARTIFACTS+=math.o

main: main.c util.o math.o
	$(CC) $(C_FLAGS) main.c util.o math.o -o main

util.o: matrix/util.c
	$(CC) $(C_FLAGS) -c matrix/util.c -o util.o

math.o: matrix/math.c
	$(CC) $(C_FLAGS) -c matrix/math.c -o math.o

.PHONY: clean
clean:
	rm -f ${ARTIFACTS}
