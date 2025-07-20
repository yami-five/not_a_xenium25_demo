#include "IPuppetFactory.h"
#include "puppetFactory.h"
#include "puppet.h"
#include "../shared/rawPuppets.h"

Bone *create_bones(const RawBone *rawBones, const uint8_t bonesNum)
{
    Bone *newBones = (Bone *)malloc(sizeof(Bone) * bonesNum);
    for (uint8_t i = 0; i < bonesNum; i++)
    {
        newBones[i].x=rawBones[i].x;
        newBones[i].y=rawBones[i].y;
        newBones[i].spriteIndex=rawBones[i].spriteIndex;
        newBones[i].childBonesNum=rawBones[i].childBonesNum;
        if(rawBones[i].childBonesNum!=0)
            newBones[i].childBones=create_bones(rawBones[i].childBones,rawBones[i].childBonesNum);
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