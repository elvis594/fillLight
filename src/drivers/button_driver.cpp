#include "button_driver.h"
#include "../config.h"

void button_init() {
    pinMode(UV_BUTTON_PIN, INPUT_PULLUP);
}

bool is_uv_button_pressed() {
    static bool last_state = HIGH;
    static unsigned long last_debounce_time = 0;
    unsigned long debounce_delay = 50;

    bool reading = digitalRead(UV_BUTTON_PIN);

    if (reading != last_state) {
        last_debounce_time = millis();
    }

    if ((millis() - last_debounce_time) > debounce_delay) {
        if (reading == LOW) {
            return true;
        }
    }

    last_state = reading;
    return false;
}