#ifndef IPAINTER_h
#define IPAINTER_h

#include <stdint.h>
#include "IDisplay.h"
#include "IHardware.h"
#include "puppet.h"

typedef struct
{
    void (*init_painter)(const IDisplay *display, const IHardware *hardware);
    void (*draw_buffer)(void);
    void (*clear_buffer)(uint16_t color);
    void (*draw_pixel)(uint16_t x, uint16_t y, uint16_t color);
    void (*draw_image)(uint8_t image_index);
    void (*apply_post_process_effect)(uint8_t effect_index);
    void (*draw_sprite)(uint8_t sprite_index, int16_t pos_x, int16_t pos_y, float angle);
    void (*draw_puppet)(Puppet *puppet);
} IPainter;

#endif