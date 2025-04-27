#ifndef MESH_h
#define MESH_h

#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include "../shared/gfx.h"

typedef struct
{
    uint16_t diffuse;
    const uint16_t *texture;
    int textureSize;
} Material;

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} TransformVector;

typedef struct
{
    uint8_t transformType;
    TransformVector* transformVector;
} TransformInfo;

typedef struct
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    int32_t *vertices;
    uint16_t *faces;
    int32_t *textureCoords;
    uint16_t *uv;
    Material *mat;
    TransformInfo *transformations;
    int transformationsNum;
} Mesh;

TransformInfo *addTransformation(TransformInfo *currentTransformations, int *currentTransformationsNum, float x, float y, float z, uint8_t transformationType);
void freeModel(Mesh* mesh);

#endif