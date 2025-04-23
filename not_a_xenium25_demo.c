#include <stdio.h>

#include "IDisplay.h"
#include "IFileReader.h"
#include "IHardware.h"
#include "IPainter.h"

#include "display.h"
#include "fileReader.h"
#include "hardware.h"
#include "painter.h"

int main()
{
    const IHardware* hardware = get_hardware();
    hardware->init_hardware();

    const IDisplay* display = get_display();
    display->init_display(hardware);

    const IPainter* painter = get_painter();
    painter->init_painter(display, hardware);

    const IFileReader* fileReader = get_fileReader();
    fileReader->init_fileReader(hardware, painter);

    painter->clear_buffer();
    painter->draw_buffer();

    fileReader->play_wave_file("test.wav");

    while (1) {
        // painter->clear_buffer();
        fileReader->draw_bmp_file("sl_w_p.bmp");
        painter->draw_buffer();
    }
}
