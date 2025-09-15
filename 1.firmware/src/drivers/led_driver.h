#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <Arduino.h>

void led_init();
void set_warm_white(uint8_t duty_cycle);
void set_cool_white(uint8_t duty_cycle);
void set_uv_led(uint8_t duty_cycle);

#endif // LED_DRIVER_H