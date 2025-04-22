#include "IHardware.h"
#include "hardware.h"
#include "../shared/pins.h"

volatile bool te_signal_detected = false;
uint32_t slice_num;

bool get_te_signal_detected()
{
    return te_signal_detected;
}
void set_te_signal_detected(bool value)
{
    te_signal_detected = value;
}

static void write(uint32_t pin, uint8_t value)
{
    gpio_put(pin, value);
}

static uint8_t read(uint32_t pin)
{
    gpio_get(pin);
}

static void spi_write_byte(uint8_t value)
{
    spi_write_blocking(SPI_PORT, &value, 1);
}

static uint8_t spi_write_read_byte(uint8_t value)
{
    uint8_t rxDat;
    spi_write_read_blocking(spi1, &value, &rxDat, 1);
    return rxDat;
}

void gpio_mode(uint16_t pin, uint16_t mode)
{
    gpio_init(pin);
    if (mode == 0 || mode == GPIO_IN)
    {
        gpio_set_dir(pin, GPIO_IN);
    }
    else
    {
        gpio_set_dir(pin, GPIO_OUT);
    }
}

static void delay_ms(uint32_t ms)
{
    sleep_ms(ms);
}

static void set_pwm(uint8_t value)
{
    if (value < 0 || value > 100)
    {
        printf("DEV_SET_PWM Error \r\n");
    }
    else
    {
        pwm_set_chan_level(slice_num, PWM_CHAN_B, value);
    }
}

static void init_hardware(void)
{
    stdio_init_all();

    // SPI Config
    spi_init(SPI_PORT, 62500 * 1000);
    gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MISO_PIN, GPIO_FUNC_SPI);

    // PWM Config
    gpio_set_function(LCD_BL_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LCD_BL_PIN);
    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 1);
    pwm_set_clkdiv(slice_num, 50);
    pwm_set_enabled(slice_num, true);

    // I2C Config
    i2c_init(i2c1, 300 * 1000);
    gpio_set_function(LCD_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(LCD_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(LCD_SDA_PIN);
    gpio_pull_up(LCD_SCL_PIN);

    gpio_set_function(audio_format.audio_data, GPIO_FUNC_PIOx);
    gpio_set_function(audio_format.audio_clock, GPIO_FUNC_PIOx);
    gpio_set_function(audio_format.audio_clock + 1, GPIO_FUNC_PIOx);

    pio_sm_claim(audio_format.pio, audio_format.sm);

    uint offset = pio_add_program(audio_format.pio, &audio_pio_program);

    audio_pio_program_init(audio_format.pio, audio_format.sm, offset, audio_format.audio_data, audio_format.audio_clock);

    uint32_t system_clock_frequency = clock_get_hz(clk_sys);
    uint32_t divider = system_clock_frequency * 4 / audio_format.sample_freq; // avoid arithmetic overflow
    pio_sm_set_clkdiv_int_frac(audio_format.pio, audio_format.sm, divider >> 8u, divider & 0xffu);

    pio_sm_set_enabled(audio_format.pio, audio_format.sm, true);

    // GPIO Config
    gpio_mode(LED_PIN, GPIO_OUT);

    gpio_mode(LCD_RST_PIN, GPIO_OUT);
    gpio_mode(LCD_DC_PIN, GPIO_OUT);
    gpio_mode(LCD_BL_PIN, GPIO_OUT);
    gpio_mode(LCD_CS_PIN, GPIO_OUT);
    gpio_mode(TP_CS_PIN, GPIO_OUT);
    gpio_mode(TP_IRQ_PIN, GPIO_IN);
    gpio_mode(SD_CS_PIN, GPIO_OUT);
    gpio_set_pulls(TP_IRQ_PIN, true, false);

    write(TP_CS_PIN, GPIO_OUT);
    write(LCD_CS_PIN, GPIO_OUT);
    write(LCD_BL_PIN, GPIO_OUT);
    write(SD_CS_PIN, GPIO_OUT);
}

spi_inst_t *get_spi_port()
{
    return SPI_PORT;
}

audio_format_t *get_audio_format()
{
    return &audio_format;
}

static IHardware hardware = {
    .init_hardware = init_hardware,
    .write = write,
    .spi_write_byte = spi_write_byte,
    .spi_write_read_byte = spi_write_read_byte,
    .delay_ms = delay_ms,
    .set_pwm = set_pwm,
    .get_spi_port = get_spi_port,
    .get_audio_format = get_audio_format
};

const IHardware *get_hardware(void)
{
    return &hardware;
}
