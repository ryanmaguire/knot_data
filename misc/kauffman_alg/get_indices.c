#include <stdlib.h>
#include "kauffman.h"

/*  Returns an array ind where ind[n] is a struct containing the indices of   *
 *  the under and over crossings of the nth crossing.                         */
struct crossing_indices *get_indices(const struct knot *K)
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
