#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <Arduino.h>

void button_init();
bool is_uv_button_pressed();
void button_update();
#endif // BUTTON_DRIVER_H