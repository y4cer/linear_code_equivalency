#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {
    int k, n;

    FILE* G_4_6 = fopen("matrices/G_4_6", "r");
    char** G_4_6_m = parse_matrix(G_4_6, &k, &n);

    print_matrix(k, n, G_4_6_m);

    char** H = alloc_matrix(n - k, n);
    build_check_matrix(k, n, G_4_6_m, H);
    print_matrix(n - k, n, H);

    dealloc_matrix(n - k, n, H);
    dealloc_matrix(k, n, G_4_6_m);
    return 0;
}

