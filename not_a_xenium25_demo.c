#include <stdio.h>
#include "pico/multicore.h"

#include "IDisplay.h"
#include "IFileReader.h"
#include "IHardware.h"
#include "IPainter.h"

#include "display.h"
#include "fileReader.h"
#include "hardware.h"
#include "painter.h"

#include "second_core.h"

int main()
{
    const IHardware* hardware = get_hardware();
    hardware->init_hardware();

    const IDisplay* display = get_display();
    display->init_display(hardware);

    const IPainter* painter = get_painter();
    painter->init_painter(display, hardware);

    // const IFileReader* fileReader = get_fileReader();
    // fileReader->init_fileReader(hardware, painter);

    // fileReader->play_wave_file("test.wav");
    multicore_launch_core1(play_music_on_second_core);
    uint16_t is_core1_running = multicore_fifo_pop_blocking();
    // play_music_on_second_core();
    painter->clear_buffer();
    painter->draw_buffer();
    while (1) {
        // painter->clear_buffer();
        // fileReader->draw_bmp_file("sl_w_p.bmp");
        painter->draw_image(0);
        painter->draw_buffer();
    }
}
