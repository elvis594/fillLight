#ifndef THERMISTOR_DRIVER_H
#define THERMISTOR_DRIVER_H

#include <Arduino.h>

void thermistor_init();
float read_temperature();

#endif // THERMISTOR_DRIVER_H