#pragma once

#include "gpio.hpp"
#include <array>
#include <cstdint>

namespace console {

enum class Button : uint8_t {
    Up,
    Down,
    Left,
    Right,
    A,
    B,
    Select,
    Start,
    Count
};

class Input {
public:
    Input(
        drivers::PinNumber pinUp,
        drivers::PinNumber pinDown,
        drivers::PinNumber pinLeft,
        drivers::PinNumber pinRight,
        drivers::PinNumber pinA,
        drivers::PinNumber pinB,
        drivers::PinNumber pinSelect,
        drivers::PinNumber pinStart
    );

    void update();

    bool isHeld(Button btn) const;
    bool justPressed(Button btn) const;
    bool justReleased(Button btn) const;

private:
    static constexpr size_t COUNT = static_cast<size_t>(Button::Count);
    static constexpr uint8_t DEBOUNCE_FRAMES = 1;

    std::array<drivers::Pin, COUNT> pins_;
    std::array<bool, COUNT> current_{};
    std::array<bool, COUNT> previous_{};
    std::array<uint8_t, COUNT> stableCount_{};
    std::array<bool, COUNT> rawLast_{};
};

}