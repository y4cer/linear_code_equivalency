#include "math.h"
#include "util.h"

void transpose(uint64_t k, uint64_t n, char** mat, char** output_mat) {
    for (uint64_t row = 0; row < n; row++) {
        for (uint64_t col = 0; col < k; col++) {
            output_mat[row][col] = mat[col][row];
        }
    }
}

char dot_product(uint64_t k, char* first, char* second) {
    char result = 0;
    for (uint64_t i = 0; i < k; i++) {
        result = (result + first[i] * second[i]) % 2;
    }

    return result;
}

void id_matrix(uint64_t n, char** output_mat) {
    for (uint64_t x = 0; x < n; x++) {
         output_mat[x][x] = 1;
    }
}

void multiply(uint64_t k1, uint64_t n1, char** mat1,
                uint64_t k2, uint64_t n2, char** mat2,
                char** output_mat) {
    if (n1 != k2) {
        perror("Error: wrong given dimensions!");
        exit(1);
    }
    char** mat2_T = alloc_matrix(n2, k2);
    transpose(k2, n2, mat2, mat2_T);
    for (uint64_t row = 0; row < k1; row++) {
        for (uint64_t col2 = 0; col2 < n2; col2++) {
            char res = dot_product(k2, mat1[row], mat2_T[col2]);
            output_mat[row][col2] = res;
        }
    }
    dealloc_matrix(n2, k2, mat2_T);
}

