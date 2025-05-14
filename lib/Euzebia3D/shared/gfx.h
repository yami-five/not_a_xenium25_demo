#ifndef GFX_H
#define GFX_H

#include "stdint.h"

typedef struct
{
    const uint16_t *image;
    const uint32_t size;
    const uint16_t width;
    const uint16_t heigth;
} Image;

typedef struct
{
    const unsigned char *obj;
    const uint16_t len;
} Model;

// const Image images[];
// const Model models[];

const Image *get_image(uint8_t image_index);
const Model *get_model(uint8_t model_index);

#endif