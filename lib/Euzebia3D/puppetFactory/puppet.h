#ifndef PUPPET_h
#define PUPPET_h

#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include "../shared/gfx.h"
#include "vectors.h"
#include "../shared/sprites.h"

typedef struct Bone Bone;

typedef struct Bone
{
    int16_t x;
    int16_t y;
    Sprite *sprite;
    Bone *childBones;
    uint8_t childBonesNum;
} Bone;

typedef struct
{
    int16_t x;
    int16_t y;
    Bone *bones;
    uint8_t bonesNum;
} Puppet;

void draw_puppet(Puppet *puppet);
void move_puppet(Puppet *puppet, int16_t newX, int16_t newY);

#endif