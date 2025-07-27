#include "rawPuppets.h"

static const RawBone mascotSkullChildren[4] = {
    {
        .label = "mascotSkullJaw",
        .x = 38,
        .y = 0,
        .spriteIndex = 7,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullEyesBack",
        .x = 10,
        .y = 0,
        .spriteIndex = 8,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullLeftEye",
        .x = 6,
        .y = 0,
        .spriteIndex = 9,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 0,
        .childBonesLayer2 = NULL,
    },
    {
        .label = "mascotSkullRightEye",
        .x = 12,
        .y = 20,
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
        .y = 50,
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
        .x = 5,
        .y = 25,
        .spriteIndex = 1,
        .childBonesNumLayer1 = 0,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer2 = 1,
        .childBonesLayer2 = mascotHand1,
    },
};

static const RawBone mascotSkullMain[1] = {
    {
        .label = "mascotSkull",
        .x = 0,
        .y = 0,
        .spriteIndex = 6,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = mascotSkullChildren,
        .childBonesNumLayer2 = 4,
    },
};

static const RawBone mascotBodyChildrenLayer1[2] = {
    {
        .label = "mascotSkullHelper",
        .x = -75,
        .y = 0,
        .spriteIndex = 255,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = mascotSkullMain,
        .childBonesNumLayer2 = 1,
    },
    {
        .label = "mascotReflection",
        .x = -150,
        .y = 27,
        .spriteIndex = 5,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = NULL,
        .childBonesNumLayer2 = 0,
    },
};

static const RawBone mascotArmMain[1] = {
    {
        .label = "mascotArm1",
        .x = 0,
        .y = 66,
        .spriteIndex = 2,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = mascotArmChildren,
        .childBonesNumLayer2 = 1,
    },
};

static const RawBone mascotBodyChildrenLayer2[1] = {
    {
        .label = "mascotArmHelper",
        .x = -30,
        .y = 5,
        .spriteIndex = 255,
        .childBonesLayer1 = NULL,
        .childBonesNumLayer1 = 0,
        .childBonesLayer2 = mascotArmMain,
        .childBonesNumLayer2 = 1,
    },
};

static const RawBone mascotBodyBone[1] = {
    {.label = "mascotBone",
     .x = 0,
     .y = 0,
     .spriteIndex = 4,
     .childBonesLayer1 = mascotBodyChildrenLayer1,
     .childBonesNumLayer1 = 0,
     .childBonesLayer2 = mascotBodyChildrenLayer2,
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