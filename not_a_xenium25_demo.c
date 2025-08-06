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
#include "IPuppetFactory.h"

#include "cameraFactory.h"
#include "display.h"
#include "fileReader.h"
#include "hardware.h"
#include "lightFactory.h"
#include "meshFactory.h"
#include "painter.h"
#include "renderer.h"
#include "mesh.h"
#include "puppetFactory.h"
#include "puppet.h"

#define PICO_MODE 0

static const IHardware *hardware_core;
static const IDisplay *display;
static const IPainter *painter;
static const IFileReader *fileReader;
static const IRenderer *renderer;
static const IMeshFactory *meshFactory;
static const ILightFactory *lightFactory;
static const ICameraFactory *cameraFactory;
static const IPuppetFactory *puppetFactory;

void core1_main();

int main()
{
    set_sys_clock_khz(200000, true);

    hardware_core = get_hardware();
    hardware_core->init_hardware();

    display = get_display();
    display->init_display(hardware_core);

#if PICO_MODE==0

    painter = get_painter();
    painter->init_painter(display, hardware_core);

    renderer = get_renderer();
    renderer->init_renderer(hardware_core, painter);

    meshFactory = get_meshFactory();
    // Mesh *cube = meshFactory->create_colored_mesh(0xff00, 0);
    Mesh *cube = meshFactory->create_textured_mesh(3, 3);
    cube->transformations = add_transformation(cube->transformations, &cube->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);

    lightFactory = get_lightFactory();
    PointLight *pointLight = lightFactory->create_point_light(0.0f, 0.0f, 50.0f, 1.5f, 0xffff);

    cameraFactory = get_cameraFactory();
    Camera *camera = cameraFactory->create_camera(0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Mesh *skybox = meshFactory->create_textured_skybox(2);

    painter->clear_buffer(0x1100);
    painter->draw_buffer();
    uint32_t t = 0;

    puppetFactory = get_puppetFactory();
    Puppet *mascot = puppetFactory->create_puppet(0);
    move_puppet(mascot, 120, 330);
    Bone *mascotSkull = get_bone_by_name(&mascot->bones[0], "mascotSkullParent");
    Bone *mascotJaw = get_bone_by_name(&mascot->bones[0], "mascotSkullJawParent");
    Bone *mascotArm = get_bone_by_name(&mascot->bones[0], "mascotArmParent");
    Bone *mascotElbow = get_bone_by_name(&mascot->bones[0], "mascotArmElbow");
    // transform_bone(mascotElbow,0,0,1.0f);
    // update_world_matrices(mascot);

    BoneAnimation boneAnimations[2] = {
        {
            .bone = mascotJaw,
            .animation = get_animation_by_label("jawAnimation"),
        },
        {
            .bone = mascotSkull,
            .animation = get_animation_by_label("skullAnimation"),
        },
    };

    while (1)
    {
        float qt = t * 0.2f;
        painter->print("dupa",0,0);
        // renderer->draw_model(skybox, pointLight, camera);
        animate_bones(boneAnimations, 2, t);
        update_world_matrices(mascot);
        modify_transformation(cube->transformations, qt, 10.0f, 10.0f, 10.0f, 0);
        // renderer->draw_model(cube, pointLight, camera);
        painter->draw_puppet(mascot);
        painter->apply_post_process_effect(0);
        // painter->draw_sprite(0,200,300,qt*2);
        // painter->draw_sprite(0,5,30,-qt);
        // painter->draw_sprite(0,15,5,qt);

        painter->draw_buffer();
        t++;
        renderer->clear_zbuffer();
        painter->clear_buffer(0x1100);
    }
#else
    multicore_launch_core1(core1_main);
#endif
}

void core1_main()
{
    hardware_core->init_audio_i2s();
    fileReader = get_fileReader();
    fileReader->init_fileReader(hardware_core);
    fileReader->play_wave_file("kostek_5.wav");
}