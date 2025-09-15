#include "encoder_driver.h"
#include <Arduino.h>
#include "../config.h"

// Simple quadrature encoder driver with per-channel interrupt
// Two encoders: one controls brightness, the other controls CCT
// For stability, we accumulate raw counts and expose +/- deltas on read

static volatile int16_t s_brightness_delta = 0;
static volatile int16_t s_cct_delta = 0;

// Per-encoder state
static volatile uint8_t s_lastAB_bright = 0;
static volatile uint8_t s_lastAB_cct = 0;
static volatile uint32_t s_last_us_bright = 0;  // debounce timestamp
static volatile uint32_t s_last_us_cct = 0;     // debounce timestamp

static inline uint8_t readAB(uint8_t pinA, uint8_t pinB) {
    uint8_t a = digitalRead(pinA);
    uint8_t b = digitalRead(pinB);
    return (a << 1) | b;
}

static inline int8_t transition_dir(uint8_t prev, uint8_t cur) {
    switch ((prev << 2) | cur) {
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            return +1;
        case 0b0010:
        case 0b0100:
        case 0b1101:
        case 0b1011:
            return -1;
        default:
            return 0;
    }
}

static void IRAM_ATTR onChangeBrightness() {
    uint32_t now = micros();
    if (now - s_last_us_bright < 500) { // ~0.5ms guard
        return;
    }
    s_last_us_bright = now;

    uint8_t ab = readAB(BRIGHTNESS_ENCODER_A_PIN, BRIGHTNESS_ENCODER_B_PIN);
    uint8_t prev = s_lastAB_bright;
    s_lastAB_bright = ab;

    int8_t dir = transition_dir(prev, ab);
    if (dir) {
        s_brightness_delta += dir;
    }
}

static void IRAM_ATTR onChangeCCT() {
    uint32_t now = micros();
    if (now - s_last_us_cct < 500) { // ~0.5ms guard
        return;
    }
    s_last_us_cct = now;

    uint8_t ab = readAB(CCT_ENCODER_A_PIN, CCT_ENCODER_B_PIN);
    uint8_t prev = s_lastAB_cct;
    s_lastAB_cct = ab;

    int8_t dir = transition_dir(prev, ab);
    if (dir) {
        s_cct_delta += dir;
    }
}

void encoder_init() {
    // Configure pins for both encoders
    pinMode(BRIGHTNESS_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(BRIGHTNESS_ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(CCT_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(CCT_ENCODER_B_PIN, INPUT_PULLUP);

    // Initialize last states and debounce baselines
    s_lastAB_bright = readAB(BRIGHTNESS_ENCODER_A_PIN, BRIGHTNESS_ENCODER_B_PIN);
    s_lastAB_cct = readAB(CCT_ENCODER_A_PIN, CCT_ENCODER_B_PIN);
    uint32_t t0 = micros();
    s_last_us_bright = t0 - 10000;
    s_last_us_cct = t0 - 10000;

    // Attach interrupts on both A/B channels for each encoder
    attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENCODER_A_PIN), onChangeBrightness, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENCODER_B_PIN), onChangeBrightness, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CCT_ENCODER_A_PIN), onChangeCCT, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CCT_ENCODER_B_PIN), onChangeCCT, CHANGE);
}

void encoder_update() {
    // Nothing required; ISRs fill deltas
}

int16_t encoder_get_brightness_delta() {
    noInterrupts();
    int16_t d = s_brightness_delta;
    s_brightness_delta = 0;
    interrupts();
    return d;
}

int16_t encoder_get_cct_delta() {
    noInterrupts();
    int16_t d = s_cct_delta;
    s_cct_delta = 0;
    interrupts();
    return d;
}
