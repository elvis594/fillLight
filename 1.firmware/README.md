# Fill Light Controller

This project is a custom-built fill light controller based on the ESP32-C3. It provides adjustable color temperature, brightness, and a UV mode, making it a versatile tool for photography and videography lighting.

## Features

- **Adjustable Color Temperature:** Smoothly transition between cool white (13000K) and warm white (2700K) using a potentiometer. The adjustment is perceptually linear, thanks to the use of the Mired color temperature scale.
- **Adjustable Brightness:** Control the overall brightness of the light with a dedicated potentiometer.
- **UV Light Mode:** Toggle a UV LED on and off with the press of a button. When UV mode is active, the warm and cool white LEDs are turned off.
- **Flicker-Free Operation:** Utilizes a high-frequency PWM (60kHz) to ensure flicker-free lighting, suitable for video recording.
- **Debounced Button:** The button for toggling UV mode is debounced in software to prevent multiple triggers from a single press.

## Hardware

- **Microcontroller:** ESP32-C3-DevKitM-1
- **LED Driver:** SGM3732YTN6G/TR
- **LEDs:**
    - 1x Cool White (13000K)
    - 1x Warm White (2700K)
    - 1x UV
- **Controls:**
    - 2x Potentiometers (for color temperature and brightness)
    - 1x Push-button (for UV mode)

## Software

- **Framework:** Arduino
- **IDE:** PlatformIO

## Pinout

| Function             | Pin |
| -------------------- | --- |
| Warm White LED       | 7   |
| Cool White LED       | 6   |
| UV LED               | 5   |
| Color Temp Pot       | 0   |
| Brightness Pot       | 1   |
| UV Mode Button       | 4   |

## Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/elvis594/fillLight.git
   ```
2. **Open with PlatformIO:**
   Open the cloned directory in Visual Studio Code with the PlatformIO IDE extension installed.
3. **Build and Upload:**
   Use the PlatformIO interface to build and upload the firmware to your ESP32-C3 device.