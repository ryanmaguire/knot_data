/******************************************************************************
 *                                 LICENSE                                    *
 ******************************************************************************
 *  This file is part of libtmpl.                                             *
 *                                                                            *
 *  libtmpl is free software: you can redistribute it and/or modify           *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  libtmpl is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with libtmpl.  If not, see <https://www.gnu.org/licenses/>.         *
 ******************************************************************************
 *  Attempt of a proof of concept of computing the Jones' polynomial from     *
 *  the Gauss code of a knot.                                                 *
 ******************************************************************************
 *  Author: Ryan Maguire                                                      *
 *  Date:   2021/09/30                                                        *
 ******************************************************************************/

/*  printf found here.                                                        */
#include <stdio.h>

/*  malloc, calloc, realloc, and free are here.                               */
#include <stdlib.h>

#include "kauffman.h"
#include "q_plus_q_inverse_powers.h"

int main(void)
{
    struct laurent_polynomial out;
    struct knot K = knot_from_gauss_code("O0+U1+O2+U0+O1+U2+");
    out = kauffman_bracket(&K);
    print_poly(&out);

    return 0;
}
