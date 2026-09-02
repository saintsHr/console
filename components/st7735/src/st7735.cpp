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

}