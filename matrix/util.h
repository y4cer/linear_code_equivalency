#include <stdint.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

/**
  * Parses user input and stores it in matix form.
  * k rows, n columns
  */
char** parse_matrix(uint64_t k, uint64_t n);


void print_matrix(int k, int n, char** mat);


void dealloc_matrix(int k, int n, char** mat);

