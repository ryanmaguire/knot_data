/******************************************************************************
 *                                  LICENSE                                   *
 ******************************************************************************
 *  This file is part of knot_data.                                           *
 *                                                                            *
 *  knot_data is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  knot_data is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with knot_data.  If not, see <https://www.gnu.org/licenses/>.       *
 ******************************************************************************/
#include <stdlib.h>
#include "kauffman.h"

struct laurent_polynomial normalized_kauffman_bracket(const struct knot *K)
{
    unsigned int n, weight, n_circles, n_elements, shift;
    const struct crossing_indices * const ind = get_indices(K);
    struct laurent_polynomial out;

    signed int kauffman_coeffs[MAX_POLY_SIZE];

    for (n = 0U; n < MAX_POLY_SIZE; ++n)
        kauffman_coeffs[n] = 0U;

    for (n = 0; n < (1U << K->number_of_crossings); ++n)
    {
        weight = hamming_weight(n);
        n_circles = circle_count(K, ind, n);
        add_normalized_kauffman_summand(n_circles, weight,
                                        DEGREE_SHIFT, kauffman_coeffs);
    }

    out.lowest_degree = MIN_DEGREE;
    out.highest_degree = MAX_DEGREE;

    while (kauffman_coeffs[out.lowest_degree + DEGREE_SHIFT] == 0)
        out.lowest_degree++;

    while (kauffman_coeffs[out.highest_degree + DEGREE_SHIFT] == 0)
        out.highest_degree--;

    n_elements = (unsigned int)(out.highest_degree - out.lowest_degree) + 1U;
    shift = (unsigned int)(DEGREE_SHIFT + out.lowest_degree);
    out.coeffs = malloc(sizeof(*out.coeffs) * n_elements);

    for (n = 0U; n < n_elements; ++n)
        out.coeffs[n] = kauffman_coeffs[n + shift];

    return out;
}
