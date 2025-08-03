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
    const char *label;
    int16_t x;
    int16_t y;
    float angle;
    const Sprite *sprite;
    int16_t baseSpriteAngle;
    int worldMatrix[9];
    int localMatrix[9];
    Bone *childBonesLayer1;
    uint8_t childBonesNumLayer1;
    Bone *childBonesLayer2;
    uint8_t childBonesNumLayer2;
} Bone;

typedef struct
{
    const char *label;
    int16_t x;
    int16_t y;
    float angle;
    int worldMatrix[9];
    int localMatrix[9];
    Bone *bones;
    uint8_t bonesNum;
} Puppet;

void make_local_matrix(Bone *bone);
void make_world_matrix(Bone *bone, int *parentWorldMatrix);
void update_world_matrices(Puppet *puppet);
void move_puppet(Puppet *puppet, int16_t newX, int16_t newY);
Bone *get_bone_by_name(Bone *bone, const char *boneLabel);
void transform_bone(Bone *bone, int16_t x, int16_t y, float angle);

#endif