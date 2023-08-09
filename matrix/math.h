#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void multiply(uint64_t k1, uint64_t n1, char** mat1,
                uint64_t k2, uint64_t n2, char** mat2,
                char** output_mat);

void transpose(uint64_t k, uint64_t n, char** mat, char** output_mat);

char dot_product(uint64_t k, char* first, char* second);

void id_matrix(uint64_t n, char** output_mat);

