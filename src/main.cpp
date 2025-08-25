#include <Arduino.h>

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}

void loop() {
  // print out "Hello World!"
  Serial.println("Hello World!");
  delay(1000);
}