#ifndef GFX_H
#define GFX_H

#include "stdint.h"

typedef struct 
{
    const uint8_t* image;
    uint32_t size;
    uint16_t width;
    uint16_t heigth;
}Image;

extern const Image images[];
extern const uint8_t image1[153600];

#endif