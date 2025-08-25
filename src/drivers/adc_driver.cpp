#include "adc_driver.h"
#include "../config.h"

void adc_init() {
    pinMode(POT1_PIN, INPUT);
    pinMode(POT2_PIN, INPUT);
}

uint16_t read_pot1() {
    return analogRead(POT1_PIN);
}

uint16_t read_pot2() {
    return analogRead(POT2_PIN);
}