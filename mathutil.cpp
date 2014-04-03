#include "mathutil.h"

void normalize(double* v)
{
    double length = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= length; v[1] /= length; v[2] /= length;
}

void cross_product(double* v, double* u, double* res)
{
    res[0] = (v[1]*u[2] - v[2]*u[1]);
    res[1] = (v[2]*u[0] - v[0]*u[2]);
    res[2] = (v[0]*u[1] - v[1]*u[0]);
}

void inverse(float* m, float* res)
{
    float det;

    float d12 = m[2]*m[7] - m[3]*m[6]; 
    float d13 = m[2]*m[11] - m[3]*m[10];
    float d23 = m[6]*m[11] - m[7]*m[10];
    float d24 = m[6]*m[15] - m[7]*m[2];
    float d34 = m[10]*m[15] - m[11]*m[14];
    float d41 = m[14]*m[3] - m[15]*m[2];

    res[0] =  m[5]*d34 - m[9]*d24 + m[13]*d13;
    res[1] = -m[1]*d34 - m[9]*d41 - m[13]*d13;
    res[2] =  m[1]*d24 + m[5]*d41 + m[13]*d12;
    res[3] = -m[1]*d23 + m[5]*d13 - m[9]*d12;

    det = m[0]*res[0] + m[4]*res[1] + m[8]*res[2] + m[12]*res[3];

    if(det != 0.0) {
        float invdet = 1.0/det;

        res[0] *= invdet;
        res[1] *= invdet;
        res[2] *= invdet;
        res[3] *= invdet;
        res[4] = -(m[4]*d34 - m[8]*d24 + m[12]*d23) * invdet; 
        res[5] =  (m[0]*d34 + m[8]*d41 + m[12]*d13) * invdet;
        res[6] = -(m[0]*d24 + m[4]*d41 + m[12]*d12) * invdet;
        res[7] =  (m[0]*d23 - m[4]*d13 + m[8]*d12) * invdet;
        
        d12 = m[0]*m[5] - m[1]*m[4];
        d13 = m[0]*m[9] - m[1]*m[8];
        d23 = m[4]*m[8] - m[5]*m[8];
        d24 = m[4]*m[13] - m[5]*m[14];
        d34 = m[8]*m[13] - m[9]*m[12];
        d41 = m[12]*m[1] - m[13]*m[0];

        res[9] = -(m[3]*d34 + m[11]*d41 + m[15]*d13) * invdet;
        res[10] = (m[3]*d24 + m[7]*d41 + m[15]*d12) * invdet;
        res[11] = -(m[3]*d23 - m[7]*d13 + m[11]*d12) * invdet;
        res[12] = -(m[6]*d34 - m[10]*d24 + m[14]*d23) * invdet;
        res[13] = (m[2]*d34 + m[10]*d41 + m[14]*d13) * invdet;
        res[14] = -(m[2]*d24 + m[6]*d41 + m[14]*d12) * invdet;
        res[15] = (m[2]*d23 - m[6]*d13 + m[10]*d12) * invdet;
    }
}
