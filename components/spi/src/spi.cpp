#include "spi.hpp"

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

    return spi_bus_initialize(
        host_,
        &bus,
        SPI_DMA_CH_AUTO
    ) == ESP_OK;
}


bool Spi::write(
    const uint8_t* data,
    size_t length
) {
    if (!device_) return false;

    spi_transaction_t transaction = {};

    transaction.length = length * 8;
    transaction.tx_buffer = data;

    return spi_device_transmit(
        device_,
        &transaction
    ) == ESP_OK;
}


bool Spi::transfer(
    const uint8_t* tx,
    uint8_t* rx,
    size_t length
) {
    if (!device_) return false;

    spi_transaction_t transaction = {};

    transaction.length = length * 8;
    transaction.tx_buffer = tx;
    transaction.rx_buffer = rx;

    return spi_device_transmit(
        device_,
        &transaction
    ) == ESP_OK;
}

}