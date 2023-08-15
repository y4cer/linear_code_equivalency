#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {

    /* FILE* in_f = fopen("matrices/12_6_3", "r"); */
    /* matrix* G = parse_matrix(in_f); */
    /* matrix* G_ = alloc_matrix(G->k, G->n); */
    /* matrix* tmp = alloc_matrix(G->k, G->n); */
    /* swap_columns(G, 0, 1, G_); */
    /* copy_matrix(G_, tmp); */
    /* swap_columns(tmp, 3, 5, G_); */

    /* print_matrix(G); */
    /* print_matrix(G_); */
    /* code_equivalence(G, G_); */

    uint64_t k = 6;
    uint64_t n = 12;

    srand(time(NULL));

    matrix* G = get_random_G(k, n);
    matrix* G_ = get_random_G(k, n);

    /* dealloc_matrix(G); */
    /* dealloc_matrix(G_); */
    /* dealloc_matrix(tmp); */
    /* fclose(in_f); */
    /* dealloc_matrix(G_); */

    code_equivalence(G, G_);

    dealloc_matrix(G);
    dealloc_matrix(G_);
    return 0;
}

