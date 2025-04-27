#ifndef IMESHFACTORY_h
#define IMESHFACTORY_h

#include <stdint.h>
#include "mesh.h"

typedef struct
{
    Mesh* (*create_colored_mesh)(uint16_t color, uint8_t meshIndex);
    Mesh* (*create_textured_mesh)(uint8_t imageIndex, uint8_t meshIndex);
} IMeshFactory;

#endif