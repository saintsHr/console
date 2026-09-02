#include "st7735.hpp"
#include <freertos/FreeRTOS.h>

extern "C" void app_main() {
    console::drivers::Spi spi(
        console::drivers::SpiHost::SPI2,
        18, 23, console::drivers::GPIO_NOT_CONNECTED
    );
    if (!spi.begin()) return;

    console::drivers::St7735 display(spi);
    if (!display.init()) return;

    while (true) {
        vTaskDelay(33 / portTICK_PERIOD_MS);
    }
}