#pragma once

#include <cstdint>
#include <driver/ledc.h>

namespace console::drivers {

using PinNumber = int8_t;

enum class PwmResolution : uint8_t {
    Bits8  = LEDC_TIMER_8_BIT,
    Bits10 = LEDC_TIMER_10_BIT,
    Bits12 = LEDC_TIMER_12_BIT
};

class Pwm {
public:
    Pwm(
        PinNumber pin,
        uint32_t frequency,
        PwmResolution resolution
    );

    ~Pwm();

    void setDuty(uint8_t percent);
    uint8_t getDuty() const;

    PinNumber getPin() const;

    void stop();
private:
    static ledc_channel_t allocateChannel();

    PinNumber pin_;
    uint32_t frequency_;
    PwmResolution resolution_;

    ledc_channel_t channel_;
    ledc_timer_t timer_;
    ledc_mode_t mode_;

    uint8_t duty_ = 0;
};

}