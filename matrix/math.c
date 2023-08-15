#include "math.h"

void transpose(matrix* input_mat, matrix* out) {
    out->n = input_mat->k;
    out->k = input_mat->n;
    for (int64_t row = 0; row < input_mat->n; row++) {
        for (uint64_t col = 0; col < input_mat->k; col++) {
            out->mat[row][col] = input_mat->mat[col][row];
        }
    }
}

uint8_t dot_product(uint64_t k, uint8_t* first, uint8_t* second) {
    uint8_t result = 0;
    for (uint64_t i = 0; i < k; i++) {
        result = (result + first[i] * second[i]) % 2;
    }

    return result;
}

void add_vectors(uint64_t k, uint8_t* first, uint8_t* second, uint8_t* output_vec) {
    for (uint64_t i = 0; i < k; i++) {
        output_vec[i] = (first[i] + second[i]) % 2;
    }
}

void id_matrix(uint64_t n, matrix* output_mat) {
    for (uint64_t x = 0; x < n; x++) {
         output_mat->mat[x][x] = 1;
    }
}

void multiply(matrix* mat1, matrix* mat2, matrix* output_mat) {
    if (mat1->n != mat2->k) {
        perror("Error: wrong given dimensions!");
        exit(1);
    }
    matrix* mat2_T = alloc_matrix(mat2->n, mat2->k);
    transpose(mat2, mat2_T);
    for (uint64_t row = 0; row < mat1->k; row++) {
        for (uint64_t col2 = 0; col2 < mat2->n; col2++) {
            uint8_t res = dot_product(mat2->k, mat1->mat[row], mat2_T->mat[col2]);
            output_mat->mat[row][col2] = res;
        }
    }
    dealloc_matrix(mat2_T);
}

void
swap_columns(
    matrix* input_mat,
    uint64_t first,
    uint64_t second,
    matrix* output_mat)
{
    copy_matrix(input_mat, output_mat);
    for (uint64_t row = 0; row < input_mat->k; row++) {
        output_mat->mat[row][second] = input_mat->mat[row][first];
        output_mat->mat[row][first] = input_mat->mat[row][second];
    }
}

void
swap_rows(
    matrix* input_mat,
    uint64_t first,
    uint64_t second,
    matrix* output_mat)
{
    copy_matrix(input_mat, output_mat);
    memcpy(output_mat->mat[second], input_mat->mat[first], input_mat->n);
    memcpy(output_mat->mat[first], input_mat->mat[second], input_mat->n);
}

int
gaussian_elimination(
    matrix* M,
    matrix* D,
    matrix* S,
    matrix* P)
{
    copy_matrix(M, D);
    id_matrix(S->k, S);
    id_matrix(P->k, P);

    for (uint64_t row = 0; row < D->k; row++) {
        if (D->mat[row][row] == 0) {
            int found = 0;
            uint64_t swap_row;
            for (swap_row = row + 1; swap_row < D->k; swap_row++) {
                if (D->mat[swap_row][row]) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                matrix* tmp_mat = alloc_matrix(D->k, D->n);
                swap_rows(D, row, swap_row, tmp_mat);
                copy_matrix(tmp_mat, D);
                dealloc_matrix(tmp_mat);

                tmp_mat = alloc_matrix(S->k, S->n);
                swap_rows(S, row, swap_row, tmp_mat);
                copy_matrix(tmp_mat, S);
                dealloc_matrix(tmp_mat);

                tmp_mat = alloc_matrix(P->k, P->n);
                swap_rows(P, row, swap_row, tmp_mat);
                copy_matrix(tmp_mat, P);
                dealloc_matrix(tmp_mat);
            }
        }
        for (uint64_t cur_row = row + 1; cur_row < D->k; cur_row++) {
            if (D->mat[cur_row][row] == 1) {
                add_vectors(
                    D->n,
                    D->mat[row],
                    D->mat[cur_row],
                    D->mat[cur_row]
                );
                add_vectors(
                    S->n,
                    S->mat[row],
                    S->mat[cur_row],
                    S->mat[cur_row]
                );
            }
        }
    }
    for (int64_t row = D->k - 1; row >= 0; row--) {
        for (int64_t cur_row = row - 1; cur_row >= 0; cur_row--) {
            if (D->mat[cur_row][row] == 1) {
                add_vectors(
                    D->n,
                    D->mat[row],
                    D->mat[cur_row],
                    D->mat[cur_row]
                );
                add_vectors(
                    S->n,
                    S->mat[row],
                    S->mat[cur_row],
                    S->mat[cur_row]
                );
            }
        }
    }
    return 0;
}

void
get_submatrix(
    matrix* input_mat,
    uint64_t start_row,
    uint64_t end_row,
    uint64_t start_col,
    uint64_t end_col,
    matrix* output_mat)
{
    for (uint64_t row = start_row; row < end_row; row++) {
        for (uint64_t col = start_col; col < end_col; col++) {
            output_mat->mat[row - start_row][col - start_col] =
                input_mat->mat[row][col];
        }
    }
}

void concat_matrices_h(matrix* left, matrix* right, matrix* output) {
    for (uint64_t row = 0; row < right->k; row++) {
        memcpy(output->mat[row], left->mat[row], left->n);
        memcpy(output->mat[row] + left->n, right->mat[row], right->k);
    }
}

void concat_matrices_v(matrix* upper, matrix* lower, matrix* output) {
    for (uint64_t row = 0; row < upper->k; row++) {
        memcpy(output->mat[row], upper->mat[row], upper->n);
    }
    for (uint64_t row = upper->k; row < upper->k + lower->k; row++) {
        memcpy(output->mat[row], lower->mat[row - upper->k], lower->n);
    }
}

int build_check_matrix(matrix* input_mat, matrix* H) {
    uint64_t n = input_mat->n;
    uint64_t k = input_mat->k;
    matrix* G = alloc_matrix(k, n);
    matrix* S = alloc_matrix(k, k);
    matrix* P = alloc_matrix(k, k);
    gaussian_elimination(input_mat, G, S, P);

    // Assuming that left part of output_mat is k*k identity matrix
    matrix* R = alloc_matrix(k, n - k);
    get_submatrix(G, 0, k, k, n, R);
    matrix* R_T = alloc_matrix(n - k, k);
    transpose(R, R_T);
    matrix* I = alloc_matrix(n - k, n - k);
    id_matrix(n - k, I);
    concat_matrices_h(R_T, I, H);

    dealloc_matrix(G);
    dealloc_matrix(S);
    dealloc_matrix(P);
    dealloc_matrix(R);
    dealloc_matrix(R_T);
    dealloc_matrix(I);

    return 0;
}

void remove_column(matrix* input_mat, uint64_t column, matrix* output_mat) {
    for (uint64_t row = 0; row < input_mat->k; row++) {
        for (uint64_t col = 0; col < input_mat->n; col++) {
            if (col == column) continue;
            uint64_t idx_remove = col > column;
            output_mat->mat[row][col - idx_remove] = input_mat->mat[row][col];
        }
    }
}

void remove_row(matrix* input_mat, uint64_t remove_row, matrix* output_mat) {
    for (uint64_t row = 0; row < input_mat->k; row++) {
        if (row == remove_row) continue;
        uint64_t idx_remove = row > remove_row;
        memcpy(output_mat->mat[row - idx_remove], input_mat->mat[row], input_mat->n);
    }
}

void
remove_rows(
        matrix* input_mat,
        uint64_t* rows_to_remove,
        matrix* output_mat)
{
    uint64_t idx_remove = 0;
    for (uint64_t row = 0; row < input_mat->k; row++) {
        if (rows_to_remove[row]) {
            idx_remove++;
            continue;
        }
        memcpy(output_mat->mat[row - idx_remove], input_mat->mat[row], input_mat->n);
    }
}

matrix* remove_null_rows(matrix* input_mat) {
    uint64_t* rows_to_remove =
        (uint64_t*) calloc(input_mat->k, sizeof(uint64_t));
    uint64_t remove_rows_n = 0;
    for (uint64_t row = 0; row < input_mat->k; row++) {
        uint64_t null_r = 1;
        for (uint64_t col = 0; col < input_mat->n; col++) {
            if (input_mat->mat[row][col] == 1) {
                null_r = 0;
                break;
            }
        }
        if (null_r) {
            rows_to_remove[row] = 1;
            remove_rows_n++;
        }
    }
    matrix* output_mat = alloc_matrix(input_mat->k - remove_rows_n, input_mat->n);
    remove_rows(input_mat, rows_to_remove, output_mat);
    free(rows_to_remove);
    return output_mat;
}

void add_matrices(matrix* first, matrix* second, matrix* output) {
    for (uint64_t row = 0; row < first->k; row++) {
        for (uint64_t col = 0; col < first->n; col++) {
            output->mat[row][col] =
                (first->mat[row][col] + second->mat[row][col]) % 2;
        }
    }
}

matrix* get_hull(matrix* G) {

    matrix* H = alloc_matrix(G->n - G->k, G->n);
    build_check_matrix(G, H);

    matrix* M = alloc_matrix(H->k + G->k, G->n);
    concat_matrices_v(G, H, M);

    matrix* Id = alloc_matrix(M->n, M->n);
    id_matrix(Id->n, Id);

    matrix* D = alloc_matrix(M->k, M->n);
    matrix* S = alloc_matrix(M->k, M->k);
    matrix* P = alloc_matrix(M->k, M->k);

    gaussian_elimination(M, D, S, P);

    matrix* Id_m_D = alloc_matrix(D->k, D->n);
    dealloc_matrix(Id);
    Id = alloc_matrix(D->k, D->n);
    id_matrix(D->k, Id);
    add_matrices(Id, D, Id_m_D);

    matrix* tmp = alloc_matrix(Id_m_D->k, Id_m_D->n);
    multiply(P, Id_m_D, tmp);

    copy_matrix(tmp, Id_m_D);

    matrix* Id_m_D_T = alloc_matrix(Id_m_D->n, Id_m_D->k);
    transpose(Id_m_D, Id_m_D_T);

    // B is the support of the original code generated by G
    matrix* B = remove_null_rows(Id_m_D_T);

    dealloc_matrix(H);
    dealloc_matrix(M);
    dealloc_matrix(Id);
    dealloc_matrix(D);
    dealloc_matrix(S);
    dealloc_matrix(P);
    dealloc_matrix(tmp);
    dealloc_matrix(Id_m_D);
    dealloc_matrix(Id_m_D_T);

    return B;
}

matrix* all_linear_combinations(matrix* B) {
    uint64_t rows = B->k;
    matrix* C = alloc_matrix((1ull << rows), B->n);
    // for each bitmask
    for (uint64_t row = 0; row < C->k; row++) {
        uint64_t tmp = row;
        // for each bit in bitmask
        for (uint64_t i = 0; i < B->n; i++) {
            if (tmp & 1) {
                add_vectors(B->n, B->mat[i], C->mat[row], C->mat[row]);
            }
            tmp >>= 1;
        }
    }
    return C;
}

matrix* get_code_spectre(matrix* G) {
    matrix* spectre = alloc_matrix(G->n, G->n);
    for (uint64_t i = 0; i < G->n; i++) {
        matrix* Gi = alloc_matrix(G->k, G->n - 1);
        remove_column(G, i, Gi);
        matrix* B = get_hull(Gi);
        matrix* C = all_linear_combinations(B);
        for (uint64_t row = 0; row < C->k; row++) {
            uint64_t sum = 0;
            for (uint64_t col = 0; col < C->n; col++) {
                sum += C->mat[row][col];
            }
            spectre->mat[i][sum]++;
        }
        dealloc_matrix(Gi);
        dealloc_matrix(B);
        dealloc_matrix(C);
    }
    return spectre;
}

int equal_vectors(uint64_t k, uint8_t* first, uint8_t* second) {
    for (uint64_t i = 0; i < k; i++) {
        if (first[i] != second[i]) {
            return 0;
        }
    }
    return 1;
}

int equal_vectors_64(uint64_t k, uint8_t* first, uint64_t* second) {
    for (uint64_t i = 0; i < k; i++) {
        if (first[i] != second[i]) {
            return 0;
        }
    }
    return 1;
}

int64_t find(uint64_t k, uint64_t n, uint8_t* vector, uint64_t** M) {
    for (int64_t row = 0; row < k; row++) {
        if (equal_vectors_64(n, vector, M[row])) {
            return row;
        }
    }
    return -1;
}


int code_equivalence(matrix *G, matrix *G_) {
    matrix* spectre_1 = get_code_spectre(G);
    matrix* spectre_2 = get_code_spectre(G_);

    /* print_matrix(spectre_1); */
    /* print_matrix(spectre_2); */

    // Codes are not equivalent for sure if their generating matrices are not
    // equal size
    if ((spectre_1->k != spectre_2->k) || (spectre_1->n != spectre_2->n)) {
        return 0;
    }

    // permutations has the following structure:
    // spectre (spectre->n elements) | # of pemutation list (1 element) | permutation list (defined in previous section)
    uint64_t** permutations = malloc(spectre_1->k * sizeof(uint64_t*));

    uint64_t* map = malloc(spectre_1->n * sizeof(uint64_t));
    int64_t to_use = -1;

    for (uint64_t sp1 = 0; sp1 < spectre_1->k; sp1++) {
        int64_t res;
        if ((res = find(to_use + 1, spectre_1->n, spectre_1->mat[sp1], permutations)) == -1) {
            map[sp1] = ++to_use;
            permutations[to_use] = calloc(2 * spectre_1->n + 1, sizeof(uint64_t));

            // Copy first part of the permutation
            for (uint64_t i = 0; i < spectre_1->n; i++) {
                permutations[to_use][i] = spectre_1->mat[sp1][i];
            }
            for (uint64_t sp2 = 0; sp2 < spectre_2->k; sp2++) {
                if (equal_vectors(spectre_1->n, spectre_1->mat[sp1], spectre_2->mat[sp2])) {
                    uint64_t* idx = &permutations[map[sp1]][spectre_1->n];
                    permutations[map[sp1]][spectre_1->n + ++(*idx)] = sp2;
                }
            }
        } else {
            map[sp1] = res;
        }
    }

    for (uint64_t row = 0; row < to_use + 1; row++) {
        for (uint64_t col = 0; col < spectre_1->n * 2 + 1; col++) {
            printf("%lu ", permutations[row][col]);
            if (col == spectre_1->n - 1)
                printf(" | ");
            if (col == spectre_1->n)
                printf(" | ");
        }
        printf("\n");
    }

    dealloc_matrix(spectre_1);
    dealloc_matrix(spectre_2);
    return 0;
}

matrix* get_random_G(uint64_t k, uint64_t n) {
    if (n < k) {
        perror("Wrong dimensions!");
        exit(1);
    }
    matrix* G_l = alloc_matrix(k, k);
    id_matrix(k, G_l);
    matrix* G_r = alloc_matrix(k, n - k);

    for (uint64_t row = 0; row < G_r->k; row++) {
        for (uint64_t col = 0; col < G_r->n; col++) {
            uint8_t r = rand() % 2;
            G_r->mat[row][col] = r;
        }
    }

    matrix* G = alloc_matrix(k, n);
    concat_matrices_h(G_l, G_r, G);

    dealloc_matrix(G_l);
    dealloc_matrix(G_r);

    return G;
}

