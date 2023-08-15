#include <stdio.h>
#include "util.h"

#undef DEBUG

matrix* parse_matrix(FILE* input_stream) {

    matrix* result = (matrix*) malloc(sizeof(matrix));
    uint8_t buffer [BUFF_SIZE];

    if (input_stream == stdin) {
        printf("Enter matrix dimensions (rows, columns): ");
        fgets(buffer, BUFF_SIZE, input_stream);

        sscanf(buffer, " %lu %lu%*c", &result->k, &result->n);

        printf("Matrix dimensions: %lu %lu\n", result->k, result->n);
        printf("Enter the matrix (row by row): \n");
    } else {
        fgets(buffer, BUFF_SIZE, input_stream);
        sscanf(buffer, " %lu %lu%*c", &result->k, &result->n);
    }
    result->mat = (uint8_t**) malloc(result->k * sizeof(uint8_t*));

    for (uint64_t row = 0; row < result->k; row++) {
        uint64_t kk = 1024;
        result->mat[row] = (uint8_t*) malloc(kk);
        uint8_t* buf = (uint8_t*) malloc(kk);
        fgets(buf, BUFF_SIZE, input_stream);
        uint8_t* token = strtok(buf, " ");
        uint64_t col = 0;
        while (token) {
            result->mat[row][col] = atoi(token);
            col++;
            token = strtok(NULL, " ");
        }
        free(buf);
    }
    return result;
}

void print_matrix(matrix* mat) {
    printf("--------\n");
    printf("k = %lu, n = %lu\n", mat->k, mat->n);
    for (uint64_t row = 0; row < mat->k; row++) {
        for (uint64_t col = 0; col < mat->n; col++) {
            printf("%d ", mat->mat[row][col]);
        }
        printf("\n");
    }
    printf("--------\n");
}

matrix* alloc_matrix(uint64_t k, uint64_t n) {
    matrix* result = malloc(sizeof(matrix));
    result->mat = (uint8_t**) malloc(k * sizeof(uint8_t*));
    result->n = n;
    result->k = k;
    for (uint64_t row = 0; row < k; row++) {
        result->mat[row] = (uint8_t*) calloc(n, sizeof(uint8_t));
    }
    return result;
}

void dealloc_matrix(matrix* mat) {
    for (uint64_t row = 0; row < mat->k; row++) {
        free(mat->mat[row]);
    }
    free(mat->mat);
    mat->k = 0;
    mat->n = 0;
    free(mat);
}

void copy_matrix(matrix* src, matrix* dest) {
    dest->k = src->k;
    dest->n = src->n;
    for (uint64_t row = 0; row < src->k; row++) {
        memcpy(dest->mat[row], src->mat[row], src->n);
    }
}

