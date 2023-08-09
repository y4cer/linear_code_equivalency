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

char dot_product(uint64_t k, char* first, char* second) {
    char result = 0;
    for (uint64_t i = 0; i < k; i++) {
        result = (result + first[i] * second[i]) % 2;
    }

    return result;
}

char** id_matrix(uint64_t n) {
    char** matrix = (char**) malloc(n * sizeof(char*));
    for (uint64_t x = 0; x < n; x++) {
        matrix[x] = (char*) malloc(n * sizeof(char));
        memset(matrix[x], 0, n);
        matrix[x][x] = 1;
    }

    return matrix;
}

char** multiply(uint64_t k1, uint64_t n1, char** mat1,
                uint64_t k2, uint64_t n2, char** mat2) {
    if (n1 != k2) {
        perror("Error: wrong given dimensions!");
        exit(1);
    }
    char** mat2_T = transpose(k2, n2, mat2);
    char** result = (char**) malloc(k1 * sizeof(char));
    for (uint64_t row = 0; row < k1; row++) {
        result[row] = (char*) malloc(n2 * sizeof(char));
        for (uint64_t col2 = 0; col2 < n2; col2++) {
            char res = dot_product(k2, mat1[row], mat2_T[col2]);
            result[row][col2] = res;
        }
    }

    return result;
}

