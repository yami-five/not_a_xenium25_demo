#include "animation.h"

const Frame jawAnimationFrames[10] = {
    {.x = -1, .y = 0, .angle = 0.08f},
    {.x = -1, .y = 0, .angle = 0.08f},
    {.x = -1, .y = 1, .angle = 0.08f},
    {.x = -1, .y = 0, .angle = 0.08f},
    {.x = -1, .y = 1, .angle = 0.08f},
    {.x = 1, .y = -1, .angle = -0.08f},
    {.x = 1, .y = 0, .angle = -0.08f},
    {.x = 1, .y = -1, .angle = -0.08f},
    {.x = 1, .y = 0, .angle = -0.08f},
    {.x = 1, .y = 0, .angle = -0.08f},
};

const Frame skullAnimationFrames[10] = {
    {.x = 0, .y = 0, .angle = 0.0f},
    {.x = 0, .y = 0, .angle = 0.0f},
    {.x = 0, .y = -1, .angle = 0.0f},
    {.x = 0, .y = 0, .angle = 0.0f},
    {.x = 0, .y = -1, .angle = 0.0f},
    {.x = 0, .y = 1, .angle = 0.0f},
    {.x = 0, .y = 0, .angle = 0.0f},
    {.x = 0, .y = 1, .angle = 0.0f},
    {.x = 0, .y = 0, .angle = 0.0f},
    {.x = 0, .y = 0, .angle = 0.0f},
};

static const Animation animations[2] = {
    {
        .label = "jawAnimation",
        .frames = jawAnimationFrames,
        .framesNum = 10,
    },
    {
        .label = "skullAnimation",
        .frames = skullAnimationFrames,
        .framesNum = 10,
    },
};

const Animation *get_animation_by_index(uint8_t index)
{
    return &animations[index];
}