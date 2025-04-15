# Migraine Mitigation ESP32 [5911 Capstone]

This software is designed to be run on the arduino nano ESP32 s3.
This software is for helping migraine patient to recieve myofascial release therapy without having to go to the doctor.

This Project is worked along with App development team and the BME team.

## Installation

This project can be built and installed using  Arduino IDE.


## Unit Testing Guide



Requirements:
Install arduino cli

Esp32 requirements:
install esptool

Wokwi Requirements:
Install wokwi cli
Provide your wokwi api key if testing to wokwi cli

Build:

To build for the DOIT esp32 build using:

Remove any extra files in firmware:
rm firmware/* -rf

Options:
    Unit Testing:
    arduino-cli compile -b esp32:esp32:esp32doit-espduino ServerAndLED/ --build-path firmware/ --libraries libraries/ --libraries wokwi_lib --build-property build.extra_flags="-DTESTING -DDOIT"

    BLE performance testing:
    arduino-cli compile -b esp32:esp32:esp32doit-espduino ServerAndLED/ --build-path firmware/ --libraries libraries/ --libraries wokwi_lib --build-property build.extra_flags="-DBLETEST"


then load firmware to the esp32 with (port depends on what computer you're using):


esptool --chip esp32 --port "/dev/ttyUSB0" --baud 921600  --before default_reset --after hard_reset write_flash -z --flash_mode keep --flash_freq keep --flash_size keep 0x10000 "firmware/ServerAndLED.ino.bin"


To build for wokwi simulations use:

rm firmware/* -rf

arduino-cli compile -b esp32:esp32:esp32s3 ServerAndLED/ --build-path firmware/ --libraries libraries/ --libraries wokwi_lib --build-property build.extra_flags="-DTESTING -DWOKWI"

then use:
wokwi-cli

to simulate the device.


To build for Nano esp32s3 use

rm firmware/* -rf

arduino-cli compile -b arduino:esp32:nano_nora ServerAndLED/ --build-path firmware/ --libraries libraries/ --libraries wokwi_lib --build-property build.extra_flags="-DTESTING -DDOITI"


## Usage/Examples

Upload ServerAndLED code onto an ESP32 ARDUINO which is hooked up to LEDs  
Then connect to the device via bluetooth.  
Then send a message such as:  
"ON_0" → Turns on LED 0  
"BLINK_1" → Blinks LED 1  
"BRIGHTNESS_2_180" → Adjusts LED 2 brightness to 180  
"UP" → To move the motor up
"DOWN"→ To move the motor down