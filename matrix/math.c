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

char dot_product(uint64_t k, char* first, char* second) {
    char result = 0;
    for (uint64_t i = 0; i < k; i++) {
        result = (result + first[i] * second[i]) % 2;
    }

    return result;
}

void add_vectors(uint64_t k, char* first, char* second, char* output_vec) {
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
            char res = dot_product(mat2->k, mat1->mat[row], mat2_T->mat[col2]);
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
    char* tmp = malloc(input_mat->n * sizeof(char));
    memcpy(tmp, output_mat->mat[second], input_mat->n);
    memcpy(output_mat->mat[second], input_mat->mat[first], input_mat->n);
    memcpy(input_mat->mat[first], output_mat->mat[second], input_mat->n);
}

int
gaussian_elimination(
    matrix* input_mat,
    matrix* output_mat,
    uint64_t* permutations)
{
    copy_matrix(input_mat, output_mat);
    uint64_t perm_idx = 0;
    uint64_t null_rows = 0;
    for (uint64_t row = 0; row < input_mat->k; row++) {
        uint64_t prev = row;
        uint64_t cur = prev;
        uint64_t i;
        for (i = row; i < input_mat->n; i++) {
            if (output_mat->mat[row][i] == 1)
                break;
            if (i == input_mat->n - 1) {
                null_rows++;
            }
        }
        cur = i;

        if (cur != prev) {
            matrix* tmp_mat = alloc_matrix(input_mat->k, input_mat->n);
            swap_columns(output_mat, cur, prev, tmp_mat);
            copy_matrix(tmp_mat, output_mat);
            dealloc_matrix(tmp_mat);
        }

        for (uint64_t work_row = row + 1; work_row < output_mat->k; work_row++) {
            if (output_mat->mat[work_row][prev]) {
                add_vectors(
                        output_mat->n,
                        output_mat->mat[row],
                        output_mat->mat[work_row],
                        output_mat->mat[work_row]
                );
            }
        }
        if (cur != prev) {
            permutations[perm_idx++] = prev;
            permutations[perm_idx++] = cur;
        }
    }

    // Back substitution
    for (int64_t row = output_mat->k - 1; row >= 0; row--) {
        for (int64_t cur = row - 1; cur >= 0; cur--) {
            if (output_mat->mat[cur][row]) {
                add_vectors(
                        output_mat->n,
                        output_mat->mat[row],
                        output_mat->mat[cur],
                        output_mat->mat[cur]
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
        memcpy(output->mat[row], left->mat[row], right->n);
        memcpy(output->mat[row] + right->n, right->mat[row], left->n);
    }
}

void concat_matrices_v(matrix* upper, matrix* lower, matrix* output) {
    for (uint64_t row = 0; row < upper->k; row++) {
        memcpy(output->mat[row], upper->mat[row], upper->n);
    }
    for (uint64_t row = upper->k; row < upper->k + lower->k; row++) {
        memcpy(output->mat[row], upper->mat[row - upper->k], upper->n);
    }
}

int build_check_matrix(matrix* input_mat, matrix* H) {
    uint64_t n = input_mat->n;
    uint64_t k = input_mat->k;
    uint64_t* perms = (uint64_t*) malloc(2 * input_mat->n * sizeof(uint64_t));
    matrix* G = alloc_matrix(k, n);
    gaussian_elimination(input_mat, G, perms);
    print_matrix(G);
    // Assuming that left part of output_mat is k*k identity matrix
    matrix* A = alloc_matrix(k, n - k);
    get_submatrix(G, 0, k, k, n, A);
    print_matrix(A);
    matrix* A_T = alloc_matrix(n - k, k);
    transpose(A, A_T);
    matrix* I = alloc_matrix(n - k, n - k);
    id_matrix(n - k, I);
    concat_matrices_h(A_T, I, H);

    return 0;
}

