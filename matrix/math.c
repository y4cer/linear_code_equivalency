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
    memcpy(output_mat->mat[second], input_mat->mat[first], input_mat->n);
    memcpy(output_mat->mat[first], input_mat->mat[second], input_mat->n);
}

int
gaussian_elimination(
    matrix* input_mat,
    matrix* output_mat,
    uint64_t* permutations)
{
    copy_matrix(input_mat, output_mat);
    for (uint64_t row = 0; row < output_mat->k; row++) {
        if (output_mat->mat[row][row] == 0) {
            int found = 0;
            uint64_t swap_row;
            for (swap_row = row + 1; swap_row < output_mat->k; swap_row++) {
                if (output_mat->mat[swap_row][row]) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                matrix* tmp_mat = alloc_matrix(output_mat->k, output_mat->n);
                swap_rows(output_mat, row, swap_row, tmp_mat);
                copy_matrix(tmp_mat, output_mat);
                dealloc_matrix(tmp_mat);
            }
        }
        for (uint64_t cur_row = row + 1; cur_row < output_mat->k; cur_row++) {
            if (output_mat->mat[cur_row][row] == 1) {
                add_vectors(
                    output_mat->n,
                    output_mat->mat[row],
                    output_mat->mat[cur_row],
                    output_mat->mat[cur_row]
                );
            }
        }
    }
    /* print_matrix(output_mat); */
    for (int64_t row = output_mat->k - 1; row >= 0; row--) {
        for (int64_t cur_row = row - 1; cur_row >= 0; cur_row--) {
            if (output_mat->mat[cur_row][row] == 1) {
                add_vectors(
                    output_mat->n,
                    output_mat->mat[row],
                    output_mat->mat[cur_row],
                    output_mat->mat[cur_row]
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
    uint64_t* perms = (uint64_t*) malloc(2 * input_mat->n * sizeof(uint64_t));
    matrix* G = alloc_matrix(k, n);
    gaussian_elimination(input_mat, G, perms);
    /* print_matrix(G); */
    // Assuming that left part of output_mat is k*k identity matrix
    matrix* A = alloc_matrix(k, n - k);
    get_submatrix(G, 0, k, k, n, A);
    matrix* A_T = alloc_matrix(n - k, k);
    transpose(A, A_T);
    matrix* I = alloc_matrix(n - k, n - k);
    id_matrix(n - k, I);
    concat_matrices_h(A_T, I, H);

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


int code_equivalence(matrix *G, matrix *G_) {
    for (uint64_t i = 0; i < G->n; i++) {

    }
    return 0;
}

