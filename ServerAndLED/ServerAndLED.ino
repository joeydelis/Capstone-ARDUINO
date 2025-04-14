#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <vector>
#include "sdkconfig.h"
#include "esp_task_wdt.h"
#include <queue>
#include "Timer.h"
#include "Device.h"

#define SERVICE_UUID "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdef123456"


/*
  Keep note of the cores that are being used by xTaskCreatePinnedToCore and FlexyDriver (in Device.h)
  as the esp32 only has two performance cores which if a core is overused, may see a decrease in perfomance.

  governor.h will not be pursued to monitor each device connected because it was planned to actively check 
  each device which will take resources as it will be constantly running on the core. This would interfere
  with Stepper motor and timer functions.
*/



/*
  Build property flags

  To test a specific 
*/
// #define PRODUCTION
// #define TESTING
// #define BLETEST
// #ifdef TESTING
#include <AUnit.h>
#include "tests/AunitTests.cpp"
// #endif

// Predefined pins that are used with the test esp32 device

#define PIN_LED0 27
#define PIN_LED1 26
#define PIN_LED2 25



// Nano pins
// #define PIN_LED0 1
// #define PIN_LED1 2
// #define PIN_LED2 3


// Motor Pins
// #define PIN_Stepper0 6
// #define PIN_Stepper1 8


using std::vector;

// setup for having multiple tasks
TaskHandle_t timeTaskHandler = NULL;
TaskHandle_t timeWatcherTaskHandler = NULL;
BaseType_t xTaskWokenByReceive = pdFALSE;  // default is token is has not been successfully recieved from xQueueRecieveFromISR
BaseType_t xTaskWokenByReceive2 = pdFALSE;


vector<Driver> drivers(2);
vector<LED> leds(3);  // leds hold the LED structs
Timer timeoutClock;   // Timer that will countdown to ending the currently used devices
BaseType_t xHigherPriorityTaskWoken = pdFALSE;


// BLE setup
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
// testing for the speed requirement
#ifdef BLETEST
  long initialTimeBLE = millis();
#endif

  if (command.startsWith("ON_")) {
    int led = command.substring(3).toInt();
    // light(getLedPin(led));
    leds.at(led).light(1);
    sendConfirmation("Turned ON LED " + String(led));
  } else if (command.startsWith("OFF_")) {
    int led = command.substring(4).toInt();
    // digitalWrite(getLedPin(led), LOW); // pinMode(PIN_LED0, OUTPUT);
    // pinMode(PIN_LED1, OUTPUT);
    // pinMode(PIN_LED2, OUTPUT);
    leds.at(led).light(0);
    sendConfirmation("Turned OFF LED " + String(led));
  } else if (command.startsWith("BLINK_")) {
    int led = command.substring(6).toInt();
    // blink(getLedPin(led));
    leds.at(led).blink();
    sendConfirmation("Blinked LED " + String(led));
  } else if (command.startsWith("BRIGHTNESS_")) {
    int firstUnderscore = command.indexOf('_', 11);
    int led = command.substring(11, firstUnderscore).toInt();
    int brightness = command.substring(firstUnderscore + 1).toInt();
    // changeBrightness(getLedPin(led), brightness);

    leds.at(led).changeBrightness(brightness);
    sendConfirmation("Set brightness of LED " + String(led) + " to " + String(brightness));
  } else if (command.startsWith("time")) {
    // int led = command.substring(6).toInt();
    // blink(getLedPin(led));

    unsigned long currentTime;
    xQueueReceiveFromISR(timeRequestQueue, &currentTime, &xTaskWokenByReceive2);

    Serial.println(currentTime);
    sendConfirmation("Time: " + String(currentTime));
  } else if (command.startsWith("speed_")) {
    int speed = command.substring(6).toInt();

    ;
    sendConfirmation("A speed of " + String(speed) + "has been applied");
  } else if (command.startsWith("step_")) {
    int step = command.substring(5).toInt();

  
    sendConfirmation("A step of " + String(step) + "has been applied");
  }

  else {
    sendConfirmation("Invalid command received");
  }
#ifdef BLETEST
  long totalTimeBLE = millis() - initialTimeBLE;
  Serial.println("Command and Confirmation took " + String(totalTimeBLE) + "ms");
#endif
}
// Callback to process received BLE messages
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {

    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      Serial.print("Received message: ");
      Serial.println(value.c_str());

      String command = String(value.c_str());
      processCommand(command);
    }
  }
};




/*
  Starts timer task
  This starts as soon as the system starts
*/
void startTimer(void* params) {
  // The main logic is handled by the BLE callback; no need to put logic in loop.
  while (1) {
    timeoutClock.stopwatch(TIMEOUTCLOCK);
  }
}
/*
  Watches the value of from the timer
*/
void timeWatcher(void* params) {
  while (1) {
    unsigned long currentTime;


    xQueueReceiveFromISR(timeQueue, &currentTime, &xTaskWokenByReceive);           // receives time while the stopwatch function is counting.
    xQueueSendFromISR(timeRequestQueue, &currentTime, &xHigherPriorityTaskWoken);  // Sends time to any time requesting functions
  }
}


void setup() {
  Serial.begin(115200);
  esp_task_wdt_deinit();
  // Creating a queue that only holds the current time.
  timeQueue = xQueueCreate(1, sizeof(int));
  timeRequestQueue = xQueueCreate(1, sizeof(int));
  // Initialize LEDs

  // leds = {};//(LED*)calloc(sizeof(LED),3); // assigning memory to each LED struct
  // #ifndef TESTING
  // Serial.println("Starting BLE Server...");
  // leds.at(0).pin = PIN_LED0;
  // leds.at(1).pin = PIN_LED1;
  // leds.at(2).pin = PIN_LED2;
  // pinMode(leds.at(0).pin, OUTPUT);
  // pinMode(leds.at(1).pin, OUTPUT);
  // pinMode(leds.at(2).pin, OUTPUT);

  // Initialize Steppers

  // Stepper library way
  // drivers.at(0).createStepper(9,5,6,13);
  // drivers.at(0).createStepper(9,5,6,13);
  // drivers.at(0).setSpeed(10);
  // int step = 17;
  // int dir = 10;
  // int enable = 6;
  // int extra = 13;

  // int step = 9;
  // int dir = 8;
  // int enable = 7;
  // int extra = 13;

  int step = 6;
  int dir = 9;
  int enable = 8;
  int extra = 13;
  // driver.createStepper(enable, step, dir, extra);
  // driver.setSpeed(10);


#ifndef TESTING
  // Initialize BLE
  BLEDevice::init("Migraine Mitigatar V1");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE Service
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());  // Enables notifications

  // Start the service
  pService->start();

  // Start BLE Advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();

  Serial.println("Migraine Mitigator started. Waiting for connections...");
#endif
  /*
      XtaskCreatePinnedToCore will create a separate task that will run along with the main program
      and assign it to a given core.
    */
  xTaskCreatePinnedToCore(
    timeWatcher,
    "timeWatcher",
    1000,
    NULL,
    0,  // Core priority
    &timeTaskHandler,
    0  // Core it's being run on
  );
  xTaskCreatePinnedToCore(
    startTimer,
    "startTimer",
    1000,
    NULL,
    0,  // Core priority
    &timeWatcherTaskHandler,
    0  // Core it's being run on
  );
#ifdef TESTING
#ifdef DOIT
  TestRunner::exclude("*");
  TestRunner::include("NonBLETestsDOIT", "*");
#endif
#ifdef WOKWI
  TestRunner::exclude("*");
  TestRunner::include("NonBLETestsWokwi", "*");
#endif
#ifdef NANO
  TestRunner::exclude("*");
  TestRunner::include("NonBLETestsNano", "*");
#endif
#endif
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

void loop() {
  // Testing space



#ifdef TESTING
  TestRunner::run();
#endif
}
