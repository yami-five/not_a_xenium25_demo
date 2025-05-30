#include "mesh.h"

TransformInfo *add_transformation(TransformInfo *currentTransformations, uint32_t *currentTransformationsNum, float x, float y, float z, uint8_t transformationType)
{
    if (transformationType > 2)
        return currentTransformations;

    *currentTransformationsNum += 1;
    TransformInfo *newTransformations = (TransformInfo *)realloc(currentTransformations, *currentTransformationsNum * sizeof(TransformInfo));
    newTransformations[*currentTransformationsNum - 1].transformVector = (TransformVector *)malloc(sizeof(TransformVector));
    newTransformations[*currentTransformationsNum - 1].transformType = transformationType;
    newTransformations[*currentTransformationsNum - 1].transformVector->x = x;
    newTransformations[*currentTransformationsNum - 1].transformVector->y = y;
    newTransformations[*currentTransformationsNum - 1].transformVector->z = z;

    return newTransformations;
}

void modify_transformation(TransformInfo *currentTransformations, float x, float y, float z, uint32_t transformationIndex)
{
    currentTransformations[transformationIndex].transformVector->x=float_to_fixed(x);
    currentTransformations[transformationIndex].transformVector->y=float_to_fixed(y);
    currentTransformations[transformationIndex].transformVector->z=float_to_fixed(z);
}

void free_model(Mesh* mesh)
{
    free(mesh->mat);
    free(mesh->faces);
    free(mesh->vertices);
    free(mesh->textureCoords);
    free(mesh->uv);
    free(mesh->transformations);
    free(mesh);
}