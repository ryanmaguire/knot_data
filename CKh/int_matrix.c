#include "int_matrix.h"
#include <libtmpl/include/tmpl_bool.h>
#include <stdlib.h>
#include <stdio.h>

void int_matrix_init(int_matrix *mat)
{
    const size_t zero = (size_t)0;

    if (!mat)
        return;

    mat->data = NULL;
    mat->n_rows = zero;
    mat->n_columns = zero;
    mat->n_elements = zero;
}

integer int_matrix_element(const int_matrix *mat, size_t row, size_t column)
{
    if (mat == NULL)
        return 0;

    else if (mat->data == NULL || mat->n_elements == 0)
        return 0;

    else if (row >= mat->n_rows || column >= mat->n_columns)
        return 0;

    return mat->data[row*mat->n_columns + column];
}

void int_matrix_calloc(int_matrix *mat, size_t n_rows, size_t n_columns)
{
    const size_t zero = (size_t)0;
    const size_t n_elements = n_rows * n_columns;

    if (mat == NULL)
        return;

    if (n_elements == zero)
    {
        if (mat->data)
        {
            free(mat->data);
            mat->data = NULL;
        }

        return;
    }

    mat->data = calloc(sizeof(*mat->data), n_elements);

    if (!mat->data)
        return;

    mat->n_rows = n_rows;
    mat->n_columns = n_columns;
    mat->n_elements = n_elements;
}

void int_matrix_free(int_matrix *mat)
{
    if (mat == NULL)
        return;

    if (mat->data == NULL)
        return;

    free(mat->data);
    mat->data = NULL;
}

tmpl_Bool int_matrix_is_diagonal(const int_matrix *mat)
{
    const size_t zero = (size_t)0;
    size_t row, column, ind;

    if (mat == NULL)
        return tmpl_False;

    if (mat->n_elements == zero)
        return tmpl_False;

    ind = zero;

    for (row = zero; row < mat->n_rows; ++row)
    {
        for (column = zero; column < mat->n_columns; ++column)
        {
            if (row != column && mat->data[ind] != 0)
                return tmpl_False;

            ++ind;
        }
    }

    return tmpl_True;
}

tmpl_Bool int_matrix_is_zero(const int_matrix *mat)
{
    const size_t zero = (size_t)0;
    size_t ind;

    if (mat == NULL)
        return tmpl_False;

    if (mat->n_elements == zero)
        return tmpl_False;

    for (ind = zero; ind < mat->n_elements; ++ind)
        if (mat->data[ind] != 0)
            return tmpl_False;

    return tmpl_True;
}

void int_matrix_swap_rows(int_matrix *mat, size_t a, size_t b)
{
    const size_t zero = (size_t)0;
    size_t ind, a_shift, b_shift;

    if (!mat)
        return;

    if (mat->n_elements == zero)
        return;

    if (a >= mat->n_rows || b >= mat->n_rows)
        return;

    a_shift = a * mat->n_columns;
    b_shift = b * mat->n_columns;

    for (ind = zero; ind < mat->n_columns; ++ind)
    {
        const size_t a_ind = ind + a_shift;
        const size_t b_ind = ind + b_shift;
        const integer tmp = mat->data[a_ind];

        mat->data[a_ind] = mat->data[b_ind];
        mat->data[b_ind] = tmp;
    }
}

void int_matrix_swap_columns(int_matrix *mat, size_t a, size_t b)
{
    const size_t zero = (size_t)0;
    size_t ind;

    if (!mat)
        return;

    if (mat->n_elements == zero)
        return;

    if (a >= mat->n_columns || b >= mat->n_columns)
        return;

    for (ind = zero; ind < mat->n_rows; ++ind)
    {
        const size_t a_ind = ind*mat->n_columns + a;
        const size_t b_ind = ind*mat->n_columns + b;
        const integer tmp = mat->data[a_ind];

        mat->data[a_ind] = mat->data[b_ind];
        mat->data[b_ind] = tmp;
    }
}

void int_matrix_row_addition(int_matrix *mat, size_t a, size_t b)
{
    const size_t zero = (size_t)0;
    size_t ind, a_shift, b_shift;

    if (mat == NULL)
        return;

    if (mat->n_elements == zero)
        return;

    a_shift = a * mat->n_columns;
    b_shift = b * mat->n_columns;

    for (ind = zero; ind < mat->n_rows; ++ind)
    {
        const size_t a_ind = ind + a_shift;
        const size_t b_ind = ind + b_shift;

        mat->data[a_ind] += mat->data[b_ind];
    }
}

void
int_matrix_scaled_row_addition(int_matrix *mat, integer n, size_t a, size_t b)
{
    const size_t zero = (size_t)0;
    size_t ind, a_shift, b_shift;

    if (mat == NULL)
        return;

    if (mat->n_elements == zero)
        return;

    a_shift = a * mat->n_columns;
    b_shift = b * mat->n_columns;

    for (ind = zero; ind < mat->n_rows; ++ind)
    {
        const size_t a_ind = ind + a_shift;
        const size_t b_ind = ind + b_shift;

        mat->data[a_ind] += n*mat->data[b_ind];
    }
}

void int_matrix_print(const int_matrix *mat)
{
    const size_t zero = (size_t)0;
    size_t ind, row, column;

    if (mat == NULL)
        return;

    if (mat->n_elements == zero)
        return;

    ind = zero;
    for (row = zero; row < mat->n_rows; ++row)
    {
        for (column = zero; column < mat->n_columns; ++column)
        {
            printf(INTEGER_QUALIFIER " ", mat->data[ind]);
            ++ind;
        }

        printf("\n");
    }
}

void int_matrix_make_random(int_matrix *mat, integer max)
{
    const size_t zero = (size_t)0;
    size_t ind;

    if (mat == NULL)
        return;

    if (mat->n_elements == zero)
        return;

    for (ind = zero; ind < mat->n_elements; ++ind)
        mat->data[ind] = rand() % max;
}

int main(void)
{
    int_matrix mat;
    int_matrix_calloc(&mat, 4, 4);
    int_matrix_make_random(&mat, 10);

    puts("Matrix Before:");
    int_matrix_print(&mat);

    int_matrix_swap_rows(&mat, 2, 3);
    puts("Matrix After Row Swap:");
    int_matrix_print(&mat);

    int_matrix_swap_columns(&mat, 2, 3);
    puts("Matrix After Column Swap:");
    int_matrix_print(&mat);
    int_matrix_free(&mat);
}
