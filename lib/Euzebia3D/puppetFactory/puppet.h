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
    uint8_t spriteIndex;
    Bone *childBonesLayer1;
    uint8_t childBonesNumLayer1;
    Bone *childBonesLayer2;
    uint8_t childBonesNumLayer2;
} Bone;

typedef struct
{
    int16_t x;
    int16_t y;
    Bone *bones;
    uint8_t bonesNum;
} Puppet;

void move_puppet(Puppet *puppet, int16_t newX, int16_t newY);

#endif