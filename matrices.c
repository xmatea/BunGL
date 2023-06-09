#include <stdlib.h>
#define CALLOC calloc
#define FREE free
  
#include "math.h"
#include "matrix_util.h"
#include "matrices.h"
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>


// constructs matrices
matrix *matrix_new(unsigned int num_rows, unsigned int num_cols) {
  if (num_rows == 0) {
    return NULL;
  } 

  if (num_cols == 0) {
    return NULL;
  }

  matrix *m = CALLOC(1, sizeof(*m));
  NP_CHECK(m, 0);

  m->num_rows = num_rows;
  m->num_cols = num_cols;
  m->is_square = (num_rows == num_cols);
  m->data = CALLOC(m->num_rows, sizeof(*m->data));
  NP_CHECK(m->data, 1);

  int i;
  for(i = 0; i < m->num_rows; ++i) {
    m->data[i] = CALLOC(m->num_cols, sizeof(**m->data));
    NP_CHECK(m->data[i], 2);
  }
  return m; 
}

matrix *matrix_new_4x4() {
    double I[4][4] = {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}
    };

    double* data[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            data[i][j] = &I[i][j];
        }
    }

    matrix *m = {
        4, 4, *data, 1
    };

    /*m->num_rows = 4;
    m->num_cols = 4;
    m->is_square = 1;
    m->data = dat; */
    return m; 
}

// Destructor-like
// De-allocates the memory for the matrix
void matrix_free(matrix *mat) {
  int i;
  for(i = 0; i < mat->num_rows; ++i) {
    FREE(mat->data[i]);
  }
  FREE(mat->data);
  FREE(mat);
}

matrix *matrix_from(unsigned int num_rows, unsigned int num_cols, unsigned int n_vals, double *vals) {
  matrix *m = matrix_new(num_rows, num_cols);
  int i, j, v_idx;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      v_idx = i * m->num_cols + j;
      m->data[i][j] = (v_idx < n_vals) ? vals[v_idx] : 0.0;
    }
  }
  return m;
}

/*// Generate random matrix
matrix *matrix_rnd(unsigned int num_rows, unsigned int num_cols, double min, double max) {
  matrix *r = matrix_new(num_rows, num_cols);
  int i, j;
  for(i = 0; i < num_rows; i++) {
    for(j = 0; j < num_cols; j++) {
      r->data[i][j] = matrix_rand_interval(min, max);
    }
  }
  return r;
}*/

// Generate square matrix
matrix *matrix_sqr(unsigned int size) {
  return matrix_new(size, size);
}


/*// Generate random square matrix
matrix *matrix_sqr_rnd(unsigned int size, double min, double max) {
  return matrix_rnd(size, size, min, max);
}*/

// Generate identity matrix
matrix *matrix_identity(unsigned int size) {
  matrix *r = matrix_new(size, size);
  int i;
  for(i = 0; i < r->num_rows; i++) {
    r->data[i][i] = 1.0;
  }
  return r;
}

matrix *matrix_copy(matrix *m) {
  matrix *r  = matrix_new(m->num_rows, m->num_cols);
  int i,j;
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      r->data[i][j] = m->data[i][j];
    }
  }
  return r;
}

// Checks if two matrices have the same dimesions
int matrix_equaldim(matrix *m1, matrix *m2) {
  return (m1->num_cols == m2->num_cols) &&
          (m1->num_rows == m2->num_rows);
}

// Checks if two matrices have the same dimensions, and the elements
// are all equal to each other with a given tolerance;
// For exact equality use tolerance = 0.0
int matrix_equal(matrix *m1, matrix *m2, double tolerance) {
  if (!matrix_equaldim(m1, m2)) {
    return 0;
  }
  int i, j;
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m1->num_cols; j++) {
      if (m_fabs(m1->data[i][j] - m2->data[i][j]) > tolerance) {
        return 0;
      }
    }
  }
  return 1;
}

void matrix_print(matrix *mat) {
  matrix_printf(mat, "%lf\t\t");
}

// Prints the matrix on the stdout (with a custom formatting for elements)
void matrix_printf(matrix *mat, const char *d_fmt) {
  int i, j;
  fprintf(stdout, "\n");
  for(i = 0; i < mat->num_rows; ++i) {
    for(j = 0; j < mat->num_cols; ++j) {
      fprintf(stdout, d_fmt, mat->data[i][j]);
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
} 

void matrix_set_all(matrix *mat, double value) {
  int i, j;
  for(i = 0; i < mat->num_rows; i++) {
    for(j = 0; j < mat->num_cols; j++) {
      mat->data[i][j] = value;
    }
  }
}

// Sets all elements of the matrix to given value
int matrix_set_diag(matrix *m, double value) {
  if (!m->is_square) {
    return 0;
  }
  int i;
  for(i = 0; i < m->num_rows; i++) {
    m->data[i][i] = value;
  }
  return 1;
} 

matrix *matrix_row_get(matrix *m, unsigned int row) {
  if (row >= m->num_rows) {
    return NULL;
  }

  matrix *r = matrix_new(1, m->num_cols);
  memcpy(r->data[0], m->data[row], m->num_cols * sizeof(*r->data[0]));
  return r;
}

matrix *matrix_col_get(matrix *m, unsigned int col) {
  if (col >= m->num_cols) {
    return NULL;
  }

  matrix *r = matrix_new(m->num_rows, 1);
  int j;
  for(j = 0; j < r->num_rows; j++) {
    r->data[j][0] = m->data[j][col];
  }
  return r;
}

int *matrix_col_get_r(matrix *m, matrix *r, unsigned int col) {
  if (col >= m->num_cols) {
    return NULL;
  }

  int j;
  for(j = 0; j < r->num_rows; j++) {
    r->data[j][0] = m->data[j][col];
  }
  return 0;
}


// multiply row by scalar on reference
int matrix_row_mult_r(matrix *m, unsigned int row, double num) {
  if (row>= m->num_rows) {
    return 0;
  }
  int i;
  for(i=0; i < m->num_cols; i++) {
    m->data[row][i] *= num;
  }
  return 1;
}

// multiply row by scalar and return new matrix
matrix *matrix_row_mult(matrix *m, unsigned int row, double num) {
  matrix *r = matrix_copy(m);
  if (!matrix_row_mult_r(r, row, num)) {
    matrix_free(r);
    return NULL;
  }
  return r;
} 

int matrix_col_mult_r(matrix *m, unsigned int col, double num) {
  if (col>=m->num_cols) {
    return 0;
  }
  int i;
  for(i = 0; i < m->num_rows; i++) {
    m->data[i][col] *= num;
  }
  return 1;
} 

matrix *matrix_col_mult(matrix *m, unsigned int col, double num) {
  matrix *r = matrix_copy(m);
  if (!matrix_col_mult_r(r, col, num)) {
    matrix_free(r);
    return NULL;
  }
  return r;
}

//multiply by scalar (reference)
int matrix_smult_r(matrix *m, double num) {
  int i, j;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0; j < m->num_cols; j++) {
      m->data[i][j] *= num;
    }
  }
  return 1;
} 

// SCALAR MULTIPLICATION
matrix *matrix_smult(matrix *m, double num) {
  matrix *r = matrix_copy(m);
  matrix_smult_r(r, num);
  return r;
}

//remove column
matrix *matrix_col_rem(matrix *m, unsigned int column) {
  if(column >= m->num_cols) {
    return NULL;
  }

  matrix *r = matrix_new(m->num_rows, m->num_cols-1);
  int i, j, k;
  for(i = 0; i < m->num_rows; i++) {
    for(j = 0, k=0; j < m->num_cols; j++) {
      if (column!=j) {
        r->data[i][k++] = m->data[i][j];
      }
    }
  }
  return r;
}

// remove row
matrix *matrix_row_rem(matrix *m, unsigned int row) {
  if (row >= m->num_rows) {
    return NULL;
  }
  matrix *r = matrix_new(m->num_rows-1, m->num_cols);
  int i, j, k;
  for(i = 0, k = 0; i < m->num_rows; i++) {
    if (row!=i) {
      for(j = 0; j < m->num_cols; j++) {
        r->data[k][j] = m->data[i][j];
      }
      k++;
    }
  }
  return r;
}

// swap row by reference
int matrix_row_swap_r(matrix*m, unsigned int row1, unsigned int row2) {
  if (row1 >= m->num_rows || row2 >= m->num_rows) {
    return 0;
  }
  double *tmp = m->data[row2];
  m->data[row2] = m->data[row1];
  m->data[row1] = tmp;
  return 1;
} 

// swap row
matrix *matrix_row_swap(matrix *m, unsigned int row1, unsigned int row2) {
  matrix *r = matrix_copy(m);
  if (!matrix_row_swap_r(r, row1, row2)) {
    matrix_free(r);
    return NULL;
  }
  return r;
} 

// swap column by reference
int matrix_col_swap_r(matrix *m, unsigned int col1, unsigned int col2) {
  if (col1 >= m->num_cols || col2 >= m->num_cols) {
    return 0;
  }
  double tmp;
  int j;
  for(j = 0; j < m->num_rows; j++) {
    tmp = m->data[j][col1];
    m->data[j][col1] = m->data[j][col2];
    m->data[j][col2] = tmp;
  }
  return 1;
} 

// swap column
matrix *matrix_col_swap(matrix *m, unsigned int col1, unsigned int col2) {
  matrix *r = matrix_copy(m);
  if (!matrix_col_swap_r(r, col1, col2)) {
    matrix_free(r);
    return NULL;
  }
  return r;
} 

// add two matrices
int matrix_add_r(matrix *m1, matrix *m2) {
  if (!matrix_equaldim(m1, m2)) {
    return 0;
  }
  int i, j;
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m1->num_rows; j++) {
      m1->data[i][j] += m2->data[i][j];
    }
  }
  return 1;
}

matrix *matrix_add(matrix *m1, matrix *m2) {
  matrix *r = matrix_copy(m1);
  if (!matrix_add_r(r, m2)) {
    matrix_free(r);
    return NULL;
  }
  return r;
}

// subtract matrices
int matrix_sub_r(matrix *m1, matrix *m2) {
  if (!matrix_equaldim(m1, m2)) {
    return 0;
  }
  int i, j;
  for(i = 0; i < m1->num_rows; i++) {
    for(j = 0; j < m1->num_cols; j++) {
      m1->data[i][j] -= m2->data[i][j];
    }
  }
  return 1;
}  
matrix *matrix_sub(matrix *m1, matrix *m2) {
  matrix *r = matrix_copy(m2);
  if (!matrix_sub_r(r, m2)) {
    matrix_free(r);
    return NULL;
  }
  return r;
}

// multiply matrices
matrix *matrix_dot(matrix *m1, matrix *m2) {
  if (!(m1->num_cols == m2->num_rows)) {
    return NULL;
  }
  int i, j, k;
  matrix *r = matrix_new(m1->num_rows, m2->num_cols);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      for(k = 0; k < m1->num_cols; k++) {
        r->data[i][j] += m1->data[i][k] * m2->data[k][j];
      }
    }
  }
  
  return r;
}

// multiply matrices on reference
int matrix_dot_r(matrix *m1, matrix *m2) {
  if (!(m1->num_cols == m2->num_rows)) {
    return 1;
  }

  matrix *r = matrix_new(m1->num_rows, m2->num_cols);
  for(int i = 0; i < r->num_rows; i++) {
    for(int j = 0; j < r->num_cols; j++) {
      for(int k = 0; k < m1->num_cols; k++) {
        r->data[i][j] += m1->data[i][k] * m2->data[k][j];
      }
    }
  }

  for(int i = 0; i < r->num_rows; i++) {
    for(int j = 0; j < r->num_cols; j++) {
      m1->data[i][j] = r->data[i][j];
      //printf("m1 = %f, r= %f\n", m1->data[i][j], r->data[i][j]);
    }
  }

    matrix_free(r);
    r = NULL;
    return 0;
}


int *matrix_transp_r(matrix *m) {
  int i, j;
  matrix *r = matrix(m->num_cols, m->num_rows);
  for(i = 0; i < r->num_rows; i++) {
    for(j = 0; j < r->num_cols; j++) {
      m->data[i][j] = m->data[j][i];
    }
  }

  for(int i = 0; i < r->num_rows; i++) {
    for(int j = 0; j < r->num_cols; j++) {
      m1->data[i][j] = r->data[i][j];
    }
  }


  matrix_free(r);
  r = NULL;
  return 0;

}