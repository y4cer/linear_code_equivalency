#include <stdio.h>

#define DEBUG

#include "matrix/util.h"

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

