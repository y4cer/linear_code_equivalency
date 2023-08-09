#include "math.h"

char** transpose(uint64_t k, uint64_t n, char** mat) {
    char** transposed = (char**) malloc(n * sizeof(char*));

    for (uint64_t row = 0; row < n; row++) {
        transposed[row] = (char*) malloc(k * sizeof(char));
        for (uint64_t col = 0; col < k; col++) {
            transposed[row][col] = mat[col][row];
        }
    }

    return transposed;
}

char** multiply(uint64_t k1, uint64_t n1, char** mat1,
                uint64_t k2, uint64_t n2, char** mat2) {
    if (n1 != k2) {
        perror("Error: wrong given dimensions!");
        exit(1);
    }

    char** result = (char**) malloc(k1 * sizeof(char));
    for (uint64_t row = 0; row < k1; row++) {
        printf("asd\n");
    }

    return NULL;
}

