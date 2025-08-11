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

#if PICO_MODE == 0

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
    Puppet *logoSkull = puppetFactory->create_puppet(1);
    Bone *logoSkullCenter = get_bone_by_name(&logoSkull->bones[0], "logoCenter");

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

    const Sprite *brcr_logo = get_sprite(11);
    const Sprite *arcadnis = get_sprite(12);
    const Sprite *yamifive = get_sprite(13);

    Gradient *sunrise = get_gradient_by_index(0);
    uint16_t spriteHeight = 334;
    const Sprite *sun = get_sprite(14);
    const Sprite *sunFace = get_sprite(15);
    int16_t burningEarthX = 304;
    int16_t burningEarthY = 0;
    const Sprite *burningEarth[3] = {get_sprite(21), get_sprite(22), get_sprite(23)};

    Mesh *earth = meshFactory->create_textured_mesh(4, 4);
    earth->transformations = add_transformation(earth->transformations, &earth->transformationsNum, 1.57f, 0.0f, 0.0f, 10.0f, 0);
    earth->transformations = add_transformation(earth->transformations, &earth->transformationsNum, 0, 0.0f, 0.0f, 10.0f, 0);

    const Sprite *skull_logo_white = get_sprite(24);

    int16_t textHeight=256;
    while (1)
    {
        if (t <= 62)
        {
            painter->draw_sprite(brcr_logo, 0, 40, 0, 2);
        }
        else if (t > 62 && t <= 126)
        {
            painter->draw_sprite(yamifive, 0, 40, 0, 2);
        }
        else if (t > 126 && t <= 200)
        {
            painter->draw_sprite(arcadnis, 0, 40, 0, 2);
        }
        else if (t > 126 && t <= 200)
        {
            painter->draw_sprite(arcadnis, 0, 40, 0, 2);
        }
        else if (t > 200 && t <= 280)
        {
            painter->print("First presented", 8, 112, 2);
            painter->print("at", 108, 144, 2);
            painter->print("Xenium 2025", 28, 176, 2);
        }
        else if (t > 280 && t <= 565)
        {
            float qt = t * 0.2f;
            if (t < 376)
            {
                move_gradient(sunrise, 3);
                spriteHeight--;
            }
            if (t == 400)
                sunFace = get_sprite(16);
            else if (t == 420)
                sunFace = get_sprite(17);
            else if (t == 450)
                sunFace = get_sprite(18);
            else if (t == 530)
                sunFace = get_sprite(19);
            else if (t == 550)
                sunFace = get_sprite(20);
            if(t>450 && t <530)
            {
                burningEarthX-=5;
                burningEarthY++;
            }
            if(t<550)
                painter->draw_gradient(sunrise);
            else
                painter->clear_buffer(0xe0);
            painter->draw_sprite(sun, 48, spriteHeight, radian_to_index(float_to_fixed(qt)), 1);
            painter->draw_sprite(sunFace, 88, spriteHeight + 32, 0, 1);
            painter->draw_sprite(burningEarth[t%3],burningEarthX,burningEarthY,0,1);
            if(t==565)
            {
                painter->clear_buffer(0);
                painter->draw_sprite(skull_logo_white,55,95,0,1);
                painter->override_buffer(1,340);
                spriteHeight=0;
            }
            if(t>562)
                painter->clear_buffer(0xff);
        }
        else if(t > 565 && t <= 960)
        {
            if(spriteHeight<320)
            {
                float qt = t * 0.2f;
                modify_transformation(earth->transformations, -qt, 10.0f, 0.0f, 0.0f, 1);
                renderer->draw_model(earth, pointLight, camera);
            }
            if(t>600 && spriteHeight<320)
            {
                spriteHeight+=15;
                painter->override_buffer(0,spriteHeight);
            }
            else if(spriteHeight>=320 && t<630)
            {
                painter->draw_sprite(skull_logo_white,55,95,0,1);
            }
            else if(t>=630 && t<640)
            {
                painter->clear_buffer(0xff);
            }
            else if(t>=640)
            {
                transform_bone(logoSkullCenter,0,0,0.05f);
                update_world_matrices(logoSkull);
                painter->draw_puppet(logoSkull);
                painter->print("CorpseTravel",24,textHeight,2);
                painter->print("We take you to places",37,textHeight+32,1);
                painter->print("not ALIVE anymore!",47,textHeight+48,1);               
            }
        }
        else
        {
            float qt = t * 0.2f;
            // renderer->draw_model(skybox, pointLight, camera);
            animate_bones(boneAnimations, 2, t);
            update_world_matrices(mascot);
            modify_transformation(cube->transformations, qt, 10.0f, 10.0f, 10.0f, 0);
            // renderer->draw_model(cube, pointLight, camera);
            painter->draw_puppet(mascot);
        }

        // painter->apply_post_process_effect(0);
        painter->draw_buffer();
        t++;
        renderer->clear_zbuffer();
        painter->clear_buffer(0);
        // painter->clear_buffer(0x11);
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