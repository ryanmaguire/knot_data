#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "kauffman.h"

struct knot knot_from_gauss_code(const char *str)
{
    enum crossing_sign sign_buffer[2U * MAX_CROSSINGS];
    enum crossing_type type_buffer[2U * MAX_CROSSINGS];
    unsigned int crossing_buffer[2U * MAX_CROSSINGS];
    unsigned int index = 0U;
    unsigned int n;
    struct knot out;
    const char *tmp;

    while (*str)
    {
        if (tolower(*str) == 'o')
            type_buffer[index] = over_crossing;
        else
            type_buffer[index] = under_crossing;

        str++;
        tmp = str;

        while (isdigit(*str))
            str++;

        crossing_buffer[index] = strtol(tmp, NULL, 10);

        if (*str == '+')
            sign_buffer[index] = positive_crossing;
        else
            sign_buffer[index] = negative_crossing;

        index++;
        str++;
    }

    out.number_of_crossings = index >> 1U;
    out.crossing_number = malloc(sizeof(*out.crossing_number) * index);
    out.type = malloc(sizeof(*out.type) * index);
    out.sign = malloc(sizeof(*out.sign) * index);

    for (n = 0U; n < index; ++n)
    {
        out.crossing_number[n] = crossing_buffer[n];
        out.type[n] = type_buffer[n];
        out.sign[n] = sign_buffer[n];
    }

    return out;
}
