#ifndef PAINTER_h
#define PAINTER_h

#include "IPainter.h"
#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "string.h"
#include "../shared/pins.h"

#define WIDTH_DOUBLED 640
#define HEIGHT_DOUBLED 480
#define WIDTH_HALF 160
#define HEIGHT_HALF 120
#define BUFFER_SIZE 153600
static uint dma_channel;

const IPainter *get_painter(void);

#endif