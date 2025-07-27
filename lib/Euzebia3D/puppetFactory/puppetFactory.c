#include "IPuppetFactory.h"
#include "puppetFactory.h"
#include "puppet.h"
#include "../shared/rawPuppets.h"

Bone *create_bones(const RawBone *rawBones, const uint8_t bonesNum, int32_t *parentWorldMatrix)
{
    Bone *newBones = (Bone *)malloc(sizeof(Bone) * bonesNum);
    for (uint8_t i = 0; i < bonesNum; i++)
    {
        newBones[i].label = rawBones[i].label;
        newBones[i].x = rawBones[i].x;
        newBones[i].y = rawBones[i].y;
        newBones[i].angle=0.0f;
        newBones[i].sprite = get_sprite(rawBones[i].spriteIndex);
        make_local_matrix(&newBones[i]);
        make_world_matrix(&newBones[i],parentWorldMatrix);
        newBones[i].childBonesNumLayer1 = rawBones[i].childBonesNumLayer1;
        newBones[i].childBonesNumLayer2 = rawBones[i].childBonesNumLayer2;
        if (rawBones[i].childBonesNumLayer1 != 0)
            newBones[i].childBonesLayer1 = create_bones(rawBones[i].childBonesLayer1, rawBones[i].childBonesNumLayer1,newBones->worldMatrix);
        if (rawBones[i].childBonesNumLayer2 != 0)
            newBones[i].childBonesLayer2 = create_bones(rawBones[i].childBonesLayer2, rawBones[i].childBonesNumLayer2,newBones->worldMatrix);
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
    int32_t parentWorldMatrix[9]={
        1024, 0, newPuppet->x*SCALE_FACTOR,
        0, 1024, newPuppet->y*SCALE_FACTOR,
        0,0,1024
    };
    if (rawPuppet->bonesNum != 0)
        newPuppet->bones = create_bones(rawPuppet->bones, rawPuppet->bonesNum,parentWorldMatrix);
    return newPuppet;
}

static IPuppetFactory puppet = {
    .create_puppet = create_puppet,
};

const IPuppetFactory *get_puppetFactory(void)
{
    return &puppet;
}