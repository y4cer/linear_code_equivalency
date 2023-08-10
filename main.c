#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {
    /* FILE* m1_f = fopen("matrices/m1", "r"); */
    /* FILE* m2_f = fopen("matrices/m2", "r"); */

    /* matrix* m1 = parse_matrix(m1_f); */
    /* matrix* m2 = parse_matrix(m2_f); */

    /* print_matrix(m1); */
    /* print_matrix(m2); */

    /* matrix* res = alloc_matrix(m1->k, m2->n); */
    /* matrix* m2_T = alloc_matrix(m2->n, m2->k); */
    /* transpose(m2, m2_T); */
    /* multiply(m1, m2_T, res); */

    FILE* G_f = fopen("matrices/G_4_6", "r");
    matrix* G = parse_matrix(G_f);
    matrix* H = alloc_matrix(G->k, G->n);
    build_check_matrix(G, H);
    print_matrix(H);
    matrix* H_T = alloc_matrix(H->n, H->k);
    matrix* res = alloc_matrix(H->k, H_T->n);
    multiply(G, H_T, res);

    print_matrix(res);
    return 0;
}

