#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {

    FILE* in_f = fopen("matrices/12_6_3", "r");
    matrix* G = parse_matrix(in_f);
    print_matrix(G);

    matrix* R = alloc_matrix(G->k, G->n / 2);
    get_submatrix(G, 0, G->k, G->n / 2, G->n, R);

    matrix* R_T = alloc_matrix(R->n, R->k);
    transpose(R, R_T);

    matrix* Id = alloc_matrix(R->k, R->k);
    id_matrix(Id->k, Id);

    matrix* H = alloc_matrix(G->n - G->k, G->n);
    concat_matrices_h(R_T, Id, H);
    /* print_matrix(H); */

    code_equivalence(G, G);
    /* matrix* M = alloc_matrix(G->n, G->n); */
    /* concat_matrices_v(G, H, M); */
    /* print_matrix(M); */

    /* matrix* D = alloc_matrix(M->k, M->n); */
    /* matrix* S = alloc_matrix(M->k, M->k); */

    /* gaussian_elimination(M, D, S); */
    /* print_matrix(D); */
    /* print_matrix(S); */

    /* matrix* Id_m_D = alloc_matrix(D->k, D->n); */
    /* dealloc_matrix(Id); */
    /* Id = alloc_matrix(D->k, D->n); */
    /* id_matrix(D->k, Id); */
    /* add_matrices(Id, D, Id_m_D); */
    /* print_matrix(Id_m_D); */

    /* matrix* Id_m_D_T = alloc_matrix(Id_m_D->n, Id_m_D->k); */
    /* transpose(Id_m_D, Id_m_D_T); */
    /* matrix* B = remove_null_rows(Id_m_D_T); */
    /* print_matrix(B); */

    return 0;
}

