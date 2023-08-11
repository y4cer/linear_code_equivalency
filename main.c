#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {

    FILE* in_f = fopen("matrices/G_4_6", "r");
    matrix* in = parse_matrix(in_f);
    matrix* G = alloc_matrix(in->k, in->n);
    uint64_t* perms = (uint64_t*) malloc(2 * G->n * sizeof(uint64_t));
    gaussian_elimination(in, G, perms);

    /* remove_column(G, 1, G_); */
    print_matrix(G);
    /* print_matrix(G_); */
    /* matrix* H = alloc_matrix(G->n - G->k, G->n); */
    /* uint64_t* perms = (uint64_t*) malloc(2*G->n*sizeof(char)); */
    /* build_check_matrix(G, H); */
    /* print_matrix(H); */
    /* matrix* H_T = alloc_matrix(H->n, H->k); */
    /* matrix* res = alloc_matrix(H->k, H_T->n); */
    /* multiply(G, H_T, res); */

    /* print_matrix(res); */
    return 0;
}

