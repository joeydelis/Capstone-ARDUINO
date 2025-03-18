# Capstone-ARDUINO
ARDUINO Code or 5911 Capstone  

Upload ServerAndLED code onto an ESP32 ARDUINO which is hooked up to LEDs  
Then connect to the device via bluetooth.  
Then send a message such as:  
"ON_0" → Turns on LED 0  
"BLINK_1" → Blinks LED 1  
"BRIGHTNESS_2_180" → Adjusts LED 2 brightness to 180  


Unit testing guide

Requirements:
Install arduino cli

Wokwi Requirements:
Install wokwi cli
Provide your wokwi api key if testing to wokwi cli

Build:

To build for the DOIT esp32 build using:

rm firmware/* -rf

arduino-cli compile -b esp32:esp32:esp32doit-espduino ServerAndLED/ --build-path firmware/ --libraries libraries/ --libraries wokwi_lib --build-property build.extra_flags="-DTESTING -DDOITI"; 

then load firmware to the esp32.

To build for wokwi simulations use:

rm firmware/* -rf

arduino-cli compile -b esp32:esp32:esp32s3 ServerAndLED/ --build-path firmware/ --libraries libraries/ --libraries wokwi_lib --build-property build.extra_flags="-DTESTING -DWOKWI"

then use:
wokwi-cli

to simulate the device.


To build for Nano esp32s3 use: (FQBN may have to be changed to arduino:esp32:nano_nora)

rm firmware/* -rf

arduino-cli compile -b esp32:esp32:nano_nora ServerAndLED/ --build-path firmware/ --libraries libraries/ --libraries wokwi_lib --build-property build.extra_flags="-DTESTING -DDOITI"; 
