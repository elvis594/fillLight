#include <Arduino.h>
#include "led_driver.h"
#include "../config.h"

void led_init() {
    ledcAttach(WARM_WHITE_PIN, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(COOL_WHITE_PIN, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(UV_LED_PIN, PWM_FREQ, PWM_RESOLUTION);
}

void set_warm_white(uint8_t duty_cycle) {
    ledcWrite(WARM_WHITE_PIN, duty_cycle);
}

void set_cool_white(uint8_t duty_cycle) {
    ledcWrite(COOL_WHITE_PIN, duty_cycle);
}

void set_uv_led(uint8_t duty_cycle) {
    ledcWrite(UV_LED_PIN, duty_cycle);
}