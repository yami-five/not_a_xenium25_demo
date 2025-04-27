#ifndef RENDERER_h
#define RENDERER_h

#include "IRenderer.h"

#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define WIDTH_HALF 160
#define HEIGHT_HALF 120
#define BUFFER_SIZE 153600
static uint dma_channel;

const IRenderer *get_renderer(void);

#endif