#include <stdio.h>

#include "matrix/util.h"
#include "matrix/math.h"

int main() {

    FILE* in_f = fopen("matrices/G_10_20", "r");
    matrix* G = parse_matrix(in_f);
    print_matrix(G);

    code_equivalence(G, G);

    return 0;
}

