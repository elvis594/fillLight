#include "light_control.h"
#include "../drivers/led_driver.h"
#include "../drivers/adc_driver.h"
#include "../drivers/button_driver.h"
#include "../config.h"

// Define the color temperatures of the LEDs in Kelvin
#define COOL_WHITE_K 13000.0f
#define WARM_WHITE_K 2700.0f

// Calculate mired values for more perceptually linear interpolation
const float MIRED_COOL = 1000000.0f / COOL_WHITE_K; // ~769.23
const float MIRED_WARM = 1000000.0f / WARM_WHITE_K; // ~370.37

bool uv_on = false;

// Helper function to map a float value from one range to another
float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void light_control_init() {
    led_init();
    adc_init();
    button_init();
}

void light_control_update() {
    uint16_t color_temp_raw = read_pot1(); // ADC1 for color temperature
    uint16_t brightness_raw = read_pot2(); // ADC2 for brightness

    uint8_t brightness = map(brightness_raw, 0, 4095, 0, 255);

    if (is_uv_button_pressed()) {
        uv_on = !uv_on;
    }

    if (uv_on) {
        set_warm_white(0);
        set_cool_white(0);
        set_uv_led(brightness);
    } else {
        // Map potentiometer to the mired color temperature range
        // Note: We map from WARM to COOL so that turning the pot in one direction
        // moves from warm to cool.
        float mired_target = map_float(color_temp_raw, 0, 4095, MIRED_WARM, MIRED_COOL);

        // Calculate the mixing ratio (0.0 for 100% warm, 1.0 for 100% cool)
        float cool_ratio = (mired_target - MIRED_WARM) / (MIRED_COOL - MIRED_WARM);
        
        // Clamp ratio to be safe
        if (cool_ratio < 0.0f) cool_ratio = 0.0f;
        if (cool_ratio > 1.0f) cool_ratio = 1.0f;

        uint8_t warm_white_val = brightness * (1.0f - cool_ratio);
        uint8_t cool_white_val = brightness * cool_ratio;

        set_warm_white(warm_white_val);
        set_cool_white(cool_white_val);
        set_uv_led(0);
    }
}