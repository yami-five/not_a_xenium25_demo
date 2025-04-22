#ifndef IFILEREADER_h
#define IFILEREADER_h

#include <stdint.h>
#include "IPainter.h"
#include "IHardware.h"

typedef struct
{
    void (*init_fileReader)(const IHardware *hardware, const IPainter *painter);
    // void (*load_obj_file)(char *file_name);
    // void (*load_bmp_file)(char *file_name);
    void (*draw_bmp_file)(char *file_name);
    void (*play_wave_file)(char *file_name);
} IFileReader;

#endif