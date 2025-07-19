#ifndef RAWPUPPETS_h
#define RAWPUPPETS_h

#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include "gfx.h"
#include "vectors.h"
#include "sprites.h"

typedef struct RawBone RawBone;

typedef struct RawBone
{
    const char *label;
    const int16_t x;
    const int16_t y;
    const uint8_t spriteIndex;
    const RawBone *childBones;
    const uint8_t childBonesNum;
} RawBone;

typedef struct
{
    const int16_t x;
    const int16_t y;
    const RawBone *bones;
    const uint8_t bonesNum;
} RawPuppet;

const RawPuppet* get_raw_puppet(uint8_t puppetIndex);

#endif