#ifndef matrices_H
#define matrices_H

#include "matrix_util.h"

#define MATRIX_MIN_COEF 0.000000000000001

// *****************************************************************************
//
// Library structures
//
// *****************************************************************************
typedef struct matrix_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} matrix;

// *****************************************************************************
//
// Constructing and destroying a matrix struct
//
// *****************************************************************************
matrix *matrix_new(unsigned int num_rows, unsigned int num_cols);
matrix *matrix_new_4x4();
matrix *matrix_rnd(unsigned int num_rows, unsigned int num_cols, double min, double max);
matrix *matrix_sqr(unsigned int size);
matrix *matrix_sqr_rnd(unsigned int size, double min, double max);
matrix *matrix_identity(unsigned int size);
matrix *matrix_copy(matrix *m);
matrix *matrix_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals);

void matrix_free(matrix *matrix);

// *****************************************************************************
//
// Matrix Equality
//
// *****************************************************************************
int matrix_equaldim(matrix *m1, matrix *m2);
int matrix_equal(matrix *m1, matrix *m2, double tolerance);

// *****************************************************************************
//
// Matrix printing
//
// *****************************************************************************
void matrix_print(matrix *matrix);
void matrix_printf(matrix *matrix, const char *d_fmt);

// *****************************************************************************
//
// Accessing and modifying matrix elements
//
// *****************************************************************************
double matrix_get(matrix *matrix, unsigned int i, unsigned int j);
void matrix_set(matrix *matrix, unsigned int i, unsigned int j, double value);
matrix *matrix_row_get(matrix *m, unsigned int row);
matrix *matrix_col_get(matrix *m, unsigned int col);
int *matrix_col_get_r(matrix *m, matrix *r, unsigned int col);
matrix *matrix_col_mult(matrix *m, unsigned int col, double num);
int matrix_col_mult_r(matrix *m, unsigned int col, double num);
matrix *matrix_row_get(matrix *m, unsigned int row);
matrix *matrix_row_mult(matrix *m, unsigned int row, double num);
int matrix_row_mult_r(matrix *m, unsigned int row, double num);
void matrix_set_all(matrix *matrix, double value);
int matrix_set_diag(matrix *matrix, double value);
matrix *matrix_smult(matrix *m, double num);
int matrix_smult_r(matrix *m, double num);

// *****************************************************************************
//
// Modifying the matrix structure
//
// *****************************************************************************
matrix *matrix_col_rem(matrix *m, unsigned int column);
matrix *matrix_row_rem(matrix *m, unsigned int row);
matrix *matrix_row_swap(matrix *m, unsigned int row1, unsigned int row2);
int matrix_row_swap_r(matrix *m, unsigned int row1, unsigned int row2);
matrix *matrix_col_swap(matrix *m, unsigned int col1, unsigned int col2);
int matrix_col_swap_r(matrix *m, unsigned int col1, unsigned int col2);

// *****************************************************************************
//
// Matrix Operations
//
// *****************************************************************************
matrix *matrix_add(matrix *m1, matrix *m2);
int matrix_add_r(matrix *m1, matrix *m2);
matrix *matrix_sub(matrix *m1, matrix *m2);
int matrix_sub_r(matrix *m1, matrix *m2);
matrix *matrix_dot(matrix *m1, matrix *m2);
int matrix_dot_r(matrix *m1, matrix *m2);
int *matrix_transp_r(matrix *m);

#endif