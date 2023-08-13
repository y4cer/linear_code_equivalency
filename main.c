#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {

    FILE* in_f = fopen("matrices/12_6_3", "r");
    matrix* G = parse_matrix(in_f);

    matrix* R = alloc_matrix(G->k, G->n / 2);
    get_submatrix(G, 0, G->k, G->n / 2, G->n, R);

    matrix* R_T = alloc_matrix(R->n, R->k);
    transpose(R, R_T);

    matrix* RR_T = alloc_matrix(R->k, R_T->n);
    multiply(R, R_T, RR_T);

    matrix* I = alloc_matrix(R->k, R->n);
    id_matrix(I->n, I);

    matrix* X = alloc_matrix(RR_T->k, RR_T->n);
    add_matrices(I, RR_T, X);

    /* print_matrix(X); */
    matrix* E = alloc_matrix(X->k, X->n);
    uint64_t* perms = (uint64_t*) malloc(2 * E->n * sizeof(uint64_t));
    /* gaussian_elimination(X, E, perms); */
    /* print_matrix(E); */

    /* matrix* RE = alloc_matrix(R->k, E->n); */
    /* multiply(R, E, RE); */

    /* matrix* R_m_RE = alloc_matrix(RE->k, RE->n); */
    /* add_matrices(R, RE, R_m_RE); */

    /* matrix* upper = alloc_matrix(R_m_RE->k, I->n + R_m_RE->n); */
    /* matrix* lower = alloc_matrix(E->k, I->n + R_m_RE->n); */
    /* matrix* null_m = alloc_matrix(E->k, I->n); */
    /* concat_matrices_h(I, R_m_RE, upper); */
    /* concat_matrices_h(null_m, E, lower); */

    /* matrix* D = alloc_matrix(I->k + null_m->k, I->n + R_m_RE->n); */
    /* concat_matrices_v(upper, lower, D); */

    /* print_matrix(D); */
    /* /1* print_matrix(R); *1/ */

    /* print_matrix(G); */
    matrix* H = alloc_matrix(G->n - G->k, G->n);
    build_check_matrix(G, H);
    /* print_matrix(H); */

    matrix* M = alloc_matrix(G->n, G->n);
    concat_matrices_v(G, H, M);
    /* print_matrix(M); */

    /* uint64_t* perms = (uint64_t*) malloc(2 * M->n * sizeof(uint64_t)); */
    matrix* M_ = alloc_matrix(M->k, M->n);
    copy_matrix(M, M_);
    gaussian_elimination(M, M_, perms);
    print_matrix(M_);

    matrix* B = alloc_matrix(2, G->n);
    build_check_matrix(M, B);
    /* print_matrix(B); */
    return 0;
}

