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
#include "hardware/sync/spin_lock.h"
#include "../shared/gfx.h"
#include "fpa.h"

static const IHardware *_hardware = NULL;
static FRESULT f_res;
static FATFS microSDFatFs;
static spin_lock_t *sd_spinlock;

LoadedObj *load_obj_file(uint16_t model_index)
{
	uint16_t offset=0;
	Model *model=get_model(model_index);
	LoadedObj *obj = (LoadedObj *)malloc(sizeof(LoadedObj));
	obj->faces = (uint16_t *)malloc(sizeof(uint16_t) * 0);
	obj->facesCounter = 0;
	obj->vertices = (uint32_t *)malloc(sizeof(uint32_t) * 0);
	obj->verticesCounter = 0;
	obj->textureCoords = (uint32_t *)malloc(sizeof(uint32_t) * 0);
	obj->uv = (uint16_t *)malloc(sizeof(uint16_t) * 0);
	obj->textureCoordsCounter = 0;
	obj->normalsCoords = (int32_t *)malloc(sizeof(int32_t)*0);
	obj->normals = (uint16_t *)malloc(sizeof(uint16_t)*0);
	obj->normalsCoordsCounter = 0;
	while(offset<model->len)
	{
		unsigned char line[32];
		uint8_t i=0;
		while(model->obj[offset]!="\n")
		{
			line[i]=model->obj[offset];
			offset++;
			i++;
		}
		if (line[0] == 'v' && line[1] == ' ')
		{
			float x, y, z;
			int result = sscanf(line + 2, "%f %f %f", &x, &y, &z);
			obj->verticesCounter++;
			obj->vertices = realloc(obj->vertices, obj->verticesCounter * 3 * sizeof(uint32_t));
			obj->vertices[(obj->verticesCounter - 1) * 3] = float_to_fixed(x);
			obj->vertices[(obj->verticesCounter - 1) * 3 + 1] = float_to_fixed(y);
			obj->vertices[(obj->verticesCounter - 1) * 3 + 2] = float_to_fixed(z);
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			float x, y;
			int result = sscanf(line + 3, "%f %f", &x, &y);
			obj->textureCoordsCounter++;
			obj->textureCoords = realloc(obj->textureCoords, obj->textureCoordsCounter * 2 * sizeof(uint32_t));
			obj->textureCoords[(obj->textureCoordsCounter - 1) * 2] = float_to_fixed(x);
			obj->textureCoords[(obj->textureCoordsCounter - 1) * 2 + 1] = float_to_fixed(y);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			float x, y, z;
			int result = sscanf(line + 2, "%f %f %f", &x, &y, &z);
			obj->normalsCoordsCounter++;
			obj->normalsCoords = realloc(obj->textureCoords, obj->textureCoordsCounter * 3 * sizeof(uint32_t));
			obj->normalsCoords[(obj->verticesCounter - 1) * 3] = float_to_fixed(x);
			obj->normalsCoords[(obj->verticesCounter - 1) * 3 + 1] = float_to_fixed(y);
			obj->normalsCoords[(obj->verticesCounter - 1) * 3 + 2] = float_to_fixed(z);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			uint16_t f[3], vt[3], vn[3];
			int result = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &vt[0], &vn[0], &f[1], &vt[1], &vn[1], &f[2], &vt[2], &vn[2]);
			obj->facesCounter++;
			obj->faces = realloc(obj->faces, obj->facesCounter * 3 * sizeof(uint16_t));
			obj->faces[(obj->facesCounter - 1) * 3] = (f[0] - 1);
			obj->faces[(obj->facesCounter - 1) * 3 + 1] = (f[1] - 1);
			obj->faces[(obj->facesCounter - 1) * 3 + 2] = (f[2] - 1);
			obj->uv = realloc(obj->uv, obj->facesCounter * 3 * sizeof(uint16_t));
			obj->uv[(obj->facesCounter - 1) * 3] = (vt[0] - 1);
			obj->uv[(obj->facesCounter - 1) * 3 + 1] = (vt[1] - 1);
			obj->uv[(obj->facesCounter - 1) * 3 + 2] = (vt[2] - 1);
			obj->uv = realloc(obj->normals, obj->facesCounter * 3 * sizeof(uint16_t));
			obj->uv[(obj->facesCounter - 1) * 3] = (vn[0] - 1);
			obj->uv[(obj->facesCounter - 1) * 3 + 1] = (vn[1] - 1);
			obj->uv[(obj->facesCounter - 1) * 3 + 2] = (vn[2] - 1);
		}
		offset++;
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
	int16_t buffer_audio[buffer_size];
	spin_lock_t *spi_spinlock = _hardware->get_spinlock();
	while (1)
	{
		uint32_t flags = spin_lock_blocking(spi_spinlock);
		f_read(&file, buffer_audio, sizeof(buffer_audio), &br);
		spin_unlock(spi_spinlock, flags);
		if (br == 0)
			break;
		struct audio_buffer_pool *audio_buffer_pool = _hardware->get_audio_buffer_pool();
		struct audio_buffer *audio_buf = take_audio_buffer(audio_buffer_pool, true);
		memcpy(audio_buf->buffer->bytes, buffer_audio, br);
		audio_buf->sample_count = br / 2;
		give_audio_buffer(audio_buffer_pool, audio_buf);
	}

	f_close(&file);
}

void init_fileReader(const IHardware *hardware)
{
	_hardware = hardware;
	sd_init();
}

static IFileReader fileReader = {
	.init_fileReader = init_fileReader,
	.play_wave_file = play_wave_file,
	.load_obj_file = load_obj_file};

const IFileReader *get_fileReader(void)
{
	return &fileReader;
}