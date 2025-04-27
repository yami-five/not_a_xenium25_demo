#include "vectors.h"
#include "fpa.h"
#include <stdlib.h>

Vector3 *subVectors(Vector3 *vecA, Vector3 *vecB)
{
    Vector3 *result = (Vector3 *)malloc(sizeof(Vector3));
    result->x = vecA->x - vecB->x;
    result->y = vecA->y - vecB->y;
    result->z = vecA->z - vecB->z;
    return result;
}

Vector3 *mulVectors(Vector3 *vecA, Vector3 *vecB)
{
    Vector3 *result = (Vector3 *)malloc(sizeof(Vector3));
    result->x = fixed_mul(vecA->y, vecB->z) + fixed_mul(vecA->z, vecB->y);
    result->y = fixed_mul(vecA->z, vecB->x) + fixed_mul(vecA->x, vecB->z);
    result->z = fixed_mul(vecA->x, vecB->y) + fixed_mul(vecA->y, vecB->x);
    return result;
}

int32_t mulVectorsScalar(Vector3 *vecA, Vector3 *vecB)
{
    return fixed_mul(vecA->x,vecB->x)+fixed_mul(vecA->y,vecB->y)+fixed_mul(vecA->z,vecB->z);
}

int32_t lenVector(Vector3 *vec)
{
    return fast_sqrt(fixed_pow(vec->x) + fixed_pow(vec->y) + fixed_pow(vec->z));
}

void normVector(Vector3 *vec)
{
    int32_t len = lenVector(vec);
    vec->x = fixed_div(vec->x, len);
    vec->y = fixed_div(vec->y, len);
    vec->z = fixed_div(vec->z, len);
}

void fixed_mulMatrixVector(int32_t *x, int32_t *y, int32_t *z, int32_t *w, int32_t *matrix)
{
    int32_t resultX = 0;
    int32_t resultY = 0;
    int32_t resultZ = 0;
    int32_t resultW = 0;

    resultX+=fixed_mul(*x,matrix[0])+fixed_mul(*y,matrix[1])+fixed_mul(*z,matrix[2])+fixed_mul(*w,matrix[3]);
    resultY+=fixed_mul(*x,matrix[4])+fixed_mul(*y,matrix[5])+fixed_mul(*z,matrix[6])+fixed_mul(*w,matrix[7]);
    resultZ+=fixed_mul(*x,matrix[8])+fixed_mul(*y,matrix[9])+fixed_mul(*z,matrix[10])+fixed_mul(*w,matrix[11]);
    resultW+=fixed_mul(*x,matrix[12])+fixed_mul(*y,matrix[13])+fixed_mul(*z,matrix[14])+fixed_mul(*w,matrix[15]);
    
    *x = resultX;
    *y = resultY;
    *z = resultZ;
    *w = resultW;
}