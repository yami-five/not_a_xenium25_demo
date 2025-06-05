#include "vectors.h"
#include "fpa.h"
#include <stdlib.h>

Quaternion *mul_quaternion(Quaternion *q1,Quaternion *q2)
{
    Quaternion *result = (Quaternion *)malloc(sizeof(Quaternion));
    result->w = q1->w*q2->w-dot_product(q1->vec,q2->vec);
    result->vec=mul_vectors(q1->vec,q2->vec);
    mul_vec_scalar(q1->vec,q2->w);
    mul_vec_scalar(q2->vec,q1->w);
    result->vec=add_vectors(add_vectors(q1->vec,q2->vec),result->vec);
    // result->w=q1->w*q2->w - q1->vec->x*q2->vec->x - q1->vec->y*q2->vec->y - q1->vec->z*q2->vec->z;

    // result->vec->x=q1->w*q2->vec->x + q1->vec->x*q2->w - q1->vec->y*q2->vec->z + q1->vec->z*q2->vec->y;

    // result->vec->y=q1->w*q2->vec->y + q1->vec->x*q2->vec->z + q1->vec->y*q2->w - q1->vec->z*q2->vec->x;

    // result->vec->z=q1->w*q2->vec->z - q1->vec->x*q2->vec->y + q1->vec->y*q2->vec->x + q1->vec->z*q2->w;

    return result;
}

Vector3 *add_vectors(Vector3 *vecA, Vector3 *vecB)
{
    Vector3 *result = (Vector3 *)malloc(sizeof(Vector3));
    result->x = vecA->x + vecB->x;
    result->y = vecA->y + vecB->y;
    result->z = vecA->z + vecB->z;
    return result;
}

Vector3 *sub_vectors(Vector3 *vecA, Vector3 *vecB)
{
    Vector3 *result = (Vector3 *)malloc(sizeof(Vector3));
    result->x = vecA->x - vecB->x;
    result->y = vecA->y - vecB->y;
    result->z = vecA->z - vecB->z;
    return result;
}

Vector3 *mul_vectors(Vector3 *vecA, Vector3 *vecB)
{
    Vector3 *result = (Vector3 *)malloc(sizeof(Vector3));
    result->x = fixed_mul(vecA->y, vecB->z) + fixed_mul(vecA->z, vecB->y);
    result->y = fixed_mul(vecA->z, vecB->x) + fixed_mul(vecA->x, vecB->z);
    result->z = fixed_mul(vecA->x, vecB->y) + fixed_mul(vecA->y, vecB->x);
    return result;
}

int32_t dot_product(Vector3 *vecA, Vector3 *vecB)
{
    return fixed_mul(vecA->x, vecB->x) + fixed_mul(vecA->y, vecB->y) + fixed_mul(vecA->z, vecB->z);
}
void add_vec_scalar(Vector3 *vec, int32_t scal)
{
    vec->x = vec->x + scal;
    vec->y = vec->y + scal;
    vec->z = vec->z + scal;
}

void sub_vec_scalar(Vector3 *vec, int32_t scal)
{
    vec->x = vec->x - scal;
    vec->y = vec->y - scal;
    vec->z = vec->z - scal;
}

void mul_vec_scalar(Vector3 *vec, int32_t scal)
{
    vec->x = fixed_mul(vec->x, scal);
    vec->y = fixed_mul(vec->y, scal);
    vec->z = fixed_mul(vec->z, scal);
}

int32_t len_vector(Vector3 *vec)
{
    return fast_sqrt(fixed_pow(vec->x) + fixed_pow(vec->y) + fixed_pow(vec->z));
}

void norm_vector(Vector3 *vec)
{
    int32_t len = len_vector(vec);
    vec->x = fixed_div(vec->x, len);
    vec->y = fixed_div(vec->y, len);
    vec->z = fixed_div(vec->z, len);
}

void fixed_mul_matrix_vector(int32_t *x, int32_t *y, int32_t *z, int32_t *w, int32_t *matrix)
{
    int32_t resultX = 0;
    int32_t resultY = 0;
    int32_t resultZ = 0;
    int32_t resultW = 0;

    resultX += fixed_mul(*x, matrix[0]) + fixed_mul(*y, matrix[1]) + fixed_mul(*z, matrix[2]) + fixed_mul(*w, matrix[3]);
    resultY += fixed_mul(*x, matrix[4]) + fixed_mul(*y, matrix[5]) + fixed_mul(*z, matrix[6]) + fixed_mul(*w, matrix[7]);
    resultZ += fixed_mul(*x, matrix[8]) + fixed_mul(*y, matrix[9]) + fixed_mul(*z, matrix[10]) + fixed_mul(*w, matrix[11]);
    resultW += fixed_mul(*x, matrix[12]) + fixed_mul(*y, matrix[13]) + fixed_mul(*z, matrix[14]) + fixed_mul(*w, matrix[15]);

    *x = resultX;
    *y = resultY;
    *z = resultZ;
    *w = resultW;
}