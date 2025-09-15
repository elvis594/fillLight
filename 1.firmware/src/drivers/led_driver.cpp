#include <Arduino.h>
#include "led_driver.h"
#include "../config.h"
// ==== 编译期防冲突（强烈建议加）====

void led_init() {
    ledcSetup(WARM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(WARM_WHITE_PIN, WARM_CHANNEL);

    ledcSetup(COOL_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(COOL_WHITE_PIN, COOL_CHANNEL);
    
    ledcSetup(UV_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(UV_LED_PIN, UV_CHANNEL);
}

void set_warm_white(uint8_t duty_cycle) {
    ledcWrite(WARM_CHANNEL, duty_cycle);
}

void set_cool_white(uint8_t duty_cycle) {
    ledcWrite(COOL_CHANNEL, duty_cycle);
}

void set_uv_led(uint8_t duty_cycle) {
    ledcWrite(UV_CHANNEL, duty_cycle);
}