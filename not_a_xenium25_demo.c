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
#define TEST 1

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
    set_sys_clock_khz(300000, true);

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
#if TEST == 0
    Mesh *cube = meshFactory->create_textured_mesh(3, 3);
    cube->transformations = add_transformation(cube->transformations, &cube->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
#else
    Mesh *cube = meshFactory->create_textured_mesh(5, 5);
    cube->transformations = add_transformation(cube->transformations, &cube->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
#endif
    lightFactory = get_lightFactory();
    PointLight *pointLight = lightFactory->create_point_light(0.0f, 0.0f, 10.0f, 1.0f, 0xffff);

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

    BoneAnimation talking[2] = {
        {
            .bone = mascotJaw,
            .animation = get_animation_by_label("jawAnimation"),
        },
        {
            .bone = mascotSkull,
            .animation = get_animation_by_label("skullAnimation"),
        },
    };
    BoneAnimation raisingArm[2] = {
        {
            .bone = mascotArm,
            .animation = get_animation_by_label("raisingArmParent"),
        },
        {
            .bone = mascotElbow,
            .animation = get_animation_by_label("raisingElbow"),
        },
    };
    BoneAnimation waving[1] = {
        {
            .bone = mascotElbow,
            .animation = get_animation_by_label("waving"),
        },
    };

    const Bone *mascotHand = get_bone_by_name(&mascot->bones[0], "mascotArmHand");

    const Sprite *brcr_logo = get_sprite(11);
    const Sprite *arcadnis = get_sprite(12);
    const Sprite *yamifive = get_sprite(13);

    Gradient *sunrise = get_gradient_by_index(0);
    uint16_t spriteHeight = 344;
    const Sprite *sun = get_sprite(14);
    const Sprite *sunFace = get_sprite(15);
    int16_t burningEarthX = 304;
    int16_t burningEarthY = 0;
    const Sprite *burningEarth[3] = {get_sprite(21), get_sprite(22), get_sprite(23)};

    Mesh *earth = meshFactory->create_textured_mesh(4, 4);
    earth->transformations = add_transformation(earth->transformations, &earth->transformationsNum, 1.57f, 0.0f, 0.0f, 10.0f, 0);
    earth->transformations = add_transformation(earth->transformations, &earth->transformationsNum, 0, 0.0f, 0.0f, 10.0f, 0);

    const Sprite *skull_logo_white = get_sprite(24);
    uint8_t glowRange = 10;
    int16_t textHeight = 256;
    while (1)
    {
#if TEST == 0
        if (t < 1)
        {
            sleep_ms(4782);
            painter->draw_sprite(brcr_logo, 0, 40, 0, 2);
        }
        else if (t < 2)
        {
            sleep_ms(5015);
            painter->draw_sprite(yamifive, 0, 40, 0, 2);
        }
        else if (t < 3)
        {
            sleep_ms(4897);
            painter->draw_sprite(arcadnis, 0, 40, 0, 2);
        }
        else if (t < 4)
        {
            sleep_ms(5015);
            painter->print("First presented", 8, 112, 2);
            painter->print("at", 108, 144, 2);
            painter->print("Xenium 2025", 28, 176, 2);
        }
        else if (t < 5)
            sleep_ms(5015);
        else if (t >= 5 && t <= 290) // 275
        {
            float qt = t * 0.2f;
            if (t < 110)
            {
                move_gradient(sunrise, 3);
                spriteHeight--;
            }
            if (t == 125)
                sunFace = get_sprite(16);
            else if (t == 145)
                sunFace = get_sprite(17);
            else if (t == 175)
                sunFace = get_sprite(18);
            else if (t == 255)
                sunFace = get_sprite(19);
            else if (t == 275)
                sunFace = get_sprite(20);
            if (t > 175 && t < 255)
            {
                burningEarthX -= 5;
                burningEarthY++;
            }
            if (t < 275)
                painter->draw_gradient(sunrise);
            else
                painter->clear_buffer(0xe0);
            painter->draw_sprite(sun, 48, spriteHeight, radian_to_index(float_to_fixed(qt)), 1);
            painter->draw_sprite(sunFace, 88, spriteHeight + 32, 0, 1);
            painter->draw_sprite(burningEarth[t % 3], burningEarthX, burningEarthY, 0, 1);
            if (t == 290)
            {
                painter->clear_buffer(0);
                painter->draw_sprite(skull_logo_white, 55, 95, 0, 1);
                uint16_t glowParams[2] = {glowRange, 0xffff};
                painter->apply_post_process_effect(1, glowParams);
                painter->apply_post_process_effect(2, NULL);
                painter->override_buffer(1, 320);
                spriteHeight = 0;
            }
            if (t > 287)
            {
                painter->clear_buffer(0xff);
            }
        }
        else if (t > 290 && t <= 470)
        {
            if (spriteHeight < 320)
            {
                float qt = t * 0.2f;
                modify_transformation(earth->transformations, -qt, 10.0f, 0.0f, 0.0f, 1);
                renderer->draw_model(earth, pointLight, camera);
            }
            if (t > 325 && spriteHeight < 320)
            {
                spriteHeight += 15;
                painter->override_buffer(0, spriteHeight);
            }
            else if (spriteHeight >= 320 && t < 345)
            {
                painter->draw_sprite(skull_logo_white, 55, 95, 0, 1);
                uint16_t glowParams[2] = {glowRange, 0xffff};
                painter->apply_post_process_effect(1, glowParams);
                painter->apply_post_process_effect(2, NULL);
                painter->override_buffer(1, 320);
            }
            // strange, but interesting
            else if (t >= 345 && t < 375)
            {
                painter->override_buffer(0, 320);
                painter->apply_post_process_effect(3, NULL);
            }
            else if (t >= 375)
            {
                uint16_t startFrame = 375;
                int16_t textsCoords[6] = {24, textHeight, 37, textHeight, 47, textHeight};
                if (t < startFrame + 20)
                    textsCoords[1] = 456 - (t - startFrame) * 10; // 256 630..640 256-t640 456-t630
                if (t <= startFrame + 35)
                    textsCoords[2] = 527 - (t - startFrame) * 14; // 37 630..645 37-t645 347-t630
                if (t <= startFrame + 40)
                    textsCoords[4] = 607 - (t - startFrame) * 14; // 47 630..650 47-t650 427-t630
                transform_bone(logoSkullCenter, 0, 0, 0.05f);
                update_world_matrices(logoSkull);
                painter->draw_puppet(logoSkull);
                painter->print("CorpseTravel", textsCoords[0], textsCoords[1], 2);
                painter->print("We take you to places", textsCoords[2], textsCoords[3] + 32, 1);
                painter->print("not ALIVE anymore!", textsCoords[4], textsCoords[5] + 48, 1);
            }
        }
        else if (t > 470 && t <= 650)
            painter->clear_buffer(0);
        else
        {
            float qt = t * 0.2f;
            // renderer->draw_model(skybox, pointLight, camera);
            animate_bones(talking, 2, t);
            update_world_matrices(mascot);
            modify_transformation(cube->transformations, qt, 10.0f, 10.0f, 10.0f, 0);
            // renderer->draw_model(cube, pointLight, camera);
            painter->draw_puppet(mascot);
        }
#else
        if (t == 0)
            change_sprite(mascotHand, get_sprite(29));
        animate_bones(talking, 2, t);
        if (t < 15)
            animate_bones(raisingArm, 2, t);
        else
            animate_bones(waving, 1, t - 15);
        update_world_matrices(mascot);
        painter->draw_puppet(mascot);
        if(t<9)
            painter->fade(0,0,t);
        else if(t>=9 && t<18)
            painter->fade(1,9,t);
#endif
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