#include "input.hpp"

#include "input.hpp"

namespace console {

Input::Input(
    drivers::PinNumber pinUp,
    drivers::PinNumber pinDown,
    drivers::PinNumber pinLeft,
    drivers::PinNumber pinRight,
    drivers::PinNumber pinA,
    drivers::PinNumber pinB,
    drivers::PinNumber pinSelect,
    drivers::PinNumber pinStart
) : pins_{
        drivers::Pin(pinUp),
        drivers::Pin(pinDown),
        drivers::Pin(pinLeft),
        drivers::Pin(pinRight),
        drivers::Pin(pinA),
        drivers::Pin(pinB),
        drivers::Pin(pinSelect),
        drivers::Pin(pinStart),
    } {
    for (auto& pin : pins_) {
        pin.setMode(drivers::PinMode::InputPullup);
    }

    rawLast_.fill(false);
    current_.fill(false);
    previous_.fill(false);
    stableCount_.fill(0);
}

void Input::update() {
    previous_ = current_;

    for (size_t i = 0; i < COUNT; i++) {
        bool rawPressed = pins_[i].low();

        if (rawPressed == rawLast_[i]) {
            if (stableCount_[i] < DEBOUNCE_FRAMES) stableCount_[i]++;
        } else {
            stableCount_[i] = 0;
            rawLast_[i] = rawPressed;
        }

        if (stableCount_[i] >= DEBOUNCE_FRAMES) {
            current_[i] = rawPressed;
        }
    }
}

bool Input::isHeld(Button btn) const {
    return current_[static_cast<size_t>(btn)];
}

bool Input::justPressed(Button btn) const {
    size_t i = static_cast<size_t>(btn);
    return current_[i] && !previous_[i];
}

bool Input::justReleased(Button btn) const {
    size_t i = static_cast<size_t>(btn);
    return !current_[i] && previous_[i];
}

}