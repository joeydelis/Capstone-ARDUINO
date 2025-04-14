#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <WString.h>
#include "sdkconfig.h"
#include "esp_task_wdt.h"
#include <queue>
#include <Preferences.h>
#include "Timer.h"
#include "Device.h"


// UUID Generated from https://www.guidgenerator.com/
#define SERVICE_UUID "c93e8091-1b04-4258-8ac2-2588e890e121"
#define CHARACTERISTIC_UUID "e7467b73-034c-4f44-8afc-4cac0be2db0b"


/*
  Keep note of the cores that are being used by xTaskCreatePinnedToCore and FlexyDriver (in Device.h)
  as the esp32 only has two performance cores which if a core is overused, may see a decrease in perfomance.

  governor.h will not be pursued to monitor each device connected because it was planned to actively check 
  each device which will take resources as it will be constantly running on the core. This would interfere
  with Stepper motor and timer functions.
*/



/*
  Build property flags

  To unit test a specific device uncomment TESTING and
  a esp32 device such as:
    DOIT for the doit esp32
    NANO for the arduino nano esp32
   
  To build for WOWKI refer to the Unit testing branch for building and running
*/

// #define TESTING
// #define BLETEST
// #define DOIT;
// #define WOKWI
// #define NANO
#ifdef TESTING
#include <AUnit.h>
#include "tests/AunitTests.cpp"
#endif

// #define ENA_PIN_1 4  // Adjust based on wiring/if needed by driver
// #define ENA_PIN_2 5

// // Define stepper motor control pins
// #define STEP_PIN_1 32  // Step pin for Motor 1
// #define DIR_PIN_1 33   // Direction pin for Motor 1
// #define STEP_PIN_2 14  // Step pin for Motor 2
// #define DIR_PIN_2 12   // Direction pin for Motor 2

// #define MOTOR_SPEED_DELAY 500  // Microseconds between steps (adjust for speed)
// #define STEPS_PER_REV 200      // Adjust based on your stepper motor specs
// Stepper stepper1(STEPS_PER_REV, DIR_PIN_1, STEP_PIN_1);
// Stepper stepper2(STEPS_PER_REV, DIR_PIN_2, STEP_PIN_2);

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
using std::string;


// setup for having multiple tasks
TaskHandle_t timeTaskHandler = NULL;
TaskHandle_t timeWatcherTaskHandler = NULL;
BaseType_t xTaskWokenByReceive = pdFALSE;  // default is token is has not been successfully recieved from xQueueRecieveFromISR
BaseType_t xTaskWokenByReceive2 = pdFALSE;

// 
vector<Driver> drivers(2);
vector<LED> leds(3);  // leds hold the LED structs
Timer timeoutClock;   // Timer that will countdown to ending the currently used devices


BaseType_t xHigherPriorityTaskWoken = pdFALSE;
Preferences preferences;


// BLE setup
BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;
void shutdownTimerTask(void* param);
// Callback to handle BLE client connections
class MyServerCallbacks : public BLEServerCallbacks {

  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected!");
    BLEDevice::getAdvertising()->stop();  // Stop advertising to prevent new connections
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected! Starting shutdown timer...");
    BLEDevice::startAdvertising();  // Resume advertising for new connections
    int shutdownTimeout = 30;       // Time in seconds before shutdown
    xTaskCreate(shutdownTimerTask, "ShutdownTimer", 1000, &shutdownTimeout, 1, NULL);
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
#ifdef BLETEST
    long totalTimeBLE = millis() - initialTimeBLE;
    Serial.println("Command and Confirmation took " + String(totalTimeBLE) + "ms");
#endif
  } else if (command.startsWith("BLINK_")) {
    int led = command.substring(6).toInt();
    // blink(getLedPin(led));
    leds.at(led).blink();
    sendConfirmation("Blinked LED " + String(led));
#ifdef BLETEST
    long totalTimeBLE = millis() - initialTimeBLE;
    Serial.println("Command and Confirmation took " + String(totalTimeBLE) + "ms");
#endif
  } else if (command.startsWith("BRIGHTNESS_")) {
    int firstUnderscore = command.indexOf('_', 11);
    int led = command.substring(11, firstUnderscore).toInt();
    int brightness = command.substring(firstUnderscore + 1).toInt();
    // changeBrightness(getLedPin(led), brightness);

    leds.at(led).changeBrightness(brightness);
    sendConfirmation("Set brightness of LED " + String(led) + " to " + String(brightness));
#ifdef BLETEST
    long totalTimeBLE = millis() - initialTimeBLE;
    Serial.println("Command and Confirmation took " + String(totalTimeBLE) + "ms");
#endif
  } else if (command.equals("UP")) {
    leds.at(1).blink(); //use this for demo to simulate code telling motors to go up
    //drivers.at(0).moveUp(); //commented out for demo 
    //drivers.at(1).moveUp();
   
  } else if (command.equals("DOWN")) {
    leds.at(2).blink(); //use this for demo to simulate code telling motors to go down
    //drivers.at(0).moveDown(); //commented out for demo 
    //drivers.at(1).moveDown();
  } else if (command.startsWith("TIMER_")) {
    int duration = command.substring(6).toInt();
    if (duration > 0) {
      xQueueSend(durationQueue, &duration, portMAX_DELAY);  // Send duration to timer task
      sendConfirmation("Started timer for " + String(duration) + " seconds");
    } else {
      sendConfirmation("Invalid timer duration");
    }
  } else if (command.startsWith("TIMERTEST")) {
    unsigned long currentTime = 0;
    if (xQueueReceive(timeRequestQueue, &currentTime, 0) == pdPASS) {
      sendConfirmation("Timer Status: " + String(currentTime) + " seconds elapsed");
    } else {
      sendConfirmation("Timer not running or no data available.");
    }
  } else if (command.startsWith("SAVE_")) {
    //SAVE_NightMode_1_0_1_128_60
    int underscores[5];
    int lastPos = 5;  // Start after "SAVE_"
    bool formatError = false;

    // Find positions of underscores
    for (int i = 0; i < 5; i++) {
      underscores[i] = command.indexOf('_', lastPos);
      if (underscores[i] == -1) {
        formatError = true;
        break;
      }
      lastPos = underscores[i] + 1;
    }

    if (formatError) {
      sendConfirmation("ERROR: Incorrect SAVE format. Use SAVE_name_LED0_LED1_LED2_Brightness_Timer");
      return;
    }

    // Extract values safely
    String profileName = command.substring(5, underscores[0]);
    int led0 = command.substring(underscores[0] + 1, underscores[1]).toInt();
    int led1 = command.substring(underscores[1] + 1, underscores[2]).toInt();
    int led2 = command.substring(underscores[2] + 1, underscores[3]).toInt();
    int brightness = command.substring(underscores[3] + 1, underscores[4]).toInt();
    int timerDuration = command.substring(underscores[4] + 1).toInt();

    // Validate LED values (should be 0 or 1)
    if ((led0 != 0 && led0 != 1) || (led1 != 0 && led1 != 1) || (led2 != 0 && led2 != 1)) {
      sendConfirmation("ERROR: LED values must be 0 (OFF) or 1 (ON).");
      return;
    }

    // Validate brightness (0-255)
    if (brightness < 0 || brightness > 255) {
      sendConfirmation("ERROR: Brightness must be between 0 and 255.");
      return;
    }

    // Validate timer (must be positive)
    if (timerDuration <= 0) {
      sendConfirmation("ERROR: Timer duration must be greater than 0.");
      return;
    }

    // Save profile if everything is valid
    saveProfile(profileName, led0, led1, led2, brightness, timerDuration);
    sendConfirmation("Saved profile: " + profileName);
  } else if (command.startsWith("LOAD_")) {
    String profileName = command.substring(5);
    loadProfile(profileName);
    sendConfirmation("Loaded profile: " + profileName);
  } else if (command.startsWith("APP_")) {
    // New command format: APP_LED_LED_LED_BRIGHTNESS_TIMER
    int underscores[4];
    int lastPos = 4;  // Start after "APP_"
    bool formatError = false;

    // Find positions of underscores
    for (int i = 0; i < 4; i++) {
      underscores[i] = command.indexOf('_', lastPos);
      if (underscores[i] == -1) {
        formatError = true;
        break;
      }
      lastPos = underscores[i] + 1;
    }

    if (formatError) {
      sendConfirmation("ERROR: Incorrect APP format. Use APP_LED_LED_LED_BRIGHTNESS_TIMER");
      return;
    }

    // Extract values safely
    int led0 = command.substring(4, underscores[0]).toInt();
    int led1 = command.substring(underscores[0] + 1, underscores[1]).toInt();
    int led2 = command.substring(underscores[1] + 1, underscores[2]).toInt();
    int brightness = command.substring(underscores[2] + 1, underscores[3]).toInt();
    int timerDuration = command.substring(underscores[3] + 1).toInt();

    // Validate LED values (should be 0 or 1)
    if ((led0 != 0 && led0 != 1) || (led1 != 0 && led1 != 1) || (led2 != 0 && led2 != 1)) {
      sendConfirmation("ERROR: LED values must be 0 (OFF) or 1 (ON).");
      return;
    }

    // Validate brightness (0-255)
    if (brightness < 0 || brightness > 255) {
      sendConfirmation("ERROR: Brightness must be between 0 and 255.");
      return;
    }

    // Validate timer (must be positive)
    if (timerDuration <= 0) {
      sendConfirmation("ERROR: Timer duration must be greater than 0.");
      return;
    }

    // Apply the values
    leds.at(0).light(led0);
    leds.at(1).light(led1);
    leds.at(2).light(led2);
    leds.at(0).changeBrightness(brightness);
    leds.at(1).changeBrightness(brightness);
    leds.at(2).changeBrightness(brightness);
    xQueueSend(durationQueue, &timerDuration, portMAX_DELAY);  // Send duration to timer task

    sendConfirmation("Set LEDs: " + String(led0) + ", " + String(led1) + ", " + String(led2) + " | Brightness: " + String(brightness) + " | Timer: " + String(timerDuration) + " seconds");
  } else {
    sendConfirmation("Invalid command received");
  }

}

void saveProfile(String profileName, int led0, int led1, int led2, int brightness, int timerDuration) {
  preferences.begin("profiles", false);  // Open storage with namespace "profiles"

  preferences.putInt((profileName + "_led0").c_str(), led0);
  preferences.putInt((profileName + "_led1").c_str(), led1);
  preferences.putInt((profileName + "_led2").c_str(), led2);
  preferences.putInt((profileName + "_brightness").c_str(), brightness);
  preferences.putInt((profileName + "_timer").c_str(), timerDuration);

  preferences.end();  // Close storage
  Serial.println("Profile saved: " + profileName);
}

void loadProfile(String profileName) {
  preferences.begin("profiles", true);  // Read mode

  int led0 = preferences.getInt((profileName + "_led0").c_str(), 0);
  int led1 = preferences.getInt((profileName + "_led1").c_str(), 0);
  int led2 = preferences.getInt((profileName + "_led2").c_str(), 0);
  int brightness = preferences.getInt((profileName + "_brightness").c_str(), 255);
  int timerDuration = preferences.getInt((profileName + "_timer").c_str(), 30);

  preferences.end();

  // Apply the loaded values (simulates loading default pressure values)
  leds.at(0).light(led0);
  leds.at(1).light(led1);
  leds.at(2).light(led2);
  leds.at(0).changeBrightness(brightness);
  leds.at(1).changeBrightness(brightness);
  leds.at(2).changeBrightness(brightness);
  timeoutClock.stopwatch(timerDuration);

  Serial.println("Profile loaded: " + profileName);
}


void shutdownTimerTask(void* param) {
  int shutdownTime = *(int*)param;
  vTaskDelay(pdMS_TO_TICKS(shutdownTime * 1000));  // Wait for the timeout

  if (!deviceConnected) {
    Serial.println("Device disconnected too long. Shutting down...");
    esp_deep_sleep_start();  // Put ESP32 into deep sleep mode
  }

  vTaskDelete(NULL);  // End the task
}
// Callback to process received BLE messages
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    // DOIT esp32
      String value = pCharacteristic->getValue();
    // nano esp32
    //string value = pCharacteristic->getValue();
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
  unsigned long receivedDuration = timerDuration;  // Default value
  while (1) {
    if (xQueueReceive(durationQueue, &receivedDuration, portMAX_DELAY) == pdPASS) {
      timeoutClock.stopwatch(receivedDuration);  // Run timer for received duration
      leds.at(0).light(0);//turns off lights after timer is up
      leds.at(1).light(0);
      leds.at(2).light(0);
    }
  }
}
/*
  Watches the value of from the timer
*/
void timeWatcher(void* params) {
  while (1) {
    unsigned long currentTime;
    // receives time while the stopwatch function is counting.
    xQueueReceiveFromISR(timeQueue, &currentTime, &xTaskWokenByReceive);
    // Sends time to any time requesting functions
    xQueueSendFromISR(timeRequestQueue, &currentTime, &xHigherPriorityTaskWoken);
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Server...");
  /*
    Watchdog task timer is disable becuase by design, the time functions
    are supposed to take a certain amount of time.
    This will prevent triggering wdt in for these cases.
    A yellow light on the esp32 will indicate that wdt is turned off
  */
  esp_task_wdt_deinit();
  // Creating a queue that only holds the current time.
  timeQueue = xQueueCreate(1, sizeof(int));
  timeRequestQueue = xQueueCreate(1, sizeof(int));
  // Initialize LEDs

  

#ifndef TESTING

/*
    Commented out due to using multiple esp32s with different pins
  */

 Serial.println("Starting BLE Server...");
 leds.at(0).pin = PIN_LED0;
 leds.at(1).pin = PIN_LED1;
 leds.at(2).pin = PIN_LED2;
 pinMode(leds.at(0).pin, OUTPUT);
 pinMode(leds.at(1).pin, OUTPUT);
 pinMode(leds.at(2).pin, OUTPUT);

// Initialize Steppers


// drivers.at(0).createStepper(9,5,6,13);
// drivers.at(0).createStepper(9,5,6,13);
// drivers.at(0).setSpeed(10);
// int step = 17;
// int dir = 10;
// int enable = 6;
// int extra = 13;

// 1st Motor Pins

// int step = 9;
// int dir = 8;
// int enable = 7;
// int extra = 13;

//  2nd motor pins

// int step = 6;
// int dir = 9;
// int enable = 8;
// int extra = 13;
#endif


#ifndef TESTING
  // Initialize BLE
  BLEDevice::init("Migraine Mitigator V1");
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


#endif
    durationQueue = xQueueCreate(1, sizeof(unsigned long)); // Queue for timer duration
    Serial.println("Migraine Mitigator started. Waiting for connections...");
    

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
// int getLedPin(int ledIndex) {
//   switch (ledIndex) {
//     case 0: return PIN_LED0;
//     case 1: return PIN_LED1;
//     case 2: return PIN_LED2;
//     default: return -1;
//   }
// }
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
