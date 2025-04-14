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
class NonBLETestsDOIT: public TestOnce {
   protected:
    std::vector<LED> leds;
    Driver stepperMotor;
  void setup() override{
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
testF(NonBLETestsDOIT, LEDOn){
 
   leds.at(0).light(1);
   assertEqual(leds.at(0).power, ON);
   leds.at(1).light(1);
   assertEqual(leds.at(1).power, ON);
   leds.at(2).light(1);
   assertEqual(leds.at(2).power, ON);
}
testF(NonBLETestsDOIT, LEDOff){
 
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
testF(NonBLETestsDOIT, LEDBrightnessUp){
  int brightness =50;
   leds.at(0).changeBrightness(brightness);
   assertEqual(leds.at(0).power, 50);
   leds.at(1).changeBrightness(brightness);
   assertEqual(leds.at(1).power, 50);
   leds.at(2).changeBrightness(brightness);
   assertEqual(leds.at(2).power, 50);
}
testF(NonBLETestsDOIT, LEDBrightnessDown){
  int brightness =30;
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
testF(NonBLETestsDOIT, waittest1){
  int timeToWait = 3;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}
testF(NonBLETestsDOIT, waitTest2){
  int timeToWait = 5;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}
testF(NonBLETestsDOIT, waitTest3){
  int timeToWait = 10;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}
testF(NonBLETestsDOIT, waitTest4){
  int timeToWait = 1000;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}
testF(NonBLETestsDOIT, waitTest5){
  int timeToWait = 1000 *5;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}
//Motor tests


Wokwi

*/
class NonBLETestsWokwi: public TestOnce {
  protected:
   std::vector<LED> leds;
  Driver stepperMotor;
  void setup() override{
    TestOnce::setup();
    Serial.begin(115200);

    leds.resize(3);
    leds.at(0).pin = 11;
    leds.at(1).pin = 12;
    leds.at(2).pin = 13;

    stepperMotor;
    stepperMotor.createStepper(0,45,47,48);


  }
};
// LED tests
testF(NonBLETestsWokwi, LEDOn){
 

   leds.at(0).light(1);
   
   assertEqual(leds.at(0).power, ON);
   leds.at(1).light(1);
   assertEqual(leds.at(1).power, ON);
   leds.at(2).light(1);
   assertEqual(leds.at(2).power, ON);
}
testF(NonBLETestsWokwi, LEDOff){
 
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
testF(NonBLETestsWokwi, LEDBrightnessUp){
  int brightness =50;
   leds.at(0).changeBrightness(brightness);
   assertEqual(leds.at(0).power, 50);
   leds.at(1).changeBrightness(brightness);
   assertEqual(leds.at(1).power, 50);
   leds.at(2).changeBrightness(brightness);
   assertEqual(leds.at(2).power, 50);
}
testF(NonBLETestsWokwi, LEDBrightnessDown){
  int brightness =30;
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
testF(NonBLETestsWokwi, waitWokwi1){
  int timeToWait = 3;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}
testF(NonBLETestsWokwi, waitWokwi2){
  int timeToWait = 5;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}
testF(NonBLETestsWokwi, waitWokwi3){
  int timeToWait = 10;
  unsigned long start= millis();
  wait(timeToWait);
  assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
}



class NonBLETestsNano: public TestOnce {

  void setup() override{
    TestOnce::setup();


  }
};