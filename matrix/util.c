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

    char** matrix = (char**) malloc(*k * sizeof(char*));

    for (uint64_t row = 0; row < *k; row++) {
        matrix[row] = (char*) malloc(*n * sizeof(char));
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

void print_matrix(int k, int n, char** mat) {
    printf("--------\n");
    printf("k = %d, n = %d\n", k, n);
    for (uint64_t row = 0; row < k; row++) {
        for (uint64_t col = 0; col < n; col++) {
            printf("%d ", mat[row][col]);
        }
        printf("\n");
    }
    printf("--------\n");
}

void dealloc_matrix(int k, int n, char** mat) {
    for (uint64_t row = 0; row < k; row++) {
        free(mat[row]);
    }
    free(mat);
}

