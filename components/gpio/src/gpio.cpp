#include "gpio.hpp"
#include "soc/gpio_num.h"

#include <cstdint>
#include <driver/gpio.h>

namespace console::drivers {

Pin::Pin(PinNumber pin) {
	pin_ = pin;
}

void Pin::write(PinState state) {
    if (mode_ != PinMode::Output) return;

	gpio_set_level(
		static_cast<gpio_num_t>(pin_),
		static_cast<uint32_t>(state)
	);

    state_ = state;
}

PinState Pin::read() const {
	return gpio_get_level(
        static_cast<gpio_num_t>(pin_)
    )
        ? PinState::High
        : PinState::Low;
}

void Pin::setMode(PinMode mode) {
	mode_ = mode;
    gpio_config_t config = {};
    config.pin_bit_mask = (1ULL << pin_);

    switch (mode) {
        case PinMode::Input:
            config.mode = GPIO_MODE_INPUT;
            break;

        case PinMode::Output:
            config.mode = GPIO_MODE_OUTPUT;
            break;

        case PinMode::InputPullup:
            config.mode = GPIO_MODE_INPUT;
            config.pull_up_en = GPIO_PULLUP_ENABLE;
            break;

        case PinMode::InputPulldown:
            config.mode = GPIO_MODE_INPUT;
            config.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
    }

    gpio_config(&config);
}

PinMode Pin::getMode() const {
	return mode_;
}

bool Pin::high() const {
	return read() == PinState::High;
}

bool Pin::low() const {
	return read() == PinState::Low;
}

void Pin::toggle() {
    write(
        state_ == PinState::High
            ? PinState::Low
            : PinState::High
    );
}

}