#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions
#define WARM_WHITE_PIN 4 // PWM_N
#define COOL_WHITE_PIN 7 // PWM_L
#define UV_LED_PIN 5     // PWM-UV

//pwm channel
#define COOL_CHANNEL 0
#define WARM_CHANNEL 1
#define UV_CHANNEL 2

#define POT1_PIN 0 // ADC1
#define POT2_PIN 1 // ADC2

#define UV_BUTTON_PIN 9 // IO4
#define THERMISTOR_PIN 10 // IO10

// Encoder (optional) — used when building encoder version
// Adjust these pins to your hardware wiring if needed
#define BRIGHTNESS_ENCODER_A_PIN 0   // IO0
#define BRIGHTNESS_ENCODER_B_PIN 1   // IO1

#define CCT_ENCODER_A_PIN 2   // IO2
#define CCT_ENCODER_B_PIN 3   // IO3

#define ENCODER_STEP 32

// Thermistor Configuration
#define THERMISTOR_NOMINAL 10000.0f   // Nominal resistance at 25°C
#define TEMPERATURE_NOMINAL 25.0f     // Temperature for nominal resistance (in Celsius)
#define B_COEFFICIENT 3950.0f         // The B-value of the thermistor
#define SERIES_RESISTOR 10000.0f      // The value of the resistor in series with the thermistor
#define MAX_TEMP_C 80.0f              // Maximum temperature in Celsius before printing a warning

// PWM Configuration
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8
#define PWM_MAX ((1U << PWM_RESOLUTION) - 1)

#endif // CONFIG_H