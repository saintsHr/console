#pragma once

#include <cstdint>
#include "driver/spi_master.h"

namespace console::drivers {

using PinNumber = uint8_t;

enum class SpiHost {
    SPI2,
    SPI3
};

enum class SpiMode {
    Mode0 = 0,
    Mode1 = 1,
    Mode2 = 2,
    Mode3 = 3
};

class Spi {
public:
    Spi(
        SpiHost host,
        PinNumber sclk,
        PinNumber mosi,
        PinNumber miso
    );

    bool begin();

    bool write(
        const uint8_t* data,
        size_t length
    );

    bool transfer(
        const uint8_t* tx,
        uint8_t* rx,
        size_t length
    );

private:
    spi_host_device_t host_;

    PinNumber sclk_;
    PinNumber mosi_;
    PinNumber miso_;

    spi_device_handle_t device_ = nullptr;
};

}