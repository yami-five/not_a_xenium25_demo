#ifndef HARDWARE_h
#define HARDWARE_h

#include "IHardware.h"
#include "audio_pio.pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "../shared/pins.h"

static spi_inst_t *SPI_PORT = spi1;
static spi_inst_t *I2C_PORT = spi1;
static const uint16_t PICO_AUDIO_FREQ=44100;
static const uint8_t PICO_AUDIO_COUNT=2;
static const uint8_t PICO_AUDIO_SM=0;
#define PICO_AUDIO_PIO 0
#define AUDIO_PIO __CONCAT(pio, PICO_AUDIO_PIO)
#define GPIO_FUNC_PIOx __CONCAT(GPIO_FUNC_PIO, PICO_AUDIO_PIO)

static audio_format_t audio_format =
{
    .sample_freq = PICO_AUDIO_FREQ,
    .channel_count = PICO_AUDIO_COUNT,
    .audio_data = PICO_AUDIO_DATA_PIN,
    .audio_clock = PICO_AUDIO_CLOCK_PIN,
    .pio = AUDIO_PIO,
    .sm = PICO_AUDIO_SM
};

const IHardware *get_hardware(void);

#endif