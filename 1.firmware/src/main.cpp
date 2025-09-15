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


// 定义 LEDC 通道和频率
// const int pwmPin = 7;     // GPIO7
// const int pwmChannel = 0; // 通道0
// const int freq = 5000;    // 5 kHz
// const int resolution = 8; // 8位分辨率 (0-255)

// void setup() {
//   ledcSetup(pwmChannel, freq, resolution); 
//   ledcAttachPin(pwmPin, pwmChannel);
// }

// void loop() {
//   for (int duty = 0; duty <= 255; duty++) {
//     ledcWrite(pwmChannel, duty);
//     delay(5);
//   }
//   for (int duty = 255; duty >= 0; duty--) {
//     ledcWrite(pwmChannel, duty);
//     delay(5);
//   }
// }
