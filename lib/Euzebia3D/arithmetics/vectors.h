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

Vector3 *subVectors(Vector3 *vecA, Vector3 *vecB);
Vector3 *mulVectors(Vector3 *vecA, Vector3 *vecB);
int32_t mulVectorsScalar(Vector3 *vecA, Vector3 *vecB);
int32_t lenVector(Vector3 *vec);
void normVector(Vector3 *vec);
void fixed_mulMatrixVector(int32_t *x, int32_t *y, int32_t *z, int32_t *w, int32_t *matrix);

#endif