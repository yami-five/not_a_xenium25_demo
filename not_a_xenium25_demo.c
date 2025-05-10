#include <stdio.h>
#include "pico/multicore.h"

#include "ICameraFactory.h"
#include "IDisplay.h"
#include "IFileReader.h"
#include "IHardware.h"
#include "ILightFactory.h"
#include "IMeshFactory.h"
#include "IPainter.h"
#include "IRenderer.h"

#include "cameraFactory.h"
#include "display.h"
#include "fileReader.h"
#include "hardware.h"
#include "lightFactory.h"
#include "meshFactory.h"
#include "painter.h"
#include "renderer.h"
#include "mesh.h"

static const IHardware *hardware_core0;
static const IHardware *hardware_core1;
static const IDisplay *display;
static const IPainter *painter;
static const IFileReader *fileReader;
static const IRenderer *renderer;
static const IMeshFactory *meshFactory;
static const ILightFactory *lightFactory;
static const ICameraFactory *cameraFactory;

void core1_main();

int main()
{
    hardware_core0 = get_hardware();
    hardware_core0->init_hardware();

    display = get_display();
    display->init_display(hardware_core0);

    painter = get_painter();
    painter->init_painter(display, hardware_core0);

    renderer = get_renderer();
    renderer->init_renderer(hardware_core0, painter);

    meshFactory = get_meshFactory();
    Mesh *cube = meshFactory->create_colored_mesh(0x00ff00, 0);
    cube->transformations = add_transformation(cube->transformations, &cube->transformationsNum, 0, 0, 0, 0);

    lightFactory = get_lightFactory();
    PointLight *pointLight = lightFactory->create_point_light(0.0f, 0.0f, 50.0f, 1.0f, 0xf800);

    cameraFactory = get_cameraFactory();
    Camera *camera = cameraFactory->create_camera(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // fileReader->play_wave_file("test.wav");
    multicore_launch_core1(core1_main);
    painter->clear_buffer();
    painter->draw_buffer();
    uint32_t t = 0;
    while (1)
    {
        float qt = t * 0.1f;
        modify_transformation(cube->transformations, qt, qt, qt, 0);
        renderer->draw_model(cube, pointLight, camera);
        painter->draw_buffer();
        t++;
        renderer->clear_zbuffer();
        painter->clear_buffer();
    }
}

void core1_main()
{
    // hardware_core1 = get_hardware();
    hardware_core0->init_audio_i2s();
    fileReader = get_fileReader();
    fileReader->init_fileReader(hardware_core0);
    fileReader->play_wave_file("test.wav");
}