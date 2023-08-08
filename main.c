#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 1024
#define DEBUG

/**
  * Parses user input and stores it in matix form.
  * k rows, n columns
  */
char** parse_matrix(uint64_t k, uint64_t n) {
    printf("Enter the matrix (row by row): \n");
    char** matrix = (char**) malloc(k * sizeof(char*));
    for (uint64_t row = 0; row < k; row++) {
        matrix[row] = (char*) malloc(n * sizeof(char));
        char* buf = (char*) malloc(n * sizeof(char));
        fgets(buf, BUFF_SIZE, stdin);
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
    printf("--------");
    for (uint64_t row = 0; row < k; row++) {
        for (uint64_t col = 0; col < n; col++) {
            printf("%d ", mat[row][col]);
        }
        printf("\n");
    }
    printf("--------");
}

void dealloc_matrix(int k, int n, char** mat) {
    for (uint64_t row = 0; row < k; row++) {
        free(mat[row]);
    }
    free(mat);
}

int main() {
    printf("Enter matrix dimensions (rows, columns): ");
    int k, n;
    scanf(" %d %d%*c", &k, &n);

    printf("Matrix dimensions: %d %d\n", k, n);
    char** matrix = parse_matrix(k, n);
    print_matrix(k, n, matrix);

    dealloc_matrix(k, n, matrix);
    return 0;
}

