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
