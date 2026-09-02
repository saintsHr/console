#pragma once

#include "spi.hpp"
#include "gpio.hpp"
#include <cstdint>

constexpr uint8_t ST7735_PIN_DC = 2; // default: 2
constexpr uint8_t ST7735_PIN_RST = 4; // default: 4
constexpr uint8_t ST7735_PIN_CS = 5; // default: 5

constexpr uint8_t ST7735_WIDTH = 128; // default: 128
constexpr uint8_t ST7735_HEIGHT = 128; // default: 128

constexpr uint8_t ST7735_OFFSET_X = 2; // default: 2
constexpr uint8_t ST7735_OFFSET_Y = 1; // default: 1

constexpr uint32_t ST7735_SPI_HZ = 26000000; // default: 26000000

constexpr uint8_t ST7735_ORIENTATION = 0x00; // default: 0x00
constexpr uint8_t ST7735_COLOR_MODE = 0x05; // default: 0x05

constexpr uint8_t ST7735_CMD_RESET_SOFT = 0x01; // default: 0x01
constexpr uint8_t ST7735_CMD_SLEEP_OUT = 0x11; // default: 0x11
constexpr uint8_t ST7735_CMD_COLOR_MODE = 0x3A; // default: 0x3A
constexpr uint8_t ST7735_CMD_ORIENTATION = 0x36; // default: 0x36
constexpr uint8_t ST7735_CMD_TURN_ON = 0x29; // default: 0x29
constexpr uint8_t ST7735_CMD_CASET = 0x2A; // default: 0x2A
constexpr uint8_t ST7735_CMD_RASET = 0x2B; // default: 0x2B
constexpr uint8_t ST7735_CMD_RAMWR = 0x2C; // default: 0x2C

namespace console::drivers {

class St7735 {
public:
	St7735(
		Spi &spi
	);

	bool init();
	void draw(const uint16_t* framebuffer);

private:
	console::drivers::Spi &spi_;
	DeviceID id_;

	Pin dc_;
	Pin rst_;

	bool write_command_(uint8_t cmd);
	bool write_data_(const uint8_t *dat, size_t length);

	void reset_hard_();
	void reset_soft_();

	void sleep_out_();
	void color_mode_();
	void orientation_();
	void turn_on_();

	void set_window_(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
};

}