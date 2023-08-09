#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {
    int k, n;

    FILE* perm_4_4 = fopen("matrices/perm_4_4", "r");
    char** perm_4_4_m = parse_matrix(perm_4_4, &k, &n);
    char** id_4 = id_matrix(4);

    print_matrix(4, 4, perm_4_4_m);
    print_matrix(4, 4, id_4);

    char** mult = multiply(4, 4, perm_4_4_m, 4, 4, perm_4_4_m);
    print_matrix(4, 4, mult);

    dealloc_matrix(4, 4, id_4);
    return 0;
}

