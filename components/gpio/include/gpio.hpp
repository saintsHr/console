#pragma once

#include <stdint.h>

namespace console::drivers {

using PinNumber = int8_t;

constexpr PinNumber GPIO_NOT_CONNECTED = -1;

enum class PinMode {
	Input,
	Output,
	InputPullup,
	InputPulldown
};

enum class PinState {
	Low  = 0,
	High = 1,
};

class Pin {
public:
    explicit Pin(PinNumber pin);

    void write(PinState state);
    PinState read() const;

    void setMode(PinMode mode);
    PinMode getMode() const;

    PinNumber getPin() const;

    bool high() const;
    bool low() const;
    void toggle();
private:
    PinNumber pin_ = 0;
    PinMode mode_ = PinMode::Input;
    PinState state_ = PinState::Low;
};

}