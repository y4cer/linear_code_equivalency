#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {
    int k, n;

    /* FILE* perm_4_4 = fopen("matrices/perm_4_4", "r"); */
    /* char** perm_4_4_m = parse_matrix(perm_4_4, &k, &n); */
    /* char** id_4 = alloc_matrix(4, 4); */
    /* id_matrix(4, id_4); */

    /* print_matrix(4, 4, perm_4_4_m); */
    /* print_matrix(4, 4, id_4); */

    /* char** mult = alloc_matrix(4, 4); */
    /* multiply(4, 4, perm_4_4_m, 4, 4, perm_4_4_m, mult); */
    /* print_matrix(4, 4, mult); */

    /* dealloc_matrix(4, 4, id_4); */
    /* dealloc_matrix(4, 4, perm_4_4_m); */
    /* dealloc_matrix(4, 4, mult); */

    FILE* G_4_6 = fopen("matrices/G_4_6", "r");
    char** G_4_6_m = parse_matrix(G_4_6, &k, &n);

    print_matrix(k, n, G_4_6_m);
    char** out = alloc_matrix(k, n);
    uint64_t* perms = (uint64_t*) malloc(100 * sizeof(uint64_t));
    copy_matrix(k, n, G_4_6_m, out);
    gaussian_elimination(k, n, G_4_6_m, out, perms);
    /* print_matrix(k, n, out); */
    /* char* res = (char*) malloc(n * sizeof(char)); */
    /* add_vectors(n, out[0], out[1], res); */
    print_matrix(k, n, out);
    /* swap_columns(k, n, 2, 4, out, G_4_6_m); */
    /* print_matrix(k, n, G_4_6_m); */
    return 0;
}

