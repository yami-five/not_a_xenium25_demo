#include "rawPuppets.h"

static const RawBone mascotBodyChildren[2] = {
    {.label = "mascotSkull",
     .x = 15,
     .y = 90,
     .spriteIndex = 6,
     .childBones = NULL,
     .childBonesNum = 0},
    {.label = "mascotReflection",
     .x = 15,
     .y = 100,
     .spriteIndex = 5,
     .childBones = NULL,
     .childBonesNum = 0},
};

static const RawBone mascotBodyBone[1] = {{.label = "mascotBone",
                                           .x = 0,
                                           .y = 0,
                                           .spriteIndex = 4,
                                           .childBones = mascotBodyChildren,
                                           .childBonesNum = 2}};

static const RawPuppet rawPuppets[] = {
    {.x = 0,
     .y = 0,
     .bonesNum = 1,
     .bones = mascotBodyBone}};

const RawPuppet *get_raw_puppet(uint8_t puppetIndex)
{
    return &rawPuppets[puppetIndex];
}