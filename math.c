#include "math.h"
#include <math.h>
double factorial[14] = {
    1, //0
    1, //1
    2, //2
    6, //3
    24, //4
    120, //5
    720, //6
    5040, //7
    40320, //8
    362880, //9
    3628800, //10
    39916800, //11
    479001600, //12
    6227020800, //13
    87178291200, //14
    1307674368000 //15
};

double m_sin(double x) {
   /* x = ((double)(((int)(x*10000)) % (31415))/10000);


    double sin_x = x - 
        (m_pow(x,3))/factorial[3] + 
        (m_pow(x,5))/factorial[5] - 
        (m_pow(x,7))/factorial[7] + 
        (m_pow(x,9))/factorial[9] - 
        (m_pow(x,11))/factorial[11] +
        (m_pow(x,13))/factorial[13];
       // (m_pow(x,15))/factorial[15];

    if (sin_x > 1)
        sin_x = 1;*/

    return sin(x);
}

double m_cos(double x) {
   /* x = ((double)(((int)(x*10000)) % (31415))/10000);

    double cos_x = 1 - 
        (m_pow(x,2))/factorial[2] + 
        (m_pow(x,4))/factorial[4] - 
        (m_pow(x,6))/factorial[6] + 
        (m_pow(x,8))/factorial[8] -
        (m_pow(x,10))/factorial[10] +
        (m_pow(x,12))/factorial[12];
        //(m_pow(x,14))/factorial[14];

    if (cos_x > 1)
        cos_x = 1; */

    return cos(x);
}

double m_tan(double x) {
    if (x == M_PI/2) {
        return 0;
    }

    return m_sin(x)/m_cos(x);
}

double m_pow(double x, int n) {
    double res = 1;
    int i;
    for (i = 0; i < n; i++) {   
        res = res*x;
    }

    return res;
}

double m_abs(double x) {
    if (x < 0) {
        x = -x;
    }

    return x;
}

float m_fabs(double x) {
    if (x < 0) {
        x = -x;
    }

    return (float) x;
}

