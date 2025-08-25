#include <Arduino.h>
#include "app/light_control.h"

void setup() {
  Serial.begin(115200);
  light_control_init();
}

void loop() {
  light_control_update();
  delay(10); // Add a small delay to prevent busy-waiting
}