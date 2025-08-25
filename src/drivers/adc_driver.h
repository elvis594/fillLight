#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <Arduino.h>

void adc_init();
uint16_t read_pot1();
uint16_t read_pot2();

#endif // ADC_DRIVER_H