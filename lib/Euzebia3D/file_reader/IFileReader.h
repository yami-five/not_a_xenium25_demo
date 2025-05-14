#ifndef IFILEREADER_h
#define IFILEREADER_h

#include <stdint.h>
#include "IPainter.h"
#include "IHardware.h"

typedef struct
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    uint16_t textureCoordsCounter;
    uint16_t normalsCoordsCounter;
    int32_t *vertices;
    uint16_t *faces;
    int32_t *textureCoords;
    uint16_t *uv;
    int32_t *normalsCoords;
    uint16_t *normals;    
} LoadedObj;

typedef struct
{
    void (*init_fileReader)(const IHardware *hardware);
    void (*play_wave_file)(char *file_name);
    LoadedObj* (*load_obj_file)(uint16_t model_index);
} IFileReader;

#endif