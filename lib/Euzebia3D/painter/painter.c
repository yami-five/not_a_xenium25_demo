#include "IPainter.h"
#include "painter.h"
#include "../shared/gfx.h"
#include "../shared/post_processing.h"
#include "../shared/sprites.h"
#include "hardware/sync/spin_lock.h"
#include "../arithmetics/fpa.h"

static const IHardware *_hardware = NULL;
static const IDisplay *_display = NULL;
static uint8_t buffer[BUFFER_SIZE];
static const uint16_t chunk_size = 240 * 16;
static spin_lock_t *lcd_spinlock;
static uint8_t scanline_offset = 0;

void dma_buffer_irq_handler()
{
    dma_hw->ints1 = 1u << dma_channel;
}

void init_dma()
{
    dma_channel = dma_claim_unused_channel(true);
    dma_channel_config config = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    channel_config_set_dreq(&config, spi_get_dreq(_hardware->get_spi_port(), true));
    dma_channel_configure(
        dma_channel,
        &config,
        &spi_get_hw(_hardware->get_spi_port())->dr,
        NULL,
        chunk_size,
        false);
    dma_channel_set_irq1_enabled(dma_channel, true);
    irq_set_exclusive_handler(DMA_IRQ_1, dma_buffer_irq_handler);
    irq_set_enabled(DMA_IRQ_1, true);
    channel_config_set_read_increment(&config, true);
    channel_config_set_write_increment(&config, false);
}

void init_painter(const IDisplay *display, const IHardware *hardware)
{
    _hardware = hardware;
    _display = display;
    init_dma();
    // lcd_spinlock=spin_lock_init(spin_lock_claim_unused(true));
}

void draw_buffer()
{
    uint32_t current_offset = 0;
    spin_lock_t *spi_spinlock = _hardware->get_spinlock();
    uint32_t flags = spin_lock_blocking(spi_spinlock);
    _hardware->write(SD_CS_PIN, 1);
    _hardware->write(LCD_CS_PIN, 0);
    _hardware->write(LCD_DC_PIN, 0);
    _hardware->spi_write_byte(0x2C);
    _hardware->write(LCD_DC_PIN, 1);
    spin_unlock(spi_spinlock, flags);
    while (current_offset < BUFFER_SIZE)
    {
        flags = spin_lock_blocking(spi_spinlock);
        _hardware->write(SD_CS_PIN, 1);
        _hardware->write(LCD_CS_PIN, 0);
        dma_channel_set_read_addr(dma_channel, buffer + current_offset, true);
        dma_channel_wait_for_finish_blocking(dma_channel);
        current_offset += chunk_size;
        spin_unlock(spi_spinlock, flags);
    }
}

void clear_buffer(uint16_t color)
{
    memset(buffer, color, sizeof(buffer));
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint32_t line_adr = (x * HEIGHT_DOUBLED) + (y * 2);
    buffer[line_adr] = (color >> 8) & 0xff;
    buffer[line_adr + 1] = color & 0xff;
}

void draw_image(uint8_t image_index)
{
    int dma_channel_flash = dma_claim_unused_channel(true);
    dma_channel_config config = dma_channel_get_default_config(dma_channel_flash);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    channel_config_set_read_increment(&config, true);
    channel_config_set_write_increment(&config, true);
    dma_channel_configure(
        dma_channel_flash,
        &config,
        buffer,
        get_image(image_index)->image,
        BUFFER_SIZE,
        false);
    dma_channel_start(dma_channel_flash);
    dma_channel_wait_for_finish_blocking(dma_channel_flash);
    dma_channel_unclaim(dma_channel_flash);
}

static inline uint8_t get_r(uint16_t c) { return (c >> 11) & 0x1F; }
static inline uint8_t get_g(uint16_t c) { return (c >> 5) & 0x3F; }
static inline uint8_t get_b(uint16_t c) { return c & 0x1F; }

static inline uint16_t make_rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
}

void crt_disp_effect()
{
    // barrel distortion
    uint8_t *framebuffer = (uint8_t *)malloc(sizeof(uint8_t) * BUFFER_SIZE);
    for (uint32_t i = 0; i < BUFFER_SIZE_HALF; i++)
    {
        uint32_t index = get_effect_table_element(0, i);
        framebuffer[i * 2] = buffer[index * 2];
        framebuffer[i * 2 + 1] = buffer[index * 2 + 1];
    }
    memcpy(buffer, framebuffer, BUFFER_SIZE);
    // chromatic aberration
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        uint32_t ydw = y * DISPLAY_WIDTH;
        for (int x = 0; x < DISPLAY_WIDTH; x++)
        {
            uint32_t i = ydw + x;

            uint16_t xr = (x > 0) ? x - 2 : x;
            uint16_t xg = (x < DISPLAY_WIDTH - 1) ? x + 2 : x;

            uint16_t ir = ydw + xr;
            uint16_t ig = ydw + xg;

            uint16_t c_r = buffer[ir * 2] | (buffer[ir * 2 + 1] << 8);
            uint16_t c_g = buffer[ig * 2] | (buffer[ig * 2 + 1] << 8);
            uint16_t c_b = buffer[i * 2] | (buffer[i * 2 + 1] << 8);

            uint8_t r = get_r(c_r);
            uint8_t g = get_g(c_g);
            uint8_t b = get_b(c_b);
            uint16_t result = make_rgb565(r, g, b);

            framebuffer[i * 2] = result & 0xFF;
            framebuffer[i * 2 + 1] = result >> 8;
        }
    }
    // broken chromatic abberration - looks interesting
    //  for (int y = 0; y < DISPLAY_HEIGHT; y++)
    //  {
    //      uint16_t yr = (y > 0) ? y - 2 : y;
    //      uint16_t yg = (y < DISPLAY_HEIGHT - 1) ? y + 2 : y;
    //      for (int x = 0; x < DISPLAY_WIDTH; x++)
    //      {
    //          uint32_t i = y * DISPLAY_WIDTH + x;

    //         uint16_t xr = (x > 0) ? x - 2 : x;
    //         uint16_t xg = (x < DISPLAY_WIDTH - 1) ? x + 2 : x;

    //         uint16_t ir = yr * DISPLAY_WIDTH + xr;
    //         uint16_t ig = yg * DISPLAY_WIDTH + xg;

    //         uint16_t c_r = buffer[ir * 2] | (buffer[ir * 2 + 1] << 8);
    //         uint16_t c_g = buffer[ig * 2] | (buffer[ig * 2 + 1] << 8);
    //         uint16_t c_b = buffer[i * 2] | (buffer[i * 2 + 1] << 8);

    //         uint8_t r = get_r(c_r);
    //         uint8_t g = get_g(c_g);
    //         uint8_t b = get_b(c_b);
    //         uint16_t result = make_rgb565(r, g, b);

    //         framebuffer[i * 2] = result & 0xFF;
    //         framebuffer[i * 2 + 1] = result >> 8;
    //     }
    // }
    memcpy(buffer, framebuffer, BUFFER_SIZE);
    free(framebuffer);
    // scanline
    for (uint16_t y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (uint16_t x = scanline_offset; x < DISPLAY_WIDTH; x += 2)
        {
            draw_pixel(x, y, 0);
        }
    }
    scanline_offset = !scanline_offset;
}

void apply_post_process_effect(uint8_t effect_index)
{
    switch (effect_index)
    {
    case 0:
        crt_disp_effect();
        break;
    default:
        return;
    }
}

void draw_sprite(uint8_t sprite_index, int16_t pos_x, int16_t pos_y, float angle)
{
    Sprite *sprite = get_sprite(sprite_index);
    int32_t fixed_angle = float_to_fixed(angle);
    int16_t cos = fast_cos(-fixed_angle);
    int16_t sin = fast_sin(-fixed_angle);
    if (fixed_angle != 0)
    {
        int8_t middle = sprite->size >> 1;
        for (uint16_t y = 0; y < sprite->size; y++)
        {
            int16_t new_y = y + pos_y;
            if (new_y >= 0 || new_y < DISPLAY_WIDTH)
            {
                for (uint16_t x = 0; x < sprite->size; x++)
                {
                    int16_t new_x = x + pos_x;
                    int16_t xr = (((x - middle) * fast_cos(-fixed_angle) - (y - middle) * fast_sin(-fixed_angle)) >> SHIFT_FACTOR) + middle;
                    int16_t yr = (((x - middle) * fast_sin(-fixed_angle) + (y - middle) * fast_cos(-fixed_angle)) >> SHIFT_FACTOR) + middle;
                    if ((uint16_t)xr < sprite->size && (uint16_t)yr < sprite->size)
                    {
                        uint16_t pixel = sprite->pixels[yr * sprite->size + xr];
                        if (pixel != 63519)
                        {
                            if (new_x >= 0 || new_x < DISPLAY_HEIGHT)
                                draw_pixel(new_x, new_y, pixel);
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (uint16_t y = 0; y < sprite->size; y++)
        {
            int16_t new_y = y + pos_y;
            if (new_y >= 0 || new_y < DISPLAY_WIDTH)
            {
                uint32_t ydw = y * sprite->size;
                for (uint16_t x = 0; x < sprite->size; x++)
                {
                    uint16_t pixel = sprite->pixels[ydw + x];
                    if (pixel != 63519)
                    {
                        int16_t new_x = x + pos_x;
                        if (new_x >= 0 || new_x < DISPLAY_HEIGHT)
                            draw_pixel(new_x, new_y, pixel);
                    }
                }
            }
        }
    }
}

void draw_bone(Bone *bone, int16_t parentX, int16_t parentY)
{
    int16_t x = bone->x + parentX;
    int16_t y = bone->y + parentY;
    draw_sprite(bone->spriteIndex, x, y, 0.0f);
    if (bone->childBonesNum == 0)
        return;
    for (uint8_t i = 0; i < bone->childBonesNum; i++)
    {
        draw_bone(&bone->childBones[i], x, y);
    }
}

void draw_puppet(Puppet *puppet)
{
    int16_t x = puppet->bones[0].x + puppet->x;
    int16_t y = puppet->bones[0].y + puppet->y;
    // draw_sprite(puppet->bones[0].spriteIndex, x, y, 0.0f);
    for (uint8_t i = 0; i < puppet->bonesNum; i++)
    {
        draw_bone(&puppet->bones[i], x, y);
    }
}

static IPainter painter = {
    .init_painter = init_painter,
    .draw_buffer = draw_buffer,
    .clear_buffer = clear_buffer,
    .draw_pixel = draw_pixel,
    .draw_image = draw_image,
    .apply_post_process_effect = apply_post_process_effect,
    .draw_sprite = draw_sprite,
    .draw_puppet = draw_puppet};

const IPainter *get_painter(void)
{
    return &painter;
}