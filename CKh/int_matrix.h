#include <libtmpl/include/tmpl_bool.h>
#include <stddef.h>
typedef int integer;
#define INTEGER_QUALIFIER "%8d"

typedef struct int_matrix_def {
    size_t n_rows;
    size_t n_columns;
    size_t n_elements;
    integer *data;
} int_matrix;

extern void int_matrix_init(int_matrix *mat);
extern integer int_matrix_element(const int_matrix *mat, size_t row, size_t column);
extern void int_matrix_calloc(int_matrix *mat, size_t n_rows, size_t n_columns);
extern void int_matrix_free(int_matrix *mat);
extern tmpl_Bool int_matrix_is_diagonal(const int_matrix *mat);
extern tmpl_Bool int_matrix_is_zero(const int_matrix *mat);
extern void int_matrix_swap_rows(int_matrix *mat, size_t a, size_t b);
extern void int_matrix_swap_columns(int_matrix *mat, size_t a, size_t b);
extern void int_matrix_print(const int_matrix *mat);
extern void int_matrix_make_random(int_matrix *mat, integer max);
extern void int_matrix_row_addition(int_matrix *mat, size_t a, size_t b);
extern void
int_matrix_scaled_row_addition(int_matrix *mat, integer n, size_t a, size_t b);
