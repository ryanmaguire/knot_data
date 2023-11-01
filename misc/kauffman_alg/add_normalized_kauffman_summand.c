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
#include "kauffman.h"
#include "q_plus_q_inverse_powers.h"

void
add_normalized_kauffman_summand(unsigned int cycles,
                                unsigned int weight,
                                unsigned int degree_shift,
                                signed int *coeffs_array)
{
    unsigned int n;
    const unsigned int shift = degree_shift - cycles + weight + 1U;
    const unsigned int number_of_terms = 2U*cycles - 1U;
    const int * const coeffs = q_qinv_powers[cycles - 1U];

    if (weight & 1U)
        for (n = 0; n < number_of_terms; ++n)
            coeffs_array[n + shift] -= coeffs[n];

    else
        for (n = 0; n < number_of_terms; ++n)
            coeffs_array[n + shift] += coeffs[n];
}
