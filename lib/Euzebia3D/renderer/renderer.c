#include "IRenderer.h"
#include "renderer.h"
#include "../shared/gfx.h"

static const IHardware *_hardware = NULL;
static const IPainter * _painter = NULL;

void init_renderer(const IHardware *hardware, const IPainter * painter)
{
    _hardware = hardware;
    _painter = painter;
}



static IRenderer renderer = {
    .init_renderer = init_renderer
};

const IRenderer *get_renderer(void)
{
    return &renderer;
}