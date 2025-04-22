#ifndef IPAINTER_h
#define IPAINTER_h

#include <stdint.h>
#include "IDisplay.h"
#include "IHardware.h"

typedef struct
{
    void (*init_painter)(const IDisplay * display, const IHardware * hardware);
    void (*draw_buffer)(void);
    void (*clear_buffer)(void);
    void (*draw_pixel)(uint16_t x, uint16_t y, uint16_t color);
} IPainter;

#endif