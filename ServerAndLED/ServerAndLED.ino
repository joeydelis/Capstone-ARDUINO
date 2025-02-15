#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "Device.h" // will structs for devices connected to the esp32

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdef123456"

#define PIN_LED0 27
#define PIN_LED1 26
#define PIN_LED2 25

LED *leds; // leds hold the LED structs

int frequency = 1000; 


BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;

// Callback to handle BLE client connections
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Device connected!");
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected! Restarting advertisement...");
        BLEDevice::startAdvertising();
    }
};
// Function to process commands received via BLE
void processCommand(String command) {
    if (command.startsWith("ON_")) {
        int led = command.substring(3).toInt();
        // light(getLedPin(led));
        leds[led].light(1);
        sendConfirmation("Turned ON LED " + String(led));
    } 
    else if (command.startsWith("OFF_")) {
        int led = command.substring(4).toInt();
        digitalWrite(getLedPin(led), LOW); // pinMode(PIN_LED0, OUTPUT);
    // pinMode(PIN_LED1, OUTPUT);
    // pinMode(PIN_LED2, OUTPUT);
      leds[led].light(0);
        sendConfirmation("Turned OFF LED " + String(led));
    } 
    else if (command.startsWith("BLINK_")) {
        int led = command.substring(6).toInt();
        // blink(getLedPin(led));
         leds[led].blink();
        sendConfirmation("Blinked LED " + String(led));
    } 
    else if (command.startsWith("BRIGHTNESS_")) {
        int firstUnderscore = command.indexOf('_', 11);
        int led = command.substring(11, firstUnderscore).toInt();
        int brightness = command.substring(firstUnderscore + 1).toInt();
        // changeBrightness(getLedPin(led), brightness);
        leds[led].changeBrightness( brightness);
        sendConfirmation("Set brightness of LED " + String(led) + " to " + String(brightness));
    } 
    else {
        sendConfirmation("Invalid command received");
    }
}
// Callback to process received BLE messages
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        String value = pCharacteristic->getValue();
        if (value.length() > 0) {
            Serial.print("Received message: ");
            Serial.println(value.c_str());

            String command = String(value.c_str());
            processCommand(command);
        }
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE Server...");


    // Initialize LEDs

    leds = (LED*)calloc(sizeof(LED),3); // assigning memory to each LED struct
    
    pinMode(leds[0].pin, OUTPUT);
    pinMode(leds[1].pin, OUTPUT);
    pinMode(leds[2].pin, OUTPUT);


    // Initialize BLE
    BLEDevice::init("ESP32_BLE_Server");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE Service
    BLEService* pService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->addDescriptor(new BLE2902()); // Enables notifications

    // Start the service
    pService->start();

    // Start BLE Advertising
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pServer->getAdvertising()->start();

    Serial.println("BLE Server Started. Waiting for connections...");
}

void loop() {
    // The main logic is handled by the BLE callback; no need to put logic in loop.
}



// Helper function to get LED pin from index
int getLedPin(int ledIndex) {
    switch (ledIndex) {
        case 0: return PIN_LED0;
        case 1: return PIN_LED1;
        case 2: return PIN_LED2;
        default: return -1;
    }
}

// Function to send confirmation back to the phone
void sendConfirmation(String message) {
    if (deviceConnected) {
        pCharacteristic->setValue(message.c_str());
        pCharacteristic->notify();
        Serial.println("Sent: " + message);
    }
}


