#include "light_control.h"
#include "../drivers/led_driver.h"
#include "../drivers/adc_driver.h"
#include "../drivers/button_driver.h"
#include "../drivers/thermistor_driver.h"
#include "../config.h"
#include <math.h>

// Define the color temperatures of the LEDs in Kelvin
#define COOL_WHITE_K 13000.0f
#define WARM_WHITE_K 2700.0f

// Calculate mired values for more perceptually linear interpolation
const float MIRED_COOL = 1000000.0f / COOL_WHITE_K;
const float MIRED_WARM = 1000000.0f / WARM_WHITE_K;

// Module-level state variables
static bool uv_on = false;
static uint16_t color_temp_raw = 0;
static uint16_t brightness_raw = 0;

// Forward declarations for local functions
static void handle_input();
static void update_leds();
static void check_temperature();
static float map_float(float x, float in_min, float in_max, float out_min, float out_max);


void light_control_init() {
    led_init();
    adc_init();
    button_init();
    thermistor_init();
}

void light_control_update() {
    handle_input();
    update_leds();
    check_temperature();
}

static void handle_input() {
    color_temp_raw = read_pot1();
    brightness_raw = read_pot2();
    if (is_uv_button_pressed()) {
        uv_on = !uv_on;
    }
}

static void update_leds() {
    uint8_t brightness = map(brightness_raw, 0, 4095, 0, 255);

    if (uv_on) {
        set_warm_white(0);
        set_cool_white(0);
        set_uv_led(brightness);
    } else {
        float mired_target = map_float(color_temp_raw, 0, 4095, MIRED_WARM, MIRED_COOL);
        float cool_ratio = (mired_target - MIRED_WARM) / (MIRED_COOL - MIRED_WARM);

        if (cool_ratio < 0.0f) cool_ratio = 0.0f;
        if (cool_ratio > 1.0f) cool_ratio = 1.0f;

        uint8_t warm_white_val = brightness * (1.0f - cool_ratio);
        uint8_t cool_white_val = brightness * cool_ratio;

        set_warm_white(warm_white_val);
        set_cool_white(cool_white_val);
        set_uv_led(0);
    }
}

static void check_temperature() {
    float temperature = read_temperature();
    if (temperature > MAX_TEMP_C) {
        Serial.printf("Over-temperature warning! Current temperature: %.2f C\n", temperature);
    }
}

// Helper function to map a float value from one range to another
static float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}