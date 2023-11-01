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

#if __has_builtin(__builtin_popcount)

unsigned int hamming_weight(unsigned int val)
{
    return __builtin_popcount(val);
}

#else

/*  Basic algorithm for computing the Hamming weight of an unsigned integer   *
 *  that is sizeof(unsigned int) * CHAR_BIT bits long. There are faster       *
 *  methods, but this does not assume 32-bit int and is more portable.        */
unsigned int hamming_weight(unsigned int val)
{
    unsigned int result = 0U;

    /*  Loop over the bits of the input.                                      */
    while(val != 0U)
    {
        /*  val & 1U returns 0 if the last bit is zero, and 1 if the last     *
         *  bit is one. Summing over these gives us the Hamming weight.       */
        result += val & 1U;

        /*  Move the input 1 bit to the right.                                */
        val = val >> 1U;
    }

    return result;
}
/*  End of hamming_weight.                                                    */

#endif
