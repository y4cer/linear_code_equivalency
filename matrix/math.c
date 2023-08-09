#include "math.h"

char** multiply(uint64_t k1, uint64_t n1, char** mat1,
                uint64_t k2, uint64_t n2, char** mat2) {
    if (n1 != k2) {
        perror("Error: wrong given dimensions!");
        exit(1);
    }

    char** result = (char**) malloc(k1 * sizeof(char));
    /* for (uint64_t row = 0; row < k1; ) */
}

