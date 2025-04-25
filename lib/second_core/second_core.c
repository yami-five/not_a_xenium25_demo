#include "fileReader.h"
#include "fatfs/ff.h"
#include "stdio.h"
#include "../shared/pins.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "pico/audio.h"
#include "pico/audio_i2s.h"
#include "string.h"
#include "second_core.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "IHardware.h"
#include "hardware.h"

static const IHardware *_hardware = NULL;
static FRESULT f_res;
static FATFS microSDFatFs;

void sd_init()
{
	_hardware->write(SD_CS_PIN, 1);
	_hardware->write(LCD_CS_PIN, 1);
	_hardware->write(TP_CS_PIN, 1);

	// Check the mounted device
	f_res = f_mount(&microSDFatFs, (TCHAR const *)"/", 1);
	if (f_res != FR_OK)
	{
		printf("SD card mount file system failed ,error code :(%d)\r\n", f_res);
	}
	else
	{
		printf("SD card mount file system success!! \r\n");
	}
}

void sd_close()
{
	f_unmount((TCHAR const *)"/");
}

void play_wave_file(char *file_name)
{
	FIL file;
	uint8_t header[44];
	f_res = f_open(&file, file_name, FA_READ);
	uint br;
	f_lseek(&file, 0); 
    f_read(&file, header, 44, &br);
	if (f_res != FR_OK)
	{
		printf("Loading file failed :(%d)\r\n", f_res);
		return;
	}
	const int buffer_size = 16;
    int16_t buffer[buffer_size];
	while(1)
	{
        f_read(&file, buffer, sizeof(buffer), &br);
        if (br == 0) break;
		struct audio_buffer_pool *audio_buffer_pool = _hardware->get_audio_buffer_pool();
        struct audio_buffer *audio_buf = take_audio_buffer(audio_buffer_pool, true);
        memcpy(audio_buf->buffer->bytes, buffer, br);
        audio_buf->sample_count = br / 2;
        give_audio_buffer(audio_buffer_pool, audio_buf);
    }

    f_close(&file);
}

void play_music_on_second_core()
{
    _hardware = get_hardware();
	// multicore_fifo_push_blocking(2137);
	sd_init();
	play_wave_file("test.wav");
}