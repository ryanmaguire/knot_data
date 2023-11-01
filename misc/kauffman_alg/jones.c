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

struct laurent_polynomial jones(const struct knot *K)
{
    unsigned int ind, n_terms;
    int shift;
    int n_plus = 0;
    int n_minus = 0;
    struct laurent_polynomial out = kauffman_bracket(K);

    for (ind = 0U; ind < 2U * K->number_of_crossings; ++ind)
    {
        if (K->sign[ind] == positive_crossing)
            n_plus++;
        else
            n_minus++;
    }

    n_plus = n_plus >> 1;
    n_minus = n_minus >> 1;
    shift = n_plus - 2*n_minus;

    out.lowest_degree += shift;
    out.highest_degree += shift;

    if (n_minus & 1)
    {
        n_terms = (unsigned int)(out.highest_degree - out.lowest_degree + 1);

        for (ind = 0U; ind < n_terms; ++ind)
            out.coeffs[ind] = -out.coeffs[ind];
    }

    return out;
}
