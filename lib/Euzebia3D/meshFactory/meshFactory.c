#include "IMeshFactory.h"
#include "meshFactory.h"
#include "mesh.h"
#include "fpa.h"
#include "../shared/gfx.h"
#include <stdlib.h>

Mesh *createMesh(Material *mat, uint8_t meshIndex)
{
    const Model *obj = get_model(meshIndex);
    Mesh *mesh = (Mesh *)malloc(sizeof(Mesh));
    mesh->verticesCounter = obj->verticesCounter;
    mesh->facesCounter = obj->facesCounter;
    mesh->vertices = (int32_t *)malloc(sizeof(int32_t) * obj->verticesCounter*3);
    mesh->faces = (uint16_t *)malloc(sizeof(uint16_t) * obj->facesCounter*3);
    mesh->textureCoords = (int32_t *)malloc(sizeof(int32_t) * obj->textureCoordsCounter*2);
    mesh->uv = (uint16_t *)malloc(sizeof(uint16_t) * obj->facesCounter*3);
    mesh->mat = mat;
    mesh->transformations = NULL;
    mesh->transformationsNum = 0;

    for(uint16_t i=0;i<obj->facesCounter*3;i++)
    {
        mesh->faces[i]=obj->faces[i];
        mesh->uv[i]=obj->uv[i];
    }
    
    for(uint16_t i=0;i<obj->verticesCounter*3;i++)
    {
        mesh->vertices[i]=float_to_fixed(obj->vertices[i]);
    }

    for(uint16_t i=0;i<obj->textureCoordsCounter*2;i++)
    {
        mesh->textureCoords[i]=float_to_fixed(obj->textureCoords[i]);
    }

    // free((void *)obj);
    return mesh;
}

Mesh* create_colored_mesh(uint16_t color, uint8_t meshIndex)
{
    Material *material = (Material *)malloc(sizeof(Material));
    material->diffuse = color;
    material->texture = 0;
    material->textureSize = 0;
    return createMesh(material, meshIndex);
}

Mesh* create_textured_mesh(uint8_t imageIndex, uint8_t meshIndex)
{
    Material *material = (Material *)malloc(sizeof(Material));
    material->diffuse = 0;
    material->texture = get_image(imageIndex)->image;
    material->textureSize = get_image(imageIndex)->heigth;
    return createMesh(material, meshIndex);
}

static IMeshFactory renderer = {
    .create_colored_mesh = create_colored_mesh,
    .create_textured_mesh = create_textured_mesh
};

const IMeshFactory *get_meshFactory(void)
{
    return &renderer;
}