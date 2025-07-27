#include "puppet.h"
#include <string.h>

void make_local_matrix(Bone *bone)
{
    int32_t fixedAngle = float_to_fixed(bone->angle);
    int16_t sin = fast_sin(fixedAngle);
    int16_t cos = fast_cos(fixedAngle);
    bone->localMatrix[0]=cos;
    bone->localMatrix[1]=-sin;
    bone->localMatrix[2]=bone->x*SCALE_FACTOR;
    bone->localMatrix[3]=sin;
    bone->localMatrix[4]=cos;
    bone->localMatrix[5]=bone->y*SCALE_FACTOR;
    bone->localMatrix[6]=0;
    bone->localMatrix[7]=0;
    bone->localMatrix[0]=1024;
}

void make_world_matrix(Bone *bone, int32_t *parentWorldMatrix)
{
    int32_t *result=mul_matrices(parentWorldMatrix,bone->localMatrix,3,3);
    for(uint8_t i=0;i<9;i++)
    {
        bone->worldMatrix[i]=result[i];
    }
    free(result);
}

void update_children_world_matrices(Bone *bone)
{
    for(uint8_t i=0;i<bone->childBonesNumLayer1;i++)
    {
        make_local_matrix(&bone->childBonesLayer1[i]);
        make_world_matrix(&bone->childBonesLayer1[i],bone->worldMatrix);
        update_children_world_matrices(&bone->childBonesLayer1[i]);
        bone->childBonesLayer1[i].x=bone->childBonesLayer1[i].localMatrix[2];
        bone->childBonesLayer1[i].y=bone->childBonesLayer1[i].localMatrix[5];
    }
    for(uint8_t i=0;i<bone->childBonesNumLayer2;i++)
    {
        make_local_matrix(&bone->childBonesLayer2[i]);
        make_world_matrix(&bone->childBonesLayer2[i],bone->worldMatrix);
        update_children_world_matrices(&bone->childBonesLayer2[i]);
    }
}

void update_world_matrices(Puppet *puppet)
{
    int32_t parentWorldMatrix[9]={
        1024, 0, puppet->x*SCALE_FACTOR,
        0, 1024, puppet->y*SCALE_FACTOR,
        0,0,1024
    };
    for(uint8_t i=0;i<puppet->bonesNum;i++)
    {
        make_local_matrix(&puppet->bones[i]);
        make_world_matrix(&puppet->bones[i],parentWorldMatrix);
        update_children_world_matrices(&puppet->bones[i]);
        puppet->bones[i].x=puppet->bones[i].worldMatrix[2];
        puppet->bones[i].y=puppet->bones[i].worldMatrix[5];
    }
}

void move_puppet(Puppet *puppet, int16_t newX, int16_t newY)
{
    puppet->x = newX;
    puppet->y = newY;
    update_world_matrices(puppet);
}

Bone *get_bone_by_name(Bone *bone, const char *boneLabel)
{
    if (strcmp(bone->label, boneLabel) == 0)
        return bone;

    for (uint8_t i = 0; i < bone->childBonesNumLayer1; i++)
    {
        Bone *result = get_bone_by_name(&bone->childBonesLayer1[i], boneLabel);
        if (result)
            return result;
    }

    for (uint8_t i = 0; i < bone->childBonesNumLayer2; i++)
    {
        Bone *result = get_bone_by_name(&bone->childBonesLayer2[i], boneLabel);
        if (result)
            return result;
    }
    return NULL;
}

void transform_bone(Bone *bone, int16_t x, int16_t y, float angle)
{
    if (!bone)
        return;
    bone->x+=x;
    bone->y+=y;
    bone->angle+=angle;
}