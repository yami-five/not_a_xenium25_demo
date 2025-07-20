#include "rawPuppets.h"

static const RawBone mascotSkullChildren[4] = {
    {
        .label = "mascotSkullJaw",
        .x = 28,
        .y = 9,
        .spriteIndex = 7,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullEyesBack",
        .x = 10,
        .y = 10,
        .spriteIndex = 8,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullLeftEye",
        .x = 20,
        .y = 25,
        .spriteIndex = 9,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullRightEye",
        .x = 17,
        .y = 12,
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
        .x = 8,
        .y = 45,
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
        .x = 11,
        .y = 48,
        .spriteIndex = 1,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 1,
        .childBonesLayer2 = mascotHand1,
    },
};

static const RawBone mascotBodyChildrenLayer1[2] = {
    {
        .label = "mascotSkull",
        .x = 13,
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
};

static const RawBone mascotBodyChildrenLayer2[1] = {
    {
        .label = "mascotArm1",
        .x = 53,
        .y = 117,
        .spriteIndex = 2,
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
     .childBonesLayer1 = mascotBodyChildrenLayer1,
     .childBonesNumLayer1 = 2,
     .childBonesLayer2 = mascotBodyChildrenLayer2,
     .childBonesNumLayer2 = 1},
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