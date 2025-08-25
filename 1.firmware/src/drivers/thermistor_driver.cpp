#include "thermistor_driver.h"
#include "../config.h"
#include <math.h>

void thermistor_init() {
    pinMode(THERMISTOR_PIN, INPUT);
}

float read_temperature() {
    int adc_val = analogRead(THERMISTOR_PIN);
    float resistance = SERIES_RESISTOR * (4095.0f / adc_val - 1.0f);
    
    float steinhart;
    steinhart = resistance / THERMISTOR_NOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= B_COEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C

    return steinhart;
}