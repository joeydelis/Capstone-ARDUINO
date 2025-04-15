#include "aunit/TestRunner.h"
#include "aunit/AssertMacros.h"
using namespace aunit;


/*
  These tests will the functions created in Device.h. 
  Timer.h wait function will be tested.

  testing using Wokwi will be done to test motor functionaliy
  Manual testing will be done to test bluetooth functionality
*/

/*
  Setup for testing the testcases for each esp32 board will look like this class
*/
class NonBLETestsDOIT : public TestOnce {
  public:
  Preferences preferences;
protected:
  std::vector<LED> leds;
  Driver stepperMotor;
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
    leds.at(0).changeBrightness(led0);
    leds.at(1).changeBrightness(led1);
    leds.at(2).changeBrightness(led2);
    // leds.at(0).changeBrightness(brightness);
    // leds.at(1).changeBrightness(brightness);
    // leds.at(2).changeBrightness(brightness);
    // timeoutClock.stopwatch(timerDuration);

    Serial.println("Profile loaded: " + profileName);
  }
  void setup() override {
    TestOnce::setup();
    Serial.begin(115200);

    // Creating a queue that only holds the current time.
    // timeQueue = xQueueCreate(1, sizeof(int));
    // timeRequestQueue = xQueueCreate(1, sizeof(int));
    // Initialize LEDs


    using std::vector;
    leds.resize(3);
    leds.at(0).pin = 27;
    leds.at(1).pin = 26;
    leds.at(2).pin = 25;
    pinMode(leds.at(0).pin, OUTPUT);
    pinMode(leds.at(1).pin, OUTPUT);
    pinMode(leds.at(2).pin, OUTPUT);
  }
};

// LED tests
testF(NonBLETestsDOIT, LEDOn) {

  leds.at(0).light(1);
  assertEqual(leds.at(0).power, ON);
  leds.at(1).light(1);
  assertEqual(leds.at(1).power, ON);
  leds.at(2).light(1);
  assertEqual(leds.at(2).power, ON);
}
testF(NonBLETestsDOIT, LEDOff) {

  leds.at(0).light(1);
  leds.at(1).light(1);
  leds.at(2).light(1);


  leds.at(0).light(0);
  leds.at(1).light(0);
  leds.at(2).light(0);
  assertEqual(leds.at(0).power, OFF);
  assertEqual(leds.at(1).power, OFF);
  assertEqual(leds.at(2).power, OFF);
}
testF(NonBLETestsDOIT, LEDBrightnessUp) {
  int brightness = 50;
  leds.at(0).changeBrightness(brightness);
  assertEqual(leds.at(0).power, 50);
  leds.at(1).changeBrightness(brightness);
  assertEqual(leds.at(1).power, 50);
  leds.at(2).changeBrightness(brightness);
  assertEqual(leds.at(2).power, 50);
}
testF(NonBLETestsDOIT, LEDBrightnessDown) {
  int brightness = 30;
  leds.at(0).light(1);
  leds.at(1).light(1);
  leds.at(2).light(1);

  leds.at(0).changeBrightness(brightness);
  leds.at(1).changeBrightness(brightness);
  leds.at(2).changeBrightness(brightness);
  assertEqual(leds.at(0).power, 30);
  assertEqual(leds.at(1).power, 30);
  assertEqual(leds.at(2).power, 30);
}
// //Wait test
testF(NonBLETestsDOIT, waittest1) {
  int timeToWait = 3;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}
testF(NonBLETestsDOIT, waitTest2) {
  int timeToWait = 5;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}
testF(NonBLETestsDOIT, waitTest3) {
  int timeToWait = 10;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}
testF(NonBLETestsDOIT, waitTest4) {
  int timeToWait = 1000;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}
testF(NonBLETestsDOIT, waitTest5) {
  int timeToWait = 1000 * 5;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}

// Preferences test
testF(NonBLETestsDOIT, prefTest1) {
  leds.at(0).changeBrightnessFast(50);
  leds.at(1).changeBrightnessFast(51);
  leds.at(2).changeBrightnessFast(52);
  int power0 = leds.at(0).power;
  int power1 = leds.at(1).power;
  int power2 = leds.at(2).power;
  saveProfile("test1", power0, power1, power2, 100, 30);
  leds.at(0).light(0);
  leds.at(1).light(0);
  leds.at(2).light(0);
  loadProfile("test1");

  assertEqual(leds.at(0).power, power0);
  assertEqual(leds.at(1).power, power1);
  assertEqual(leds.at(2).power, power2);
}

testF(NonBLETestsDOIT, prefTest2) {
  leds.at(0).changeBrightnessFast(75);
  leds.at(1).changeBrightnessFast(90);
  leds.at(2).changeBrightnessFast(22);
  int power0 = leds.at(0).power;
  int power1 = leds.at(1).power;
  int power2 = leds.at(2).power;
  saveProfile("test1", power0, power1, power2, 100, 30);
  leds.at(0).light(0);
  leds.at(1).light(0);
  leds.at(2).light(0);
  loadProfile("test1");

  assertEqual(leds.at(0).power, power0);
  assertEqual(leds.at(1).power, power1);
  assertEqual(leds.at(2).power, power2);
}
testF(NonBLETestsDOIT, prefTest3) {
  leds.at(0).changeBrightnessFast(120);
  leds.at(1).changeBrightnessFast(16);
  leds.at(2).changeBrightnessFast(73);
  int power0 = leds.at(0).power;
  int power1 = leds.at(1).power;
  int power2 = leds.at(2).power;
  saveProfile("test1", power0, power1, power2, 100, 30);
  leds.at(0).light(0);
  leds.at(1).light(0);
  leds.at(2).light(0);
  loadProfile("test1");

  assertEqual(leds.at(0).power, power0);
  assertEqual(leds.at(1).power, power1);
  assertEqual(leds.at(2).power, power2);
}
/*
  clear preferences test
*/
testF(NonBLETestsDOIT, prefTest4) {
  leds.at(0).changeBrightnessFast(33);
  leds.at(1).changeBrightnessFast(25);
  leds.at(2).changeBrightnessFast(12);
  int power0 = leds.at(0).power;
  int power1 = leds.at(1).power;
  int power2 = leds.at(2).power;
  saveProfile("test1", power0, power1, power2, 100, 30);
  leds.at(0).light(0);
  leds.at(1).light(0);
  leds.at(2).light(0);
  preferences.clear();

  assertEqual(leds.at(0).power, 0);
  assertEqual(leds.at(1).power, 0);
  assertEqual(leds.at(2).power, 0);
}

testF(NonBLETestsDOIT, prefTest5) {
  leds.at(0).changeBrightnessFast(100);
  leds.at(1).changeBrightnessFast(50);
  leds.at(2).changeBrightnessFast(70);
  int power0 = leds.at(0).power;
  int power1 = leds.at(1).power;
  int power2 = leds.at(2).power;
  saveProfile("test1", power0, power1, power2, 100, 30);
  leds.at(0).light(0);
  leds.at(1).light(0);
  leds.at(2).light(0);
  loadProfile("test1");

  assertEqual(leds.at(0).power, power0);
  assertEqual(leds.at(1).power, power1);
  assertEqual(leds.at(2).power, power2);
}
// Wokwi

// */
class NonBLETestsWokwi : public TestOnce {
protected:
  std::vector<LED> leds;
  Driver stepperMotor;
  void setup() override {
    TestOnce::setup();
    Serial.begin(115200);

    leds.resize(3);
    leds.at(0).pin = 11;
    leds.at(1).pin = 12;
    leds.at(2).pin = 13;

    stepperMotor;
    stepperMotor.createStepper(0, 45, 47, 48);
  }
};
// LED tests
testF(NonBLETestsWokwi, LEDOn) {


  leds.at(0).light(1);

  assertEqual(leds.at(0).power, ON);
  leds.at(1).light(1);
  assertEqual(leds.at(1).power, ON);
  leds.at(2).light(1);
  assertEqual(leds.at(2).power, ON);
}
testF(NonBLETestsWokwi, LEDOff) {

  leds.at(0).light(1);
  leds.at(1).light(1);
  leds.at(2).light(1);


  leds.at(0).light(0);
  leds.at(1).light(0);
  leds.at(2).light(0);
  assertEqual(leds.at(0).power, OFF);
  assertEqual(leds.at(1).power, OFF);
  assertEqual(leds.at(2).power, OFF);
}
testF(NonBLETestsWokwi, LEDBrightnessUp) {
  int brightness = 50;
  leds.at(0).changeBrightness(brightness);
  assertEqual(leds.at(0).power, 50);
  leds.at(1).changeBrightness(brightness);
  assertEqual(leds.at(1).power, 50);
  leds.at(2).changeBrightness(brightness);
  assertEqual(leds.at(2).power, 50);
}
testF(NonBLETestsWokwi, LEDBrightnessDown) {
  int brightness = 30;
  leds.at(0).light(1);
  leds.at(1).light(1);
  leds.at(2).light(1);

  leds.at(0).changeBrightness(brightness);
  leds.at(1).changeBrightness(brightness);
  leds.at(2).changeBrightness(brightness);
  assertEqual(leds.at(0).power, 30);
  assertEqual(leds.at(1).power, 30);
  assertEqual(leds.at(2).power, 30);
}
// //Wait test
testF(NonBLETestsWokwi, waitWokwi1) {
  int timeToWait = 3;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}
testF(NonBLETestsWokwi, waitWokwi2) {
  int timeToWait = 5;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}
testF(NonBLETestsWokwi, waitWokwi3) {
  int timeToWait = 10;
  unsigned long start = millis();
  wait(timeToWait);
  assertLessOrEqual(millis() - start, (unsigned long)timeToWait);
}



class NonBLETestsNano : public TestOnce {

  void setup() override {
    TestOnce::setup();
  }
};