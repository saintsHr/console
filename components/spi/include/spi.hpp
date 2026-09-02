#pragma once

#include <cstdint>
#include "driver/spi_master.h"

namespace console::drivers {

using PinNumber = uint8_t;
using DeviceID = uint8_t;
using DeviceCount = uint8_t;

constexpr uint8_t SPI_MAX_DEVICES = 8;

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
    
    bool addDevice(PinNumber cs, uint32_t hz, SpiMode mode, DeviceID &outId);

    bool write(
        const uint8_t *data,
        size_t length,
        DeviceID device
    );

    bool transfer(
        const uint8_t *tx,
        uint8_t *rx,
        size_t length,
        DeviceID device
    );

private:
    spi_host_device_t host_;

    PinNumber sclk_;
    PinNumber mosi_;
    PinNumber miso_;

    spi_device_handle_t devices_[SPI_MAX_DEVICES];
    DeviceCount device_count_ = 0;
};

}