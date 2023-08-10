#include "math.h"
#include "util.h"

void transpose(uint64_t k, uint64_t n, char** mat, char** output_mat) {
    for (uint64_t row = 0; row < n; row++) {
        for (uint64_t col = 0; col < k; col++) {
            output_mat[row][col] = mat[col][row];
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

void multiply(uint64_t k1, uint64_t n1, char** mat1,
                uint64_t k2, uint64_t n2, char** mat2,
                char** output_mat) {
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


void
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
        uint64_t cur;
        uint64_t i;
        for (i = row; i < n; i++) {
            if (output_mat[row][i] == 1)
                break;
        }
        cur = i;
        char** tmp_mat = alloc_matrix(k, n);

        if (cur != prev) {
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
                add_vectors(n, output_mat[row], output_mat[cur], output_mat[cur]);
            }
        }
    }
}

