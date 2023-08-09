#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {
    int k, n;
    FILE* id_4_4 = fopen("id_4_4", "r");
    char** id_4_4_m = parse_matrix(id_4_4, &k, &n);

    FILE* perm_4_4 = fopen("perm_4_4", "r");
    char** perm_4_4_m = parse_matrix(perm_4_4, &k, &n);

    print_matrix(k, n, id_4_4_m);
    print_matrix(k, n, perm_4_4_m);

    dealloc_matrix(k, n, id_4_4_m);
    dealloc_matrix(k, n, perm_4_4_m);
    return 0;
}

