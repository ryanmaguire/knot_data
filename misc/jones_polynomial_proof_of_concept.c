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

#include "jones_poly_coeffs.h"

/*  The algorithm is exponential in crossings. The crossings need to fit in   *
 *  the width of an integer, which is usually 32-bits.                        */
#define MAX_CROSSINGS (32)

/*  Given a knot with n crossings, bound is (n-1) + (n+1) = 2n. Lower bound   *
 *  is -(n+1). Max size of coefficient array is then 3n + 2.                  */
#define MAX_POLY_SIZE (3 * MAX_CROSSINGS + 2)

/*  The zeroth entry of the array corresponds to the minimum degree. Need to  *
 *  shift to get the constant term.                                           */
#define DEGREE_SHIFT (MAX_CROSSINGS + 1)

#define MIN_DEGREE (-MAX_CROSSINGS)
#define MAX_DEGREE (2 * MAX_CROSSINGS)

/*  Buffer for the polynomial.                                                */
static signed int kauffman_coeffs[MAX_POLY_SIZE];

/*  Crossing signs are negative and positive. This assumes the knot has been  *
 *  given an orientation.                                                     */
enum crossing_sign {negative_crossing, positive_crossing};

/*  Crossing type for the Gauss code. Is the current strand over or under.    */
enum crossing_type {under_crossing, over_crossing};

/*  The direction is stored as an unsigned char. There are two possibilities: *
 *  forwards and backwards.                                                   */
static unsigned char backward = 0x00U;
static unsigned char forward = 0x01U;

/*  Extended Gauss code. This contains crossing number, sign, and type.       */
struct knot {
    unsigned int number_of_crossings;
    enum crossing_sign *sign;
    enum crossing_type *type;
    unsigned int *crossing_number;
};

/*  This struct is used for keeping track of which crossing number            *
 *  corresponds to which indices.                                             */
struct crossing_indices {
    unsigned int under;
    unsigned int over;
};

/*  Laurent polynomials, i.e. polynomials with negative exponents allowed.    */
struct laurent_polynomial {
    signed int lowest_degree;
    signed int highest_degree;
    signed int *coeffs;
};

static const struct laurent_polynomial q_qinv_powers[34] = {
    {-0, 0, q_qinv_pow00},
    {-1, 1, q_qinv_pow01},
    {-2, 2, q_qinv_pow02},
    {-3, 3, q_qinv_pow03},
    {-4, 4, q_qinv_pow04},
    {-5, 5, q_qinv_pow05},
    {-6, 6, q_qinv_pow06},
    {-7, 7, q_qinv_pow07},
    {-8, 8, q_qinv_pow08},
    {-9, 9, q_qinv_pow09},
    {-10, 10, q_qinv_pow10},
    {-11, 11, q_qinv_pow11},
    {-12, 12, q_qinv_pow12},
    {-13, 13, q_qinv_pow13},
    {-14, 14, q_qinv_pow14},
    {-15, 15, q_qinv_pow15},
    {-16, 16, q_qinv_pow16},
    {-17, 17, q_qinv_pow17},
    {-18, 18, q_qinv_pow18},
    {-19, 19, q_qinv_pow19},
    {-20, 20, q_qinv_pow20},
    {-21, 21, q_qinv_pow21},
    {-22, 22, q_qinv_pow22},
    {-23, 23, q_qinv_pow23},
    {-24, 24, q_qinv_pow24},
    {-25, 25, q_qinv_pow25},
    {-26, 26, q_qinv_pow26},
    {-27, 27, q_qinv_pow27},
    {-28, 28, q_qinv_pow28},
    {-29, 29, q_qinv_pow29},
    {-30, 30, q_qinv_pow30},
    {-31, 31, q_qinv_pow31},
    {-32, 32, q_qinv_pow32},
    {-33, 33, q_qinv_pow33}
};

/*  Basic algorithm for computing the Hamming weight of an unsigned integer   *
 *  that is sizeof(unsigned int) * CHAR_BIT bits long. There are faster       *
 *  methods, but this does not assume 32-bit int and is more portable.        */
static unsigned int hamming_weight(unsigned int val)
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

/*  Returns an array ind where ind[n] is a struct containing the indices of   *
 *  the under and over crossings of the nth crossing.                         */
static struct crossing_indices *get_indices(const struct knot *K)
{
    unsigned int n;
    struct crossing_indices *ind;

    /*  Check for invalid inputs.                                             */
    if (!K)
        return NULL;

    /*  If there are no crossings, return an empty array (a NULL pointer).    */
    if (K->number_of_crossings == 0U)
        return NULL;

    /*  Allocate memory for the array.                                        */
    ind = malloc(sizeof(*ind)*K->number_of_crossings);

    /*  Check if malloc failed.                                               */
    if (!ind)
        return NULL;

    /*  Loop through and save the indices.                                    */
    for (n = 0U; n < 2U * K->number_of_crossings; ++n)
    {
        if (K->type[n] == over_crossing)
            ind[K->crossing_number[n]].over = n;
        else
             ind[K->crossing_number[n]].under = n;
    }

    return ind;
}
/*  End of get_indices.                                                       */

static unsigned int
number_of_circles_in_resolution(const struct knot *K,
                                const struct crossing_indices *ind,
                                unsigned int resolution)
{
    unsigned int number_of_circles, n, k, m;
    unsigned char dir, crossing_resolution;
    unsigned char *have_visited;

    /*  The empty knot has zero circles.                                      */
    if (!K)
        return 0U;

    /*  The unknot has 1 circle.                                              */
    if (K->number_of_crossings == 0U)
        return 1U;

    /*  If ind is a NULL pointer, there's nothing that can be done.           */
    if (!ind)
        return 0U;

    /*  Create an array of zeros with calloc.                                 */
    have_visited = calloc(sizeof(*have_visited), 4U*K->number_of_crossings);

    /*  Check if calloc failed.                                               */
    if (!have_visited)
        return 0U;

    /*  Initialize number_of_circles to zero.                                 */
    number_of_circles = 0U;

    for (n = 0U; n < 2U*K->number_of_crossings; ++n)
    {
        k = 4U*K->crossing_number[n];

        /*  Each crossing has four entrances. Bottom left, bottom right, top  *
         *  left, top right. Check which one's we have visited.               */
        for (m = 0U; m < 3; ++m)
        {
            if (!have_visited[k])
                break;
            else
                ++k;
        }

        if (have_visited[k])
            continue;

        if (m < 2U)
            dir = forward;
        else
            dir = backward;

        m = n;
        while (!have_visited[k])
        {
            crossing_resolution = (resolution >> K->crossing_number[m]) & 0x01U;
            have_visited[k] = 0x01U;

            if (K->sign[m] == positive_crossing)
            {
                if (K->type[m] == over_crossing)
                {
                    if (crossing_resolution == 0x00U)
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m] + 3U;
                        else
                            k = 4U*K->crossing_number[m] + 1U;
                    }
                    else
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m] + 1U;
                        else
                            k = 4U*K->crossing_number[m] + 3U;

                        dir = 0x01U - dir;
                    }
                    m = ind[K->crossing_number[m]].under;
                }
                else
                {
                    if (crossing_resolution == 0x00U)
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m] + 2U;
                        else
                            k = 4U*K->crossing_number[m];
                    }
                    else
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m];
                        else
                            k = 4U*K->crossing_number[m] + 2U;

                        dir = 0x01U - dir;
                    }
                    m = ind[K->crossing_number[m]].over;
                }
            }
            else
            {
                if (K->type[m] == over_crossing)
                {
                    if (crossing_resolution == 0x00U)
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m] + 2U;
                        else
                            k = 4U*K->crossing_number[m];
                    }
                    else
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m];
                        else
                            k = 4U*K->crossing_number[m] + 2U;

                        dir = 0x01U - dir;
                    }

                    m = ind[K->crossing_number[m]].under;
                }
                else
                {
                    if (crossing_resolution == 0x00U)
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m] + 3U;
                        else
                            k = 4U*K->crossing_number[m] + 1U;
                    }
                    else
                    {
                        if (dir == forward)
                            k = 4U*K->crossing_number[m] + 1U;
                        else
                            k = 4U*K->crossing_number[m] + 3U;

                        dir = 0x01U - dir;
                    }
                    m = ind[K->crossing_number[m]].over;
                }
            }

            if (dir == forward)
            {
                if (m == 2U*K->number_of_crossings - 1U)
                    m = 0U;
                else
                    ++m;
            }
            else
            {
                if (m == 0U)
                    m = 2U*K->number_of_crossings - 1U;
                else
                    --m;
            }

            have_visited[k] = 0x01U;

            k = 4U*K->crossing_number[m];
            if (K->sign[m] == positive_crossing)
            {
                if (K->type[m] == over_crossing)
                {
                    if (dir == backward)
                        k += 2U;
                }
                else
                {
                    if (dir == forward)
                        k += 1U;
                    else
                        k += 3U;
                }
            }
            else
            {
                if (K->type[m] == over_crossing)
                {
                    if (dir == forward)
                        k += 1U;
                    else
                        k += 3U;
                }
                else
                {
                    if (dir == backward)
                        k += 2U;
                }
            }
        }
        ++number_of_circles;
    }
    free(have_visited);
    return number_of_circles;
}

static void
print_poly(struct laurent_polynomial P)
{
    signed int n, N;

    if (!P.coeffs)
    {
        puts("0");
        return;
    }

    N = P.highest_degree - P.lowest_degree + 1;

    if (P.coeffs[0] == 1)
        printf("q^%d", P.lowest_degree);
    else if (P.coeffs[0] == -1)
        printf("-q^%d", P.lowest_degree);
    else
        printf("%dq^%d", P.coeffs[0], P.lowest_degree);

    for (n = 1; n < N; ++n)
    {
        if (P.coeffs[n] == 0)
            continue;
        else if (P.coeffs[n] == 1)
            printf(" + q^%d", P.lowest_degree + n);
        else if (P.coeffs[n] == -1)
            printf(" - q^%d", P.lowest_degree + n);
        else if (P.coeffs[n] > 1)
            printf(" + %dq^%d", P.coeffs[n], P.lowest_degree + n);
        else
            printf(" - %dq^%d", -P.coeffs[n], P.lowest_degree + n);
    }

    printf("\n");
}

static void
add_kauffman_summand(const struct laurent_polynomial *poly, unsigned int weight)
{
    const int signed_weight = (int)weight;
    int n = DEGREE_SHIFT + poly->lowest_degree;

    if (weight & 1U)
        for (; n <= poly->highest_degree + DEGREE_SHIFT; ++n)
            kauffman_coeffs[n + signed_weight] -= poly->coeffs[n];

    else
        for (; n <= poly->highest_degree + DEGREE_SHIFT; ++n)
            kauffman_coeffs[n + signed_weight] += poly->coeffs[n];
}

/*  Function for multiplying two polynomials.                                 */
static void initialize_buffer(void)
{
    unsigned int n;

    for (n = 0U; n < MAX_POLY_SIZE; ++n)
        kauffman_coeffs[n] = 0U;
}

static void kauffman_bracket(const struct knot *K)
{
    unsigned int n, weight, n_circles;
    const struct crossing_indices * const ind = get_indices(K);
    initialize_buffer();

    for (n = 0; n < (1U << K->number_of_crossings); ++n)
    {
        weight = hamming_weight(n);
        n_circles = number_of_circles_in_resolution(K, ind, n);
        add_kauffman_summand(&q_qinv_powers[n_circles], weight);
    }
}

int main(void)
{
    struct laurent_polynomial out;
    struct knot K;
    enum crossing_sign s[6] = {
        positive_crossing, positive_crossing, positive_crossing,
        positive_crossing, positive_crossing, positive_crossing
    };
    unsigned int c[6] = {0U, 1U, 2U, 0U, 1U, 2U};

    enum crossing_type t[6] = {
        over_crossing, under_crossing, over_crossing,
        under_crossing, over_crossing, under_crossing
    };

    K.number_of_crossings = 3;
    K.sign = s;
    K.type = t;
    K.crossing_number = c;
    kauffman_bracket(&K);
    out.lowest_degree = MIN_DEGREE;
    out.highest_degree = MAX_DEGREE;
    out.coeffs = kauffman_coeffs;
    print_poly(out);

    return 0;
}
