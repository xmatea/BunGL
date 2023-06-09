#ifndef matrix_UTIL_H
#define matrix_UTIL_H

#define BUFFER_SIZE 4096
#define MATRIX_RAND_MAX 0x7fffffff
#define DEFAULT_VALUE 0.0

#define NP_CHECK(ptr, dbg) \
        if ((ptr)) { \
           /* printf("%d\n",ptr); */\
           /* printf("%d\n",dbg); */\
        } else { \
            exit(-1); \
        } \


//double matrix_rand_interval(double min, double max);

  

#endif