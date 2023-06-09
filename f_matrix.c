matrix *matrix_new_4x4() {

    double mat[4][4] = {
        {1,0,0,0}, 
        {0,1,0,0}, 
        {0,0,1,0}, 
        {0,0,0,1}
    };

    double *a[4] = {mat[0], mat[1], mat[2], mat[3]};
    double **dat = a;

    matrix *m;
    m->data = dat;
    m->num_rows = 4;
    m->num_cols = 4;
    m->is_square = 1;
    return(m);
}