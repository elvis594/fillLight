#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions
#define WARM_WHITE_PIN 7 // PWM_N
#define COOL_WHITE_PIN 6 // PWM_L
#define UV_LED_PIN 5     // PWM-UV

#define POT1_PIN 0 // ADC1
#define POT2_PIN 1 // ADC2

#define UV_BUTTON_PIN 4 // IO4

// PWM Configuration
#define PWM_FREQ 60000
#define PWM_RESOLUTION 8

#endif // CONFIG_H