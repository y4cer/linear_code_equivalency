#include <stdio.h>
#include "util.h"

#undef DEBUG

char** parse_matrix(FILE* input_stream, int* k, int* n) {
    char buffer [BUFF_SIZE];

    if (input_stream == stdin) {
        printf("Enter matrix dimensions (rows, columns): ");
        fgets(buffer, BUFF_SIZE, input_stream);

        sscanf(buffer, " %d %d%*c", k, n);

        printf("Matrix dimensions: %d %d\n", *k, *n);
        printf("Enter the matrix (row by row): \n");
    } else {
        fgets(buffer, BUFF_SIZE, input_stream);
        sscanf(buffer, " %d %d%*c", k, n);
    }

    char** matrix = alloc_matrix(*k, *n);

    for (uint64_t row = 0; row < *k; row++) {
        char* buf = (char*) malloc(*n * sizeof(char));
        fgets(buf, BUFF_SIZE, input_stream);
#ifdef DEBUG
        printf("---> %s", buf);
#endif
        char* token = strtok(buf, " ");
        uint64_t col = 0;
        while (token) {
            matrix[row][col] = atoi(token);
            col++;
            token = strtok(NULL, " ");
        }
    }
    return matrix;
}

void print_matrix(uint64_t k, uint64_t n, char** mat) {
    printf("--------\n");
    printf("k = %lu, n = %lu\n", k, n);
    for (uint64_t row = 0; row < k; row++) {
        for (uint64_t col = 0; col < n; col++) {
            printf("%d ", mat[row][col]);
        }
        printf("\n");
    }
    printf("--------\n");
}

char** alloc_matrix(uint64_t k, uint64_t n) {
    char** matrix = (char**) malloc(k * sizeof(char*));
    for (uint64_t row = 0; row < k; row++) {
        matrix[row] = (char*) malloc(n * sizeof(char));
    }
    return matrix;
}

void dealloc_matrix(uint64_t k, uint64_t n, char** mat) {
    for (uint64_t row = 0; row < k; row++) {
        free(mat[row]);
    }
    free(mat);
}

void copy_matrix(uint64_t k, uint64_t n, char** src, char** dest) {
    for (uint64_t row = 0; row < k; row++) {
        memcpy(dest[row], src[row], n);
    }
}

