# Enya Guitar ESP32 Controller

![ESP32 Guitar Controller](https://img.shields.io/badge/ESP32-BLE_Controller-blue)
![Open Source](https://img.shields.io/badge/Open%20Source-MIT-green)

An open-source Bluetooth Low Energy (BLE) controller for Enya guitars that allows you to change presets and toggle pedals without using the official app or built-in buttons. Perfect for creating a custom pedalboard interface.

## Features

- **Preset Control**: Switch between 4 different presets (0-3)
- **Pedal Toggle**: Turn on/off 5 different effects:
  - Equalizer
  - Modulation
  - Noise Gate
  - Delay
  - Reverb
- **Hardware Interface**: Connect physical buttons for tactile control
- **Auto-Reconnect**: Automatically reconnects if connection is lost
- **Open Protocol**: Reverse-engineered BLE protocol for full control
- **Serial Commands**: Send commands to device over serial (arduino-IDE, putty...)

## Hardware Requirements

- ESP32 development board
- 4 push buttons (for presets)
- 5 push buttons (for pedals)
- Optional: LED for status indicator

## Wiring Guide

| ESP32 GPIO | Connected To         |
|------------|----------------------|
| 12         | Preset 0 Button       |
| 13         | Preset 1 Button       |
| 14         | Preset 2 Button       |
| 15         | Preset 3 Button       |
| 16         | Pedal 0 (Equalizer)   |
| 17         | Pedal 1 (Modulation)  |
| 18         | Pedal 2 (Noise Gate)  |
| 19         | Pedal 3 (Delay)       |
| 21         | Pedal 4 (Reverb)      |
| 4          | Status LED (optional) |

## Installation

1. Install Arduino IDE with ESP32 support
2. Clone this repository
3. Open `Enya_Guitar_Controller.ino` in Arduino IDE
4. Update the `DEVICE_ADDRESS` with your guitar's MAC address
5. Upload to your ESP32
6. Check your guitar mac address and set in the start of the code in #define DEVICE_ADDRESS      "XX:XX:XX:XX:XX:XX" 
you can use any app like NrfConnect to check the device Mac Addr.

## Usage

1. Power on your Enya guitar
2. Power on the ESP32 controller
3. Buttons:
   - Preset buttons: Immediately switch to the selected preset
   - Pedal buttons: Toggle the effect on/off with each press
4. You can send commands over serial for testing.


## Reverse Engineering Process

The BLE protocol was reverse-engineered using:
- **Frida**: For dynamic analysis of the official app
- **Packet Analysis**: To understand the command structure

The protocol uses a custom service/characteristic UUID with specific byte patterns for each command.

## License

MIT License

Copyright (c) [2025] [your Victor Sousa]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Contributing

Contributions are welcome! Please open an issue or pull request for any:
- Bug fixes
- Additional features
- Documentation improvements
- Hardware integration examples

## Disclaimer

This project is not affiliated with Enya Musical Instruments. Use at your own risk. The authors are not responsible for any damage to your equipment.
