#ifndef IRENDERER_h
#define IRENDERER_h

#include <stdint.h>
#include "IPainter.h"
#include "IHardware.h"

typedef struct
{
    void (*init_renderer)(const IHardware * hardware, const IPainter * painter);
} IRenderer;

#endif