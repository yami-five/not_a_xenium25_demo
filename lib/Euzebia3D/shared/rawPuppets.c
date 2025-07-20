#include "rawPuppets.h"

static const RawBone mascotSkullChildren[4] = {
    {
        .label = "mascotSkullJaw",
        .x = 0,
        .y = 0,
        .spriteIndex = 7,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullEyesBack",
        .x = 0,
        .y = 0,
        .spriteIndex = 8,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullLeftEye",
        .x = 0,
        .y = 0,
        .spriteIndex = 9,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullRightEye",
        .x = 0,
        .y = 0,
        .spriteIndex = 10,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
};

static const RawBone mascotHand1[1] = {
    {
        .label = "mascotHand1",
        .x = 0,
        .y = 0,
        .spriteIndex = 3,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
};

static const RawBone mascotArmChildren[1] = {
    {
        .label = "mascotArm2",
        .x = 0,
        .y = 0,
        .spriteIndex = 2,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 1,
        .childBonesLayer2 = mascotHand1,
    },
};

static const RawBone mascotBodyChildren[3] = {
    {
        .label = "mascotSkull",
        .x = 15,
        .y = 90,
        .spriteIndex = 6,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = mascotSkullChildren,
        .childBonesNumLayer2 = 4,
    },
    {
        .label = "mascotReflection",
        .x = 15,
        .y = 100,
        .spriteIndex = 5,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = NULL,
        .childBonesNumLayer2 = 0,
    },
    {
        .label = "mascotArm1",
        .x = 0,
        .y = 0,
        .spriteIndex = 1,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = mascotArmChildren,
        .childBonesNumLayer2 = 1,
    },
};

static const RawBone mascotBodyBone[1] = {
    {.label = "mascotBone",
     .x = 0,
     .y = 0,
     .spriteIndex = 4,
     .childBonesLayer1 = mascotBodyChildren,
     .childBonesNumLayer1 = 3,
     .childBonesLayer2 = NULL,
     .childBonesNumLayer2 = 0},
};

static const RawPuppet rawPuppets[] = {
    {
        .x = 0,
        .y = 0,
        .bonesNum = 1,
        .bones = mascotBodyBone,
    },
};

const RawPuppet *get_raw_puppet(uint8_t puppetIndex)
{
    return &rawPuppets[puppetIndex];
}