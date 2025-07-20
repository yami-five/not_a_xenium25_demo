#include "IPuppetFactory.h"
#include "puppetFactory.h"
#include "puppet.h"
#include "../shared/rawPuppets.h"

Bone *create_bones(const RawBone *rawBones, const uint8_t bonesNum)
{
    Bone *newBones = (Bone *)malloc(sizeof(Bone) * bonesNum);
    for (uint8_t i = 0; i < bonesNum; i++)
    {
        newBones[i].label = rawBones[i].label;
        newBones[i].x = rawBones[i].x;
        newBones[i].y = rawBones[i].y;
        newBones[i].spriteIndex = rawBones[i].spriteIndex;
        newBones[i].childBonesNumLayer1 = rawBones[i].childBonesNumLayer1;
        newBones[i].childBonesNumLayer2 = rawBones[i].childBonesNumLayer2;
        if (rawBones[i].childBonesNumLayer1 != 0)
            newBones[i].childBonesLayer1 = create_bones(rawBones[i].childBonesLayer1, rawBones[i].childBonesNumLayer1);
        if (rawBones[i].childBonesNumLayer2 != 0)
            newBones[i].childBonesLayer2 = create_bones(rawBones[i].childBonesLayer2, rawBones[i].childBonesNumLayer2);
    }
    return newBones;
}

Puppet *create_puppet(uint8_t puppetIndex)
{
    Puppet *newPuppet = (Puppet *)malloc(sizeof(Puppet));
    const RawPuppet *rawPuppet = get_raw_puppet(puppetIndex);
    newPuppet->x = rawPuppet->x;
    newPuppet->y = rawPuppet->y;
    newPuppet->bonesNum = rawPuppet->bonesNum;
    if (rawPuppet->bonesNum != 0)
        newPuppet->bones = create_bones(rawPuppet->bones, rawPuppet->bonesNum);
    return newPuppet;
}

static IPuppetFactory puppet = {
    .create_puppet = create_puppet,
};

const IPuppetFactory *get_puppetFactory(void)
{
    return &puppet;
}