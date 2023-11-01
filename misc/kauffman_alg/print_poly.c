#include <stdio.h>
#include "kauffman.h"

void print_poly(const struct laurent_polynomial *P)
{
    signed int n, N;

    if (!P)
    {
        puts("0");
        return;
    }

    if (!P->coeffs)
    {
        puts("0");
        return;
    }

    N = P->highest_degree - P->lowest_degree + 1;

    if (P->coeffs[0] == 1)
        printf("q^%d", P->lowest_degree);
    else if (P->coeffs[0] == -1)
        printf("-q^%d", P->lowest_degree);
    else
        printf("%dq^%d", P->coeffs[0], P->lowest_degree);

    for (n = 1; n < N; ++n)
    {
        if (P->coeffs[n] == 0)
            continue;
        else if (P->coeffs[n] == 1)
            printf(" + q^%d", P->lowest_degree + n);
        else if (P->coeffs[n] == -1)
            printf(" - q^%d", P->lowest_degree + n);
        else if (P->coeffs[n] > 1)
            printf(" + %dq^%d", P->coeffs[n], P->lowest_degree + n);
        else
            printf(" - %dq^%d", -P->coeffs[n], P->lowest_degree + n);
    }

    printf("\n");
}
