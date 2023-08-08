CC=gcc
C_FLAGS=-std=c11

ARTIFACTS=

ARTIFACTS+=main

main: main.c
	$(CC) $(C_FLAGS) main.c -o main

.PHONY: clean
clean:
	rm -f ${ARTIFACTS}
