#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#include "util.h"

void multiply(matrix* mat1, matrix* mat2, matrix* output_mat);

void transpose(matrix* input_mat, matrix* out);

char dot_product(uint64_t k, char* first, char* second);

void id_matrix(uint64_t n, matrix* output_mat);

int
gaussian_elimination(
    matrix* input_mat,
    matrix* output_mat,
    uint64_t* permutations
);

void add_vectors(uint64_t k, char* first, char* second, char* output_vec);

void
swap_columns(
    matrix* input_mat,
    uint64_t first,
    uint64_t second,
    matrix* output_mat
);

void
swap_rows(
    matrix* input_mat,
    uint64_t first,
    uint64_t second,
    matrix* output_mat
);

void
get_submatrix(
    matrix* input_mat,
    uint64_t start_row,
    uint64_t end_row,
    uint64_t start_col,
    uint64_t end_col,
    matrix* output_mat
);

void concat_matrices_h(matrix* left, matrix* right, matrix* output);

void concat_matrices_v(matrix* upper, matrix* lower, matrix* output);

int build_check_matrix(matrix* input_mat, matrix* H);

