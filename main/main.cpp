#include "gpio.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern "C" void app_main() {
    console::drivers::Pin led(2);
    led.setMode(console::drivers::PinMode::Output);

    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}