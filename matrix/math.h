#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

void
multiply(
    uint64_t k1,
    uint64_t n1,
    char** mat1,
    uint64_t k2,
    uint64_t n2,
    char** mat2,
    char** output_mat
);

void transpose(uint64_t k, uint64_t n, char** mat, char** output_mat);

char dot_product(uint64_t k, char* first, char* second);

void id_matrix(uint64_t n, char** output_mat);

int
gaussian_elimination(
    uint64_t k,
    uint64_t n,
    char** mat,
    char** output_mat,
    uint64_t* permutations
);

void add_vectors(uint64_t k, char* first, char* second, char* output_vec);

void
swap_columns(
        uint64_t k,
        uint64_t n,
        uint64_t first,
        uint64_t second,
        char** mat,
        char** output_mat
);

void
get_submatrix(
    uint64_t k,
    uint64_t n,
    uint64_t start_row,
    uint64_t end_row,
    uint64_t start_col,
    uint64_t end_col,
    char** mat,
    char** output_mat
);

void
concat_matrices_h(
    uint64_t k,
    uint64_t n1,
    uint64_t n2,
    char** mat1,
    char** mat2,
    char** output_mat
);

void
concat_matrices_v(
    uint64_t n,
    uint64_t k1,
    uint64_t n2,
    char** mat1,
    char** mat2,
    char** output_mat
);

int build_check_matrix(uint64_t k, uint64_t n, char** mat, char** output_mat);

