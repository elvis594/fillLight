#include <Arduino.h>
#include "app/light_control.h"

void setup() {
  light_control_init();
}

void loop() {
  light_control_update();
  delay(10); // Add a small delay to prevent busy-waiting
}