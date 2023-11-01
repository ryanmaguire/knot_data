#include "kauffman.h"
#include "q_plus_q_inverse_powers.h"

void
add_kauffman_summand(unsigned int cycles,
                     unsigned int weight,
                     unsigned int degree_shift,
                     signed int *coeffs_array)
{
    unsigned int n;
    const unsigned int shift = degree_shift - cycles + weight;
    const unsigned int number_of_terms = 2U*cycles + 1U;
    const int * const coeffs = q_qinv_powers[cycles];

    if (weight & 1U)
        for (n = 0; n < number_of_terms; ++n)
            coeffs_array[n + shift] -= coeffs[n];

    else
        for (n = 0; n < number_of_terms; ++n)
            coeffs_array[n + shift] += coeffs[n];
}
