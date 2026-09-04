#include "pwm.hpp"
#include <soc/gpio_num.h>

namespace console::drivers {

static ledc_channel_t nextChannel = LEDC_CHANNEL_0;

ledc_channel_t Pwm::allocateChannel() {
    auto channel = nextChannel;

    nextChannel =
        static_cast<ledc_channel_t>(
            static_cast<int>(nextChannel) + 1
        );

    return channel;
}



Pwm::Pwm(
    PinNumber pin,
    uint32_t frequency,
    PwmResolution resolution
)
    :
    pin_(pin),
    frequency_(frequency),
    resolution_(resolution),
    channel_(allocateChannel()),
    timer_(LEDC_TIMER_0),
    mode_(LEDC_LOW_SPEED_MODE)
{

    ledc_timer_config_t timer = {};

    timer.speed_mode = mode_;
    timer.timer_num = timer_;

    timer.duty_resolution =
        static_cast<ledc_timer_bit_t>(resolution_);
    timer.freq_hz = frequency_;
    timer.clk_cfg = LEDC_AUTO_CLK;

    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {};

    channel.gpio_num =
        static_cast<gpio_num_t>(pin_);
    channel.speed_mode = mode_;
    channel.channel = channel_;
    channel.timer_sel = timer_;
    channel.duty = 0;
    channel.hpoint = 0;

    ledc_channel_config(&channel);
}

Pwm::~Pwm() {
    stop();
}

void Pwm::setDuty(uint8_t percent) {
    if (percent > 100) percent = 100;

    duty_ = percent;

    uint32_t maxDuty =
        (1 << static_cast<uint8_t>(resolution_)) - 1;

    uint32_t duty =
        (maxDuty * percent) / 100;

    ledc_set_duty(
        mode_,
        channel_,
        duty
    );

    ledc_update_duty(
        mode_,
        channel_
    );
}



uint8_t Pwm::getDuty() const {
    return duty_;
}



void Pwm::stop() {
    setDuty(0);
}



PinNumber Pwm::getPin() const {
    return pin_;
}


}