#include "light_control.h"
#include "../drivers/led_driver.h"
#include "../drivers/adc_driver.h"
#include "../drivers/button_driver.h"
#include "../drivers/thermistor_driver.h"
#include "../config.h"
#include <math.h>

#ifdef USE_ENCODER_VERSION
#include "../drivers/encoder_driver.h"
#endif

// Define the color temperatures of the LEDs in Kelvin
#define COOL_WHITE_K 13000.0f
#define WARM_WHITE_K 2700.0f

// Calculate mired values for more perceptually linear interpolation
const float MIRED_COOL = 1000000.0f / COOL_WHITE_K;
const float MIRED_WARM = 1000000.0f / WARM_WHITE_K;

// Module-level state variables
// static bool uv_on = false; // 屏蔽UV控制
static uint16_t color_temp_raw = 500;
static uint16_t brightness_raw = 500;
static bool uv_active = false; // 按下按键4时开启UV并关闭其他两路
static unsigned long last_debug_ms = 0; // 调试打印节流

#ifdef USE_ENCODER_VERSION
static const int ENC_STEP = ENCODER_STEP; // 每个编码器脉冲对应的原始步进
#endif

// Forward declarations for local functions
static void handle_input();
static void update_leds();
static void check_temperature();
static float map_float(float x, float in_min, float in_max, float out_min, float out_max);


void light_control_init() {
    led_init();
    adc_init();
    button_init();
#ifdef USE_ENCODER_VERSION
    encoder_init();
#endif
    // thermistor_init();
}

void light_control_update() {
    handle_input();
    update_leds();
    // 周期性打印：按钮状态 + NTC温度 + 当前PWM值
    if (millis() - last_debug_ms >= 300) {
        // float temperature = read_temperature(); // NTC温度
        float temperature = 0.0f; // NTC温度

        uint16_t brightness_pwm = map(brightness_raw, 0, 4095, 0, PWM_MAX);
        if (uv_active) {
            Serial.printf("BTN4=%s | NTC(IO2)=%.2fC | UV=%u (Brightness) | WW=0 | CW=0\n",
                          uv_active ? "PRESSED" : "RELEASED", temperature, brightness_pwm);
        } else {
            // 计算两路占比（线性混色：color_temp_raw 越大，冷白占比越大）
            float t = color_temp_raw / 4095.0f; // 0..1
            float warm_ratio = 1.0f - t;
            float cool_ratio = t;
            uint16_t warm_white_val = (uint16_t)roundf(brightness_pwm * warm_ratio);
            uint16_t cool_white_val = (uint16_t)roundf(brightness_pwm * cool_ratio);
            Serial.printf("BTN4=%s | NTC(IO2)=%.2fC | WW=%u | CW=%u (Bright=%u, CT_raw=%u)\n",
                          uv_active ? "PRESSED" : "RELEASED", temperature,
                          warm_white_val, cool_white_val, brightness_raw, color_temp_raw);
        }
        last_debug_ms = millis();
    }
    // check_temperature();
}

static void handle_input() {
    button_update();

#ifdef USE_ENCODER_VERSION
    // 使用编码器：增量调节亮度与色温，支持编码器按键或BTN4进入UV
    encoder_update();
    int16_t dB = encoder_get_brightness_delta();
    int16_t dC = encoder_get_cct_delta();
    if (dB != 0) {
        int32_t v = (int32_t)brightness_raw + (int32_t)dB * ENC_STEP;
        if (v < 0) v = 0; if (v > 4095) v = 4095;
        brightness_raw = (uint16_t)v;
    }
    if (dC != 0) {
        int32_t v = (int32_t)color_temp_raw + (int32_t)dC * ENC_STEP;
        if (v < 0) v = 0; if (v > 4095) v = 4095;
        color_temp_raw = (uint16_t)v;
    }
    uv_active = is_uv_button_pressed();
#else
    color_temp_raw = read_pot1();  // POT1: 色温/混色（0..4095）
    brightness_raw = read_pot2();  // POT2: 亮度（0..4095）
    // 按下按键4：进入UV模式（UV亮度=当前两路LED的亮度），其他两路关
    uv_active = is_uv_button_pressed();
#endif
}

static void update_leds() {
    // 统一计算亮度PWM值
    uint16_t brightness_pwm = map(brightness_raw, 0, 4095, 0, PWM_MAX);

    if (uv_active) {
        set_warm_white(0);
        set_cool_white(0);
        set_uv_led((uint8_t)brightness_pwm); // UV 亮度=两路LED当前亮度（Brightness旋钮/编码器）
        return;
    }

    // 未进入UV模式：按亮度与色温混色输出两路白光
    float t = color_temp_raw / 4095.0f; // 0..1，越大越偏冷白
    float warm_ratio = 1.0f - t;
    float cool_ratio = t;

    uint16_t warm_white_val = (uint16_t)roundf(brightness_pwm * warm_ratio);
    uint16_t cool_white_val = (uint16_t)roundf(brightness_pwm * cool_ratio);

    set_warm_white((uint8_t)warm_white_val);
    set_cool_white((uint8_t)cool_white_val);
    set_uv_led(0); // UV保持关闭
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