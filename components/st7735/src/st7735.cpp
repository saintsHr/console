#include "st7735.hpp"
#include "freertos/idf_additions.h"
#include "gpio.hpp"
#include "portmacro.h"
#include "spi.hpp"
#include <cstdint>

namespace console::drivers {

St7735::St7735(
	Spi& spi
) : spi_(spi), id_(SPI_INVALID_DEVICE), dc_(ST7735_PIN_DC), rst_(ST7735_PIN_RST) {
	
}

bool St7735::write_command_(uint8_t cmd) {
	dc_.write(PinState::Low);
	return spi_.write(&cmd, 1, id_);
}

bool St7735::write_data_(const uint8_t *dat, size_t length) {
	dc_.write(PinState::High);
	return spi_.write(dat, length, id_);
}

void St7735::reset_hard_() {
	rst_.write(PinState::High);
	vTaskDelay(20 / portTICK_PERIOD_MS);
	rst_.write(PinState::Low);
	vTaskDelay(20 / portTICK_PERIOD_MS);
	rst_.write(PinState::High);
	vTaskDelay(200 / portTICK_PERIOD_MS);
}

void St7735::reset_soft_() {
	write_command_(ST7735_CMD_RESET_SOFT);
	vTaskDelay(200 / portTICK_PERIOD_MS);
}

void St7735::sleep_out_() {
	write_command_(ST7735_CMD_SLEEP_OUT);
	vTaskDelay(200 / portTICK_PERIOD_MS);
}

void St7735::color_mode_() {
	write_command_(ST7735_CMD_COLOR_MODE);

	uint8_t param = ST7735_COLOR_MODE;
	write_data_(&param, 1);

	vTaskDelay(10 / portTICK_PERIOD_MS);
}

void St7735::orientation_() {
	write_command_(ST7735_CMD_ORIENTATION);

	uint8_t param = ST7735_ORIENTATION;
	write_data_(&param, 1);

	vTaskDelay(10 / portTICK_PERIOD_MS);
}

void St7735::turn_on_() {
	write_command_(ST7735_CMD_TURN_ON);
	vTaskDelay(200 / portTICK_PERIOD_MS);
}

void St7735::set_window_(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	uint8_t data_x[4] = {
		0, static_cast<uint8_t>(x0 + ST7735_OFFSET_X),
		0, static_cast<uint8_t>(x1 + ST7735_OFFSET_X)
	};

	uint8_t data_y[4] = {
		0, static_cast<uint8_t>(y0 + ST7735_OFFSET_Y),
		0, static_cast<uint8_t>(y1 + ST7735_OFFSET_Y)
	};

	write_command_(ST7735_CMD_CASET);
	write_data_(data_x, 4);

	write_command_(ST7735_CMD_RASET);
	write_data_(data_y, 4);

	write_command_(ST7735_CMD_RAMWR);
}

bool St7735::init() {
	dc_.setMode(PinMode::Output);
	rst_.setMode(PinMode::Output);

	if (
		!spi_.addDevice(
			ST7735_PIN_CS, ST7735_SPI_HZ,
			SpiMode::Mode0, id_
		)
	) return false;

	reset_hard_();
	reset_soft_();
	sleep_out_();
	color_mode_();
	orientation_();
	turn_on_();

	return true;
}

void St7735::draw(const uint16_t* framebuffer) {
	set_window_(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1);
	
	write_data_(
		reinterpret_cast<const uint8_t*>(framebuffer),
		ST7735_WIDTH * ST7735_HEIGHT * sizeof(uint16_t)
	);
}

}