#include "IFileReader.h"
#include "fileReader.h"
#include "fatfs/ff.h"
#include "stdio.h"
#include "../shared/pins.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "pico/audio.h"
#include "pico/audio_i2s.h"
#include "string.h"

static const IHardware *_hardware = NULL;
static const IPainter *_painter = NULL;
static FRESULT f_res;
static FATFS microSDFatFs;

uint8_t readline(uint8_t *line, uint8_t length, FIL *file)
{
	uint br;
	uint8_t lineBuffer[1];
	f_read(file, lineBuffer, 1, &br);
	if (f_eof(file))
	{
		return 1;
	}
	line[0] = lineBuffer[0];
	uint8_t counter = 1;
	while ((lineBuffer[0] != '\n') && (counter < length - 1))
	{
		f_read(file, lineBuffer, 1, &br);
		line[counter] = lineBuffer[0];
		counter++;
	}
	return 0;
}

void clearline(uint8_t *line, uint8_t length)
{
	for (uint8_t i = 0; i < length; i++)
	{
		line[i] = 0;
	}
}

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

// LoadedObj *load_obj_file(char *file_name)
// {
// 	FIL file;
// 	f_res = f_open(&file, file_name, FA_READ);
// 	if (f_res != FR_OK)
// 	{
// 		printf("Loading file failed :(%d)\r\n", f_res);
// 		return 0;
// 	}
// 	uint8_t linelength = 32;
// 	uint8_t line[linelength];
// 	// uint8_t *line = (uint8_t *)malloc(sizeof(uint8_t) * linelength);

// 	LoadedObj *obj = (LoadedObj *)malloc(sizeof(LoadedObj));
// 	obj->faces = (uint16_t *)malloc(sizeof(uint16_t) * 0);
// 	obj->facesCounter = 0;
// 	obj->vertices = (uint32_t *)malloc(sizeof(uint32_t) * 0);
// 	obj->verticesCounter = 0;
// 	obj->textureCoords = (uint32_t *)malloc(sizeof(uint32_t) * 0);
// 	obj->uv = (uint16_t *)malloc(sizeof(uint16_t) * 0);
// 	obj->textureCoordsCounter = 0;
// 	clearline(line, linelength);

// 	while (!readline(line, linelength, &file))
// 	{
// 		if (line[0] == 'v' && line[1] == ' ')
// 		{
// 			float x, y, z;
// 			int result = sscanf(line + 2, "%f %f %f", &x, &y, &z);
// 			obj->verticesCounter++;
// 			// uint32_t *temp = realloc(obj->vertices, obj->verticesCounter * 3 * sizeof(uint32_t));
// 			// obj->vertices = temp;
// 			obj->vertices = realloc(obj->vertices, obj->verticesCounter * 3 * sizeof(uint32_t));
// 			obj->vertices[(obj->verticesCounter - 1) * 3] = float_to_fixed(x);
// 			obj->vertices[(obj->verticesCounter - 1) * 3 + 1] = float_to_fixed(y);
// 			obj->vertices[(obj->verticesCounter - 1) * 3 + 2] = float_to_fixed(z);
// 		}
// 		else if (line[0] == 'v' && line[1] == 't')
// 		{
// 			float x, y;
// 			int result = sscanf(line + 3, "%f %f", &x, &y);
// 			obj->textureCoordsCounter++;
// 			obj->textureCoords = realloc(obj->textureCoords, obj->textureCoordsCounter * 2 * sizeof(uint32_t));
// 			// uint32_t *temp = realloc(obj->textureCoords, obj->textureCoordsCounter * 2 * sizeof(uint32_t));
// 			// obj->textureCoords = temp;
// 			obj->textureCoords[(obj->textureCoordsCounter - 1) * 2] = float_to_fixed(x);
// 			obj->textureCoords[(obj->textureCoordsCounter - 1) * 2 + 1] = float_to_fixed(y);
// 		}
// 		else if (line[0] == 'f' && line[1] == ' ')
// 		{
// 			uint16_t f[3], vt[3], vn[3];
// 			int result = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &vt[0], &vn[0], &f[1], &vt[1], &vn[1], &f[2], &vt[2], &vn[2]);
// 			obj->facesCounter++;
// 			obj->faces = realloc(obj->faces, obj->facesCounter * 3 * sizeof(uint16_t));
// 			// uint16_t *temp1 = realloc(obj->faces, obj->facesCounter * 3 * sizeof(uint16_t));
// 			// obj->faces = temp1;
// 			obj->faces[(obj->facesCounter - 1) * 3] = (f[0] - 1);
// 			obj->faces[(obj->facesCounter - 1) * 3 + 1] = (f[1] - 1);
// 			obj->faces[(obj->facesCounter - 1) * 3 + 2] = (f[2] - 1);
// 			obj->uv = realloc(obj->uv, obj->facesCounter * 3 * sizeof(uint16_t));
// 			// uint16_t *temp2 = realloc(obj->uv, obj->facesCounter * 3 * sizeof(uint16_t));
// 			// obj->uv = temp2;
// 			obj->uv[(obj->facesCounter - 1) * 3] = (vt[0] - 1);
// 			obj->uv[(obj->facesCounter - 1) * 3 + 1] = (vt[1] - 1);
// 			obj->uv[(obj->facesCounter - 1) * 3 + 2] = (vt[2] - 1);
// 		}
// 		clearline(line, linelength);
// 	}
// 	f_close(&file);
// 	// free(line);
// 	return obj;
// }

// LoadedBmp *load_bmp_file(char *file_name)
// {
// 	FIL file;
// 	f_res = f_open(&file, file_name, FA_READ);
// 	if (f_res != FR_OK)
// 	{
// 		printf("Loading file failed :(%d)\r\n", f_res);
// 		return 0;
// 	}
// 	LoadedBmp *bmp = (LoadedBmp *)malloc(sizeof(LoadedBmp));
// 	int br;
// 	uint32_t pixelArrayOffset[1];
// 	f_lseek(&file, 10);
// 	f_read(&file, pixelArrayOffset, 4, &br);
// 	uint32_t buffer[1];
// 	// f_read(&file, buffer, 4, &br);
// 	f_lseek(&file, 18);
// 	f_read(&file, buffer, 4, &br);
// 	bmp->sizeY = buffer[0];
// 	f_read(&file, buffer, 4, &br);
// 	bmp->sizeX = buffer[0];
// 	f_lseek(&file, pixelArrayOffset[0]);
// 	uint32_t pixelsNum = bmp->sizeX * bmp->sizeY;
// 	bmp->pixels = (uint16_t *)malloc(sizeof(uint16_t) * pixelsNum);
// 	for (uint16_t i = 0; i < pixelsNum; i++)
// 	{
// 		uint16_t buf[1];
// 		f_read(&file, buf, 2, &br);
// 		uint16_t color = buf[0];
// 		uint8_t r = (color >> 11) & 0x1f;
// 		uint8_t g = (color >> 5) & 0x3f;
// 		uint8_t b = color & 0x1f;
// 		color = (r << 12) | (g << 6) | b;
// 		bmp->pixels[i] = color;
// 	}
// 	f_close(&file);
// 	return bmp;
// }

void draw_bmp_file(char *file_name)
{
	FIL file;
	f_res = f_open(&file, file_name, FA_READ);
	int br;
	uint32_t pixelArrayOffset[1];
	f_lseek(&file, 10);
	f_read(&file, pixelArrayOffset, 4, &br);
	uint32_t buffer[1];
	// f_read(&file, buffer, 4, &br);
	f_lseek(&file, 18);
	f_read(&file, buffer, 4, &br);
	uint16_t width = buffer[0];
	f_read(&file, buffer, 4, &br);
	uint16_t height = buffer[0];
	f_lseek(&file, pixelArrayOffset[0]);
	uint16_t buf[1];
	for (uint16_t y = 0; y < height; y++)
	{
		for (uint16_t x = 0; x < width; x++)
		{
			f_read(&file, buf, 2, &br);
			uint16_t color = buf[0];
			uint8_t r = (color >> 11) & 0x1f;
			uint8_t g = (color >> 5) & 0x3f;
			uint8_t b = color & 0x1f;
			color = (r << 12) | (g << 6) | b;
			_painter->draw_pixel(x, y, color);
		}
	}
	f_close(&file);
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
	while (1)
	{
		f_read(&file, buffer, sizeof(buffer), &br);
		if (br == 0)
			break;
		struct audio_buffer_pool *audio_buffer_pool = _hardware->get_audio_buffer_pool();
		struct audio_buffer *audio_buf = take_audio_buffer(audio_buffer_pool, true);
		memcpy(audio_buf->buffer->bytes, buffer, br);
		audio_buf->sample_count = br / 2;
		give_audio_buffer(audio_buffer_pool, audio_buf);
	}

	f_close(&file);
}

void init_fileReader(const IHardware *hardware, const IPainter *painter)
{
	_hardware = hardware;
	_painter = painter;
	sd_init();
}

static IFileReader fileReader = {
	.init_fileReader = init_fileReader,
	.draw_bmp_file = draw_bmp_file,
	.play_wave_file = play_wave_file};

const IFileReader *get_fileReader(void)
{
	return &fileReader;
}