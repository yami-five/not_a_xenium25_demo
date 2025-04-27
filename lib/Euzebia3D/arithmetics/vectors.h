#include "fpa.h"

#ifndef VECTORS_H
#define VECTORS_H

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
} Vector4;

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vector3;

typedef struct
{
    int32_t x;
    int32_t y;
} Vector2;

Vector3 *sub_vectors(Vector3 *vecA, Vector3 *vecB);
Vector3 *mul_vectors(Vector3 *vecA, Vector3 *vecB);
int32_t mul_vectors_scalar(Vector3 *vecA, Vector3 *vecB);
int32_t len_vector(Vector3 *vec);
void norm_vector(Vector3 *vec);
void fixed_mul_matrix_vector(int32_t *x, int32_t *y, int32_t *z, int32_t *w, int32_t *matrix);

#endif