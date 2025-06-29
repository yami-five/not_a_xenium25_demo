#ifndef SPRITES_H
#define SPRITES_H

#include "stdint.h"

typedef struct
{
    const uint16_t *pixels;
    const uint8_t size;
} Sprite;

const Sprite *get_sprite(uint8_t sprite_index);

#endif