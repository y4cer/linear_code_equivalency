#include "math.h"
#include "util.h"

void transpose(uint64_t k, uint64_t n, char** mat, char** output_mat) {
    for (int64_t row = n - 1; row >= 0; row--) {
        for (uint64_t col = 0; col < k; col++) {
            output_mat[n - row - 1][col] = mat[col][row];
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

void id_matrix(uint64_t n, char** output_mat) {
    for (uint64_t x = 0; x < n; x++) {
         output_mat[x][x] = 1;
    }
}

void
multiply(
    uint64_t k1,
    uint64_t n1,
    char** mat1,
    uint64_t k2,
    uint64_t n2,
    char** mat2,
    char** output_mat)
{
    if (n1 != k2) {
        perror("Error: wrong given dimensions!");
        exit(1);
    }
    char** mat2_T = alloc_matrix(n2, k2);
    transpose(k2, n2, mat2, mat2_T);
    for (uint64_t row = 0; row < k1; row++) {
        for (uint64_t col2 = 0; col2 < n2; col2++) {
            char res = dot_product(k2, mat1[row], mat2_T[col2]);
            output_mat[row][col2] = res;
        }
    }
    dealloc_matrix(n2, k2, mat2_T);
}

void
swap_columns(
    uint64_t k,
    uint64_t n,
    uint64_t first,
    uint64_t second,
    char** mat,
    char** output_mat)
{
    copy_matrix(k, n, mat, output_mat);
    for (uint64_t row = 0; row < k; row++) {
        output_mat[row][second] = mat[row][first];
        output_mat[row][first] = mat[row][second];
    }
}


int
gaussian_elimination(
        uint64_t k,
        uint64_t n,
        char** mat,
        char** output_mat,
        uint64_t* permutations)
{
    copy_matrix(k, n, mat, output_mat);
    uint64_t perm_idx = 0;
    for (uint64_t row = 0; row < k; row++) {
        uint64_t prev = row;
        uint64_t cur = prev;
        uint64_t i;
        for (i = row; i < n; i++) {
            if (output_mat[row][i] == 1)
                break;
            if (i == n - 1) {
                perror("Given matrix does not have full rank");
                return 1;
            }
        }
        cur = i;

        if (cur != prev) {
            char** tmp_mat = alloc_matrix(k, n);
            swap_columns(k, n, cur, prev, output_mat, tmp_mat);
            copy_matrix(k, n, tmp_mat, output_mat);
            dealloc_matrix(k, n, tmp_mat);
        }

        for (uint64_t work_row = row + 1; work_row < k; work_row++) {
            if (output_mat[work_row][prev]) {
                add_vectors(
                        n,
                        output_mat[row],
                        output_mat[work_row],
                        output_mat[work_row]
                );
            }
        }
        if (cur != prev) {
            permutations[perm_idx++] = prev;
            permutations[perm_idx++] = cur;
        }
    }

    // Back substitution
    for (int64_t row = k - 1; row >= 0; row--) {
        for (int64_t cur = row - 1; cur >= 0; cur--) {
            if (output_mat[cur][row]) {
                add_vectors(
                        n,
                        output_mat[row],
                        output_mat[cur],
                        output_mat[cur]
                );
            }
        }
    }
    return 0;
}

void
get_submatrix(
    uint64_t k,
    uint64_t n,
    uint64_t start_row,
    uint64_t end_row,
    uint64_t start_col,
    uint64_t end_col,
    char** mat,
    char** output_mat)
{
    for (uint64_t row = start_row; row < end_row; row++) {
        for (uint64_t col = start_col; col < end_col; col++) {
            output_mat[row - start_row][col - start_col] = mat[row][col];
        }
    }
}

void
concat_matrices_h(
    uint64_t k,
    uint64_t n1,
    uint64_t n2,
    char** mat1,
    char** mat2,
    char** output_mat)
{
    for (uint64_t row = 0; row < k; row++) {
        memcpy(output_mat[row], mat1[row], n1);
        memcpy(output_mat[row] + n1, mat2[row], n2);
    }
}

void
concat_matrices_v(
    uint64_t n,
    uint64_t k1,
    uint64_t k2,
    char** mat1,
    char** mat2,
    char** output_mat)
{
    for (uint64_t row = 0; row < k1; row++) {
        memcpy(output_mat[row], mat1[row], n);
    }
    for (uint64_t row = k1; row < k1 + k2; row++) {
        memcpy(output_mat[row], mat1[row - k1], n);
    }
}

int build_check_matrix(uint64_t k, uint64_t n, char** mat, char** H) {
    uint64_t* perms = (uint64_t*) malloc(2 * n * sizeof(uint64_t));
    char** G = alloc_matrix(k, n);
    gaussian_elimination(k, n, mat, G, perms);
    print_matrix(k, n, G);
    // Assuming that left part of output_mat is k*k identity matrix
    char** A = alloc_matrix(k, n - k);
    get_submatrix(k, n, 0, k, k, n, G, A);
    char** A_T = alloc_matrix(n - k, k);
    transpose(k, n - k, A, A_T);
    char** I = alloc_matrix(n - k, n - k);
    id_matrix(n - k, I);
    concat_matrices_h(n - k, k, n - k, A_T, I, H);

    return 0;
}

