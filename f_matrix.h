#ifndef f_matrix_H
#define f_matrix_H

typedef struct transformation_matrix {
  unsigned int num_rows;
  unsigned int num_cols;
  double data[4][4];
  int is_square;
} matrix;



#endif
