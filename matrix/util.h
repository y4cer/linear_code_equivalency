#include <stdint.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#pragma once

#define BUFF_SIZE 1024

typedef struct {
    uint64_t k;
    uint64_t n;
    uint8_t** mat;
} matrix;

matrix* parse_matrix(FILE* input_stream);

void print_matrix(matrix* mat);

matrix* alloc_matrix(uint64_t k, uint64_t n);

void dealloc_matrix(matrix* matrix);

void copy_matrix(matrix* src, matrix* dest);

