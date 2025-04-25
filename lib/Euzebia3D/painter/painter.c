#include "IPainter.h"
#include "painter.h"
#include "../shared/gfx.h"

static const IHardware *_hardware = NULL;
static const IDisplay *_display = NULL;
static uint8_t buffer[BUFFER_SIZE];

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
        buffer,
        BUFFER_SIZE,
        false
    );
}

void init_painter(const IDisplay *display, const IHardware *hardware)
{
    _hardware = hardware;
    _display = display;
    init_dma();
}

void display_refresh()
{
    _hardware->write(LCD_CS_PIN, 0);
    _hardware->write(LCD_DC_PIN, 0);
    _hardware->spi_write_byte(0x2C);
    _hardware->write(LCD_DC_PIN, 1);
    _hardware->write(LCD_CS_PIN, 1);
}

void draw_buffer()
{
    _hardware->write(LCD_DC_PIN, 1);
    _hardware->write(LCD_CS_PIN, 0);
    dma_channel_transfer_from_buffer_now(dma_channel, buffer, BUFFER_SIZE);
    dma_channel_wait_for_finish_blocking(dma_channel);
    display_refresh();
    _hardware->write(LCD_CS_PIN, 1);
}

void clear_buffer()
{
    memset(buffer, 0, sizeof(buffer));
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint32_t line_adr = x* HEIGHT_DOUBLED;
    uint16_t y2=y*2;
    buffer[line_adr + y2]=(color >> 8) & 0xff;
    buffer[line_adr + y2 +1]=color & 0xff;
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
        images[image_index].image,
        BUFFER_SIZE,
        false
    );
    dma_channel_start(dma_channel_flash);
    dma_channel_wait_for_finish_blocking(dma_channel_flash);
    dma_channel_unclaim(dma_channel_flash);
}

static IPainter painter = {
    .init_painter = init_painter,
    .draw_buffer = draw_buffer,
    .clear_buffer = clear_buffer,
    .draw_pixel = draw_pixel,
    .draw_image = draw_image
};

const IPainter *get_painter(void)
{
    return &painter;
}