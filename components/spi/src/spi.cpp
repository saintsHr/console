#include "spi.hpp"
#include <driver/spi_master.h>
#include <esp_err.h>

namespace console::drivers {

Spi::Spi(
    SpiHost host,
    PinNumber sclk,
    PinNumber mosi,
    PinNumber miso
) {
    switch(host) {
        case SpiHost::SPI2:
            host_ = SPI2_HOST;
            break;

        case SpiHost::SPI3:
            host_ = SPI3_HOST;
            break;
    }

    sclk_ = sclk;
    mosi_ = mosi;
    miso_ = miso;
}


bool Spi::begin() {
    spi_bus_config_t bus = {};

    bus.sclk_io_num = sclk_;
    bus.mosi_io_num = mosi_;
    bus.miso_io_num = miso_;
    bus.max_transfer_sz = 4096;

    return (spi_bus_initialize(host_, &bus, SPI_DMA_CH_AUTO) == ESP_OK);
}

bool Spi::addDevice(PinNumber cs, uint32_t hz, SpiMode mode, DeviceID &outId) {
    if (device_count_ >= SPI_MAX_DEVICES) return false;

    spi_device_interface_config_t device = {};
    DeviceID id = device_count_;

    device.clock_speed_hz = hz;
    device.mode = static_cast<uint8_t>(mode);
    device.spics_io_num = cs;
    device.queue_size = 7;

    if (spi_bus_add_device(
        host_,
        &device,
        &devices_[id]
    ) != ESP_OK) return false;

    outId = id;
    device_count_++;

    return true;
}


bool Spi::write(
    const uint8_t *data,
    size_t length,
    DeviceID device
) {
    if (device >= SPI_MAX_DEVICES) return false;
    if (device >= device_count_) return false;
    if (device == SPI_INVALID_DEVICE) return false;

    spi_transaction_t transaction = {};

    transaction.length = length * 8;
    transaction.tx_buffer = data;

    return spi_device_transmit(
        devices_[device],
        &transaction
    ) == ESP_OK;
}


bool Spi::transfer(
    const uint8_t *tx,
    uint8_t *rx,
    size_t length,
    DeviceID device
) {
    if (device >= SPI_MAX_DEVICES) return false;
    if (device >= device_count_) return false;
    if (device == SPI_INVALID_DEVICE) return false;

    spi_transaction_t transaction = {};

    transaction.length = length * 8;
    transaction.tx_buffer = tx;
    transaction.rx_buffer = rx;

    return spi_device_transmit(
        devices_[device],
        &transaction
    ) == ESP_OK;
}

}