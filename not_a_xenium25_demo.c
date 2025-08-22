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
#define TEST 0

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
    // Mesh *mug = meshFactory->create_colored_mesh(0xff00, 0);
#if TEST == 0
    Mesh *mug = meshFactory->create_textured_mesh(3, 3);
    mug->transformations = add_transformation(mug->transformations, &mug->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
    Mesh *casette = meshFactory->create_textured_mesh(5, 5);
    casette->transformations = add_transformation(casette->transformations, &casette->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
    Mesh *atari = meshFactory->create_textured_mesh(6, 6);
    atari->transformations = add_transformation(atari->transformations, &atari->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
    Mesh *pizza = meshFactory->create_textured_mesh(7, 7);
    pizza->transformations = add_transformation(pizza->transformations, &pizza->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
    Mesh *win95 = meshFactory->create_textured_mesh(8, 8);
    win95->transformations = add_transformation(win95->transformations, &win95->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
    Mesh *duck = meshFactory->create_textured_mesh(9, 9);
    duck->transformations = add_transformation(duck->transformations, &duck->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
#else
    // Mesh *mug = meshFactory->create_textured_mesh(6, 6);
    // mug->transformations = add_transformation(mug->transformations, &mug->transformationsNum, 0, 0.0f, 5.0f, 0.0f, 0);
    // mug->transformations = add_transformation(mug->transformations, &mug->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);

    Mesh *mug = meshFactory->create_textured_mesh(9, 9);
    mug->transformations = add_transformation(mug->transformations, &mug->transformationsNum, 0, 10.0f, 10.0f, 10.0f, 0);
#endif
    lightFactory = get_lightFactory();
    PointLight *pointLight = lightFactory->create_point_light(0.0f, 0.0f, 3.0f, 1.0f, 0xffff);
    PointLight *pointLight_pizza = lightFactory->create_point_light(-12.0f, 0.0f, 10.0f, 1.2f, 0xffff);
    PointLight *pointLight_coal = lightFactory->create_point_light(12.0f, 0.0f, 10.0f, 1.2f, 0xffff);

    cameraFactory = get_cameraFactory();
    Camera *camera = cameraFactory->create_camera(0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Mesh *skybox = meshFactory->create_textured_skybox(2);

    painter->clear_buffer(0x1100);
    painter->draw_buffer();
    uint32_t t = 0;

    puppetFactory = get_puppetFactory();
    Puppet *mascot = puppetFactory->create_puppet(0);
    move_puppet(mascot, 60, 550);
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

    Bone *mascotHand = get_bone_by_name(&mascot->bones[0], "mascotArmHand");

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

    const Sprite *logo_dark = get_sprite(30);

    const Scroller *broken_earth = get_scroller_by_index(0);
    const Scroller *room = get_scroller_by_index(1);
    const Scroller *beach = get_scroller_by_index(2);
    const Scroller *vault = get_scroller_by_index(3);
    const Scroller *park = get_scroller_by_index(4);
    const Scroller *froggy = get_scroller_by_index(5);
    const Scroller *bath = get_scroller_by_index(6);

    const Sprite *promo1 = get_sprite(31);
    const Sprite *promo2 = get_sprite(32);

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
        else if (t > 290 && t <= 478)
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
                painter->fade_fullscreen(1, 470, t);
            }
        }
        else if (t > 478 && t < 510)
        {
            painter->clear_buffer(0);
            if (t == 480)
            {
                painter->draw_sprite(logo_dark, 12, 52, 0, 2);
                painter->override_buffer(1, 320);
                painter->clear_buffer(0);
            }
        }
        else if (t >= 510 && t <= 1280)
        {
            uint16_t startFrame = 510;
            uint16_t textStartFrame = startFrame + 215;
            uint16_t textFactor = 35;
            if (t == startFrame)
                change_sprite(mascotHand, get_sprite(29));
            else if (t == (startFrame + 186))
                change_sprite(mascotHand, get_sprite(3));
            painter->override_buffer(0, 320);
            painter->draw_puppet(mascot);
            if (t > (startFrame + 15) && t <= (startFrame + 70))
                move_puppet(mascot, 0, -4);
            if (t > (startFrame + 75) && t <= (startFrame + 90))
            {
                animate_bones(raisingArm, 2, t, false);
                update_world_matrices(mascot);
            }
            else if (t > (startFrame + 90) && t <= (startFrame + 170))
            {
                animate_bones(waving, 1, t, false);
                update_world_matrices(mascot);
            }
            else if (t > (startFrame + 170) && t <= (startFrame + 185))
            {
                animate_bones(raisingArm, 2, t, true);
                update_world_matrices(mascot);
            }
            else if (t > (startFrame + 215))
            {
                animate_bones(talking, 2, t, true);
                update_world_matrices(mascot);
            }
            if (t < (startFrame + 8))
                painter->fade_fullscreen(0, startFrame, t);
            if (t > (textStartFrame + textFactor * 0) && t <= (textStartFrame + textFactor * 1))
            {
                painter->print("Are you bored of", 55, textHeight + 32, 1);
                painter->print("shifting bits all day?", 38, textHeight + 48, 1); // 1
            }
            else if (t > (textStartFrame + textFactor * 1) && t <= (textStartFrame + textFactor * 2))
            {
                painter->print("Do you need", 76, textHeight + 32, 1);
                painter->print("an update and reboot?", 37, textHeight + 48, 1); // 2
            }
            else if (t > (textStartFrame + textFactor * 2) && t <= (textStartFrame + textFactor * 3))
            {
                painter->print("Defragment your circuits", 26, textHeight + 32, 1);
                painter->print("with us!", 88, textHeight + 48, 1); // 3
            }
            else if (t > (textStartFrame + textFactor * 3) && t <= (textStartFrame + textFactor * 4))
            {
                painter->print("CorpseTravel-the number zero", 7, textHeight + 32, 1);
                painter->print("travel agency for machines", 17, textHeight + 48, 1); // 4
            }
            else if (t > (textStartFrame + textFactor * 4) && t <= (textStartFrame + textFactor * 5))
            {
                painter->print("just like you", 71, textHeight + 32, 1); // 5
            }
            else if (t > (textStartFrame + textFactor * 5) && t <= (textStartFrame + textFactor * 6))
            {
                painter->print("offers trips like you", 40, textHeight + 32, 1);
                painter->print("have never seen before!", 28, textHeight + 48, 1); // 6
            }
            else if (t > (textStartFrame + textFactor * 6) && t <= (textStartFrame + textFactor * 7))
            {
                painter->draw_scroller(broken_earth, 120, 110, textStartFrame + textFactor * 6 + 5, t);
                painter->fade(1, textStartFrame + textFactor * 6 + 1, t, 120, 110, 100, 100);
                painter->print("Visit places where", 49, textHeight + 32, 1);
                painter->print("HUMANS once lived!", 45, textHeight + 48, 1); // 7
            }
            else if (t > (textStartFrame + textFactor * 7) && t <= (textStartFrame + textFactor * 8))
            {
                painter->draw_scroller(broken_earth, 120, 110, textStartFrame + textFactor * 6 + 5, t);
                painter->fade(0, textStartFrame + textFactor * 8 - 7, t, 120, 110, 100, 100);
                painter->print("Yes, HUMANS!", 66, textHeight + 32, 1);
                painter->print("Those legendary meat jellies!", 6, textHeight + 48, 1); // 8
            }
            else if (t > (textStartFrame + textFactor * 8) && t <= (textStartFrame + textFactor * 9))
            {
                painter->draw_scroller(room, 120, 110, textStartFrame + textFactor * 8 + 5, t);
                painter->fade(1, textStartFrame + textFactor * 8 + 1, t, 120, 110, 100, 100);
                painter->print("Luxury Human Interior!", 34, textHeight + 32, 1); // 9
            }
            else if (t > (textStartFrame + textFactor * 9) && t <= (textStartFrame + textFactor * 10))
            {
                painter->draw_scroller(room, 120, 110, textStartFrame + textFactor * 8 + 5, t);
                painter->fade(0, textStartFrame + textFactor * 10 - 7, t, 120, 110, 100, 100);
                painter->print("Because nothing says comfort", 9, textHeight + 32, 1);
                painter->print("like concrete and broken glass", 4, textHeight + 48, 1); // 10
            }
            else if (t > (textStartFrame + textFactor * 10) && t <= (textStartFrame + textFactor * 11))
            {
                painter->draw_scroller(beach, 120, 110, textStartFrame + textFactor * 10 + 5, t);
                painter->fade(1, textStartFrame + textFactor * 10 + 1, t, 120, 110, 100, 100);
                painter->print("Behold the Human Arks!", 31, textHeight + 32, 1); // 11
            }
            else if (t > (textStartFrame + textFactor * 11) && t <= (textStartFrame + textFactor * 12))
            {
                painter->draw_scroller(beach, 120, 110, textStartFrame + textFactor * 10 + 5, t);
                painter->fade(0, textStartFrame + textFactor * 12 - 7, t, 120, 110, 100, 100);
                painter->print("Built to save millions...", 26, textHeight + 32, 1);
                painter->print("...sank before saving one", 21, textHeight + 48, 1); // 12
            }
            else if (t > (textStartFrame + textFactor * 12) && t <= (textStartFrame + textFactor * 13))
            {
                painter->draw_scroller(vault, 120, 110, textStartFrame + textFactor * 12 + 5, t);
                painter->fade(1, textStartFrame + textFactor * 12 + 1, t, 120, 110, 100, 100);
                painter->print("The Ultimate Vault!", 45, textHeight + 32, 1);
                painter->print("Now permanently open", 37, textHeight + 48, 1); // 13
            }
            else if (t > (textStartFrame + textFactor * 13) && t <= (textStartFrame + textFactor * 14))
            {
                painter->draw_scroller(vault, 120, 110, textStartFrame + textFactor * 12 + 5, t);
                painter->fade(0, textStartFrame + textFactor * 14 - 7, t, 120, 110, 100, 100);
                painter->print("Bloodstains are part", 44, textHeight + 32, 1);
                painter->print("of the exhibition", 57, textHeight + 48, 1); // 14
            }
            else if (t > (textStartFrame + textFactor * 14) && t <= (textStartFrame + textFactor * 15))
            {
                painter->draw_scroller(park, 120, 110, textStartFrame + textFactor * 14 + 5, t);
                painter->fade(1, textStartFrame + textFactor * 14 + 1, t, 120, 110, 100, 100);
                painter->print("Funland Amusement Park", 27, textHeight + 32, 1);
                painter->print("Where laughter rusts forever", 9, textHeight + 48, 1); // 15
            }
            else if (t > (textStartFrame + textFactor * 15) && t <= (textStartFrame + textFactor * 16))
            {
                painter->draw_scroller(park, 120, 110, textStartFrame + textFactor * 14 + 5, t);
                painter->fade(0, textStartFrame + textFactor * 16 - 7, t, 120, 110, 100, 100);
                painter->print("Rides no longer operational", 16, textHeight + 32, 1);
                painter->print("Screams simulated", 51, textHeight + 48, 1); // 16
            }
            else if (t > (textStartFrame + textFactor * 16) && t <= (textStartFrame + textFactor * 17))
            {
                painter->draw_scroller(froggy, 120, 110, textStartFrame + textFactor * 16 + 5, t);
                painter->fade(1, textStartFrame + textFactor * 14 + 1, t, 120, 110, 100, 100);
                painter->print("Froggy", 95, textHeight + 32, 1);
                painter->print("Convenience Store", 54, textHeight + 48, 1); // 17
            }
            else if (t > (textStartFrame + textFactor * 17) && t <= (textStartFrame + textFactor * 18))
            {
                painter->draw_scroller(froggy, 120, 110, textStartFrame + textFactor * 16 + 5, t);
                painter->print("Shop, restaurant,", 54, textHeight + 32, 1);
                painter->print("pharmacy, post office...", 24, textHeight + 48, 1); // 18
            }
            else if (t > (textStartFrame + textFactor * 18) && t <= (textStartFrame + textFactor * 19))
            {
                painter->draw_scroller(froggy, 120, 110, textStartFrame + textFactor * 16 + 5, t);
                painter->fade(0, textStartFrame + textFactor * 19 - 7, t, 120, 110, 100, 100);
                painter->print("...and maybe a gas station", 17, textHeight + 32, 1); // 19
            }
            else if (t > (textStartFrame + textFactor * 19) && t <= (textStartFrame + textFactor * 20))
            {
                painter->draw_scroller(bath, 120, 110, textStartFrame + textFactor * 19 + 5, t);
                painter->fade(1, textStartFrame + textFactor * 19 + 1, t, 120, 110, 100, 100);
                painter->print("Relax like a Human", 48, textHeight + 32, 1); // 20
            }
            else if (t > (textStartFrame + textFactor * 20) && t <= (textStartFrame + textFactor * 21))
            {
                painter->draw_scroller(bath, 120, 110, textStartFrame + textFactor * 19 + 5, t);
                painter->print("The bath is gone...", 45, textHeight + 32, 1);
                painter->print("but the bath remains", 41, textHeight + 48, 1); // 21
            }
            else if (t > (textStartFrame + textFactor * 21) && t <= (textStartFrame + textFactor * 22))
            {
                painter->draw_scroller(bath, 120, 110, textStartFrame + textFactor * 19 + 5, t);
                painter->fade(0, textStartFrame + textFactor * 22 - 7, t, 120, 110, 100, 100);
                painter->print("Water not included", 50, textHeight + 32, 1); // 22
            }
        }
        else if (t > 1280 && t <= 2000)
        {
            uint32_t start_frame = 1280;
            uint16_t textFactor = 60;
            painter->override_buffer(0, 320);
            float qt = t * 0.2f;
            if (t > start_frame && t <= (start_frame + textFactor * 1))
            {
                animate_bones(talking, 2, t, true);
                update_world_matrices(mascot);
                painter->draw_puppet(mascot);
                painter->print("Authentic human trash", 37, textHeight + 32, 1);
                painter->print("sorry, souvenirs!", 53, textHeight + 48, 1);
            }
            else if (t > (start_frame + textFactor * 1) && t <= (start_frame + textFactor * 2))
            {
                modify_transformation(win95->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(win95, pointLight_pizza, camera);
                painter->print("Windows95", 81, textHeight + 32, 1);
                painter->print("A veeery original edition", 25, textHeight + 48, 1);
            }
            else if (t > (start_frame + textFactor * 2) && t <= (start_frame + textFactor * 3))
            {
                modify_transformation(win95->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(win95, pointLight_pizza, camera);
                painter->print("Complete with", 68, textHeight + 32, 1);
                painter->print("handwritten CD-KEY!", 43, textHeight + 48, 1);
            }
            else if (t > (start_frame + textFactor * 3) && t <= (start_frame + textFactor * 4))
            {
                modify_transformation(casette->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(casette, pointLight_pizza, camera);
                painter->print("Magnetic Tape", 67, textHeight + 32, 1);
            }
            else if (t > (start_frame + textFactor * 4) && t <= (start_frame + textFactor * 5))
            {
                modify_transformation(casette->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(casette, pointLight_pizza, camera);
                painter->print("Storing data and fungi", 36, textHeight + 32, 1);
                painter->print("since January 1st, 1970!", 27, textHeight + 48, 1);
            }
            else if (t > (start_frame + textFactor * 5) && t <= (start_frame + textFactor * 6))
            {
                modify_transformation(atari->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(atari, pointLight_pizza, camera);
                painter->print("Bag of coal", 77, textHeight + 32, 1);
            }
            else if (t > (start_frame + textFactor * 6) && t <= (start_frame + textFactor * 7))
            {
                modify_transformation(atari->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(atari, pointLight_pizza, camera);
                painter->print("When power failed,", 46, textHeight + 32, 1);
                painter->print("they tried coal", 64, textHeight + 48, 1);
            }
            else if (t > (start_frame + textFactor * 7) && t <= (start_frame + textFactor * 8))
            {
                modify_transformation(pizza->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(pizza, pointLight_pizza, camera);
                painter->print("Pizza slice", 78, textHeight + 32, 1);
            }
            else if (t > (start_frame + textFactor * 8) && t <= (start_frame + textFactor * 9))
            {
                modify_transformation(pizza->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(pizza, pointLight_pizza, camera);
                painter->print("Humans called it 'gourmet'", 21, textHeight + 32, 1);
                painter->print("We call it 'biohazard'", 39, textHeight + 48, 1);
            }
            else if (t > (start_frame + textFactor * 9) && t <= (start_frame + textFactor * 10))
            {
                modify_transformation(mug->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(mug, pointLight_pizza, camera);
                painter->print("Mysterious Human Vessel", 26, textHeight + 32, 1);
            }
            else if (t > (start_frame + textFactor * 10) && t <= (start_frame + textFactor * 11))
            {
                modify_transformation(mug->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(mug, pointLight_pizza, camera);
                painter->print("Purpose: unknown", 53, textHeight + 32, 1);
                painter->print("Probably sacred", 60, textHeight + 48, 1);
            }
            else if (t > (start_frame + textFactor * 11) && t <= (start_frame + textFactor * 12))
            {
                modify_transformation(duck->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(duck, pointLight_pizza, camera);
                painter->print("Rubber Duck", 75, textHeight + 32, 1);
            }
            else if (t > (start_frame + textFactor * 12) && t <= (start_frame + textFactor * 13))
            {
                modify_transformation(duck->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
                renderer->draw_model(duck, pointLight_pizza, camera);
                painter->print("Humans debugged with this", 20, textHeight + 32, 1);
                painter->print("No wonder they are extinct", 17, textHeight + 48, 1);
            }
        }
        else if (t > 2000 && t <= 2200)
        {
            float qt = t * 0.2f;
            uint32_t start_frame = 2000;
            uint16_t textFactor = 60;
            painter->override_buffer(0, 320);
            animate_bones(talking, 2, t, false);
            painter->draw_puppet(mascot);
            int32_t spriteRotPattern[10] = {2140, 2140, 2140, 2140, 2140, 1075, 1075, 1075, 1075, 1075};
            if (t > (start_frame + textFactor * 0) && t <= (start_frame + textFactor * 1))
            {
                update_world_matrices(mascot);
                painter->print("Super promo: three worlds", 18, textHeight + 32, 1);
                painter->print("for the price of one!", 38, textHeight + 48, 1);
                painter->draw_sprite(promo1, 112, 30, spriteRotPattern[t % 10], 2);
            }
            else if (t > (start_frame + textFactor * 1) && t <= (start_frame + textFactor * 2))
            {
                update_world_matrices(mascot);
                painter->print("Bring a fellow robot and get a free", 11, textHeight + 32, 1);
                painter->print("a free Barbecue Hunt voucher!", 5, textHeight + 48, 1);
                painter->draw_sprite(promo2, 112, 30, spriteRotPattern[t % 10], 2);
            }
            else if (t > (start_frame + textFactor * 2) && t <= (start_frame + textFactor * 3))
            {
                update_world_matrices(mascot);
                painter->print("CorspeTravel - always", 35, textHeight + 32, 1);
                painter->print("number 0, never number 1!", 16, textHeight + 48, 1);
            }
        }
        else
        {
            if (t < 2210)
                textHeight = 340;
            painter->print("Thank you for watching!", 27.0, textHeight + 0, 1);
            painter->print("That was the latest production", 4.0, textHeight + 16, 1);
            painter->print("of Aberration Creations called", 5.0, textHeight + 32, 1);
            painter->print("CorpseTravel", 72.0, textHeight + 48, 1);
            painter->print("first presented at Xenium 25", 11.0, textHeight + 64, 1);
            painter->print("", 120.0, textHeight + 80, 1);
            painter->print("Code & GFX", 79.0, textHeight + 96, 1);
            painter->print("YamiFive", 86.0, textHeight + 112, 1);
            painter->print("Music", 99.0, textHeight + 128, 1);
            painter->print("Arcadnis", 88.0, textHeight + 144, 1);
            painter->print("Support", 93.0, textHeight + 160, 1);
            painter->print("Mystix", 96.0, textHeight + 176, 1);
            painter->print("", 120.0, textHeight + 192, 1);
            painter->print("Thank you Yoshitaka", 44.0, textHeight + 208, 1);
            painter->print("for soldering everything", 28.0, textHeight + 224, 1);
            painter->print("together!", 86.0, textHeight + 240, 1);
            painter->print("", 120.0, textHeight + 256, 1);
            painter->print("Greetings", 86.0, textHeight + 272, 1);
            painter->print("", 120.0, textHeight + 288, 1);
            painter->print("ALTAIR", 94.0, textHeight + 304, 1);
            painter->print("ABYSS CONNECTION", 52.0, textHeight + 320, 1);
            painter->print("ADDICT", 96.0, textHeight + 336, 1);
            painter->print("AGENDA", 93.0, textHeight + 352, 1);
            painter->print("AMNESTY", 86.0, textHeight + 368, 1);
            painter->print("ANADUNE", 87.0, textHeight + 384, 1);
            painter->print("APPENDIX", 86.0, textHeight + 400, 1);
            painter->print("ARISE", 100.0, textHeight + 416, 1);
            painter->print("ARTWAY", 89.0, textHeight + 432, 1);
            painter->print("ASD", 107.0, textHeight + 448, 1);
            painter->print("ASTROIDEA", 82.0, textHeight + 464, 1);
            painter->print("BONZAI", 95.0, textHeight + 480, 1);
            painter->print("BRAINSTORM", 75.0, textHeight + 496, 1);
            painter->print("CNCD", 103.0, textHeight + 512, 1);
            painter->print("COCOON", 95.0, textHeight + 528, 1);
            painter->print("CONSPIRACY", 77.0, textHeight + 544, 1);
            painter->print("DAMAGE", 92.0, textHeight + 560, 1);
            painter->print("DESIRE", 97.0, textHeight + 576, 1);
            painter->print("DIGITAL DYNAMITE", 54.0, textHeight + 592, 1);
            painter->print("DILEMMA", 89.0, textHeight + 608, 1);
            painter->print("DLG CREW", 85.0, textHeight + 624, 1);
            painter->print("DREAMWEB", 82.0, textHeight + 640, 1);
            painter->print("ELUDE", 99.0, textHeight + 656, 1);
            painter->print("ELYSIUM", 90.0, textHeight + 672, 1);
            painter->print("EXCEED", 96.0, textHeight + 688, 1);
            painter->print("FAIRLIGHT", 84.0, textHeight + 704, 1);
            painter->print("FAITH DESIGN", 72.0, textHeight + 720, 1);
            painter->print("FARBRAUSCH", 74.0, textHeight + 736, 1);
            painter->print("FCI", 109.0, textHeight + 752, 1);
            painter->print("FLOPPY", 92.0, textHeight + 768, 1);
            painter->print("FUTURIS", 89.0, textHeight + 784, 1);
            painter->print("GENESIS PROJECT", 58.0, textHeight + 800, 1);
            painter->print("GHOSTOWN", 84.0, textHeight + 816, 1);
            painter->print("HAUJOBB", 90.0, textHeight + 832, 1);
            painter->print("JOKER", 98.0, textHeight + 848, 1);
            painter->print("KVASIGEN", 86.0, textHeight + 864, 1);
            painter->print("LAMERS", 92.0, textHeight + 880, 1);
            painter->print("LEMON", 97.0, textHeight + 896, 1);
            painter->print("LEPSI.DE", 89.0, textHeight + 912, 1);
            painter->print("LETHARGY", 84.0, textHeight + 928, 1);
            painter->print("LNX", 107.0, textHeight + 944, 1);
            painter->print("LOGICOMA", 87.0, textHeight + 960, 1);
            painter->print("MADWIZARDS", 74.0, textHeight + 976, 1);
            painter->print("MELON", 97.0, textHeight + 992, 1);
            painter->print("MFX", 105.0, textHeight + 1008, 1);
            painter->print("NAH-KOLOR", 80.0, textHeight + 1024, 1);
            painter->print("NETRO", 97.0, textHeight + 1040, 1);
            painter->print("NG", 111.0, textHeight + 1056, 1);
            painter->print("NUANCE", 92.0, textHeight + 1072, 1);
            painter->print("OB5VR", 98.0, textHeight + 1088, 1);
            painter->print("ODBYT DESIGN", 71.0, textHeight + 1104, 1);
            painter->print("OFTENHIDE", 83.0, textHeight + 1120, 1);
            painter->print("PADAWANS", 81.0, textHeight + 1136, 1);
            painter->print("POO BRAIN", 82.0, textHeight + 1152, 1);
            painter->print("PRISMBEINGS", 75.0, textHeight + 1168, 1);
            painter->print("RADIANCE", 86.0, textHeight + 1184, 1);
            painter->print("RAZOR 1911", 79.0, textHeight + 1200, 1);
            painter->print("REBELS", 95.0, textHeight + 1216, 1);
            painter->print("REVISION", 89.0, textHeight + 1232, 1);
            painter->print("SAMAR PRODUCTIONS", 44.0, textHeight + 1248, 1);
            painter->print("SATELLITE", 83.0, textHeight + 1264, 1);
            painter->print("SATORI", 95.0, textHeight + 1280, 1);
            painter->print("SPACEBALLS", 77.0, textHeight + 1296, 1);
            painter->print("SPECCY.PL", 81.0, textHeight + 1312, 1);
            painter->print("STILL", 101.0, textHeight + 1328, 1);
            painter->print("SUNDIAL AEON", 69.0, textHeight + 1344, 1);
            painter->print("SWEET16", 90.0, textHeight + 1360, 1);
            painter->print("TBL", 107.0, textHeight + 1376, 1);
            painter->print("TGD", 107.0, textHeight + 1392, 1);
            painter->print("TIFECO", 96.0, textHeight + 1408, 1);
            painter->print("TRISTESSE", 83.0, textHeight + 1424, 1);
            painter->print("TRSI", 104.0, textHeight + 1440, 1);
            painter->print("UMLAUT DESIGN", 63.0, textHeight + 1456, 1);
            painter->print("UNITED FORCE", 69.0, textHeight + 1472, 1);
            painter->print("WHELPZ", 92.0, textHeight + 1488, 1);
            textHeight = textHeight - 2;
        }
#else
        // float qt = t * 0.2f;
        // modify_transformation(mug->transformations, -qt, 10.0f, 0.0f, 0.0f, 0);
        // renderer->draw_model(mug, pointLight_pizza, camera);

        int32_t spriteRotPattern[10] = {2140, 2140, 2140, 2140, 2140, 1075, 1075, 1075, 1075, 1075};
        painter->draw_sprite(promo1, 112, 30, spriteRotPattern[t % 10], 2);
        // coal
        //  modify_transformation(mug->transformations, -qt, 10.0f, 0.0f, 0.0f, 1);
        //  renderer->draw_model(mug, pointLight_coal, camera);
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
    fileReader->play_wave_file("kostek.wav");
    fileReader->play_wave_file("kosteke.wav");
}