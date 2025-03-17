#line 1 "/home/jroundtree/5911/Capstone-ARDUINO/ServerAndLED/tests/AunitTests.cpp"
#include "aunit/TestRunner.h"
#include "aunit/AssertMacros.h"
#include "NonBLETestsNano.h"
#include "NonBLETestsPhysical.h"
#include "NonBLETestsWokwi.h"
using namespace aunit;




// class NonBLETestsDOIT: public TestOnce {
//    protected:
//   void setup() override{
//     TestOnce::setup();
//     Serial.begin(115200);
//     Serial.println("Starting BLE Server...");
//     esp_task_wdt_deinit();
//     // Creating a queue that only holds the current time.
//     timeQueue = xQueueCreate(1, sizeof(int));
//     timeRequestQueue = xQueueCreate(1, sizeof(int));
//     // Initialize LEDs

//     // leds = {};//(LED*)calloc(sizeof(LED),3); // assigning memory to each LED struct
//     using std::vector;
//     vector<LED> leds(3);
//     leds.at(0).pin = 27;
//     leds.at(1).pin = 26;
//     leds.at(2).pin = 25;
//     pinMode(leds.at(0).pin, OUTPUT);
//     pinMode(leds.at(1).pin, OUTPUT);
//     pinMode(leds.at(2).pin, OUTPUT);

  
    

//   }
//     void teardown() override {
     
//       leds.at(0).light(0);
//       leds.at(0).light(1);
//       leds.at(0).light(2);
      
      

//       TestOnce::teardown();
//     }
//     std::vector<LED> leds;
   
// };

// // LED tests
// testF(NonBLETestsDOIT, LEDOn){
 
//    leds.at(0).light(1);
//    assertEqual(leds.at(0).power, ON);
//    leds.at(1).light(1);
//    assertEqual(leds.at(1).power, ON);
//    leds.at(2).light(1);
//    assertEqual(leds.at(2).power, ON);
// }
// testF(NonBLETestsDOIT, LEDOff){
 
//    leds.at(0).light(1);
//    assertEqual(leds.at(0).power, OFF);
//    leds.at(1).light(1);
//    assertEqual(leds.at(1).power, OFF);
//    leds.at(2).light(1);
//    assertEqual(leds.at(2).power, OFF);
// }
// testF(NonBLETestsDOIT, LEDBrightnessUp){
//   int brightness =50;
//    leds.at(0).changeBrightness(brightness);
//    assertEqual(leds.at(0).power, 50);
//    leds.at(1).changeBrightness(brightness);
//    assertEqual(leds.at(1).power, 50);
//    leds.at(2).changeBrightness(brightness);
//    assertEqual(leds.at(2).power, 50);
// }
// testF(NonBLETestsDOIT, LEDBrightnessDown){
//   int brightness =30;
//    leds.at(0).light(1);
//    leds.at(1).light(1);
//    leds.at(2).light(1);
 
//   leds.at(0).changeBrightness(brightness);
//   leds.at(1).changeBrightness(brightness);
//   leds.at(2).changeBrightness(brightness);
//    assertEqual(leds.at(0).power, 30);
//    assertEqual(leds.at(1).power, 30);
//    assertEqual(leds.at(2).power, 30);
// }
// // //Wait test
// testF(NonBLETestsDOIT, waitWokwi1){
//   int timeToWait = 3;
//   unsigned long start= millis();
//   wait(timeToWait);
//   assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
// }
// testF(NonBLETestsDOIT, waitWokwi2){
//   int timeToWait = 5;
//   unsigned long start= millis();
//   wait(timeToWait);
//   assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
// }
// testF(NonBLETestsDOIT, waitWokwi3){
//   int timeToWait = 10;
//   unsigned long start= millis();
//   wait(timeToWait);
//   assertLessOrEqual(millis()-start,(unsigned long) timeToWait);
// }
/*

Wokwi

*/
class NonBLETestsWokwi: public TestOnce {
  protected:
   std::vector<LED> leds;
  Driver stepperMotor;
  void setup() override{
    TestOnce::setup();
    Serial.begin(115200);
    // Serial.println("Starting BLE Server...");
    // esp_task_wdt_deinit();
    // Creating a queue that only holds the current time.
    // timeQueue = xQueueCreate(1, sizeof(int));
    // timeRequestQueue = xQueueCreate(1, sizeof(int));
    // Initialize LEDs

    // leds = {};//(LED*)calloc(sizeof(LED),3); // assigning memory to each LED struct
    // using std::vector;
    leds.resize(3);
    leds.at(0).pin = 11;
    leds.at(1).pin = 12;
    leds.at(2).pin = 13;
    // pinMode(leds.at(0).pin, OUTPUT);
    // pinMode(leds.at(1).pin, OUTPUT);
    // pinMode(leds.at(2).pin, OUTPUT);
// Serial.println("Past LED");
    stepperMotor;
    stepperMotor.createStepper(0,45,47,48);
    // stepperMotor.setSpeed(0); 
    // stepperMotor.setStep(0);
//     Serial.println("Past motor");

  }
    // void teardown() override {
     
    //   leds.at(0).light(0);
    //   leds.at(1).light(0);
    //   leds.at(2).light(0);
    // //   stepperMotor.setSpeed(0); 
    // //   stepperMotor.setStep(0);
      

    // //   TestOnce::teardown();
    // }
   
    

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
// // Motor Tests
testF(NonBLETestsWokwi, motorClockwiseMotion){
  int rpm = 60;
  int step =5;
  stepperMotor.setSpeed(rpm);
  stepperMotor.setStep(step);
  // Wokwi will check that it is moving clockwize
  while(Serial.available()){
    // waits for wokwi input
  }
  assertEqual((int) Serial.parseInt(), 1);

}
testF(NonBLETestsWokwi, motorCounterClockwiseMotion){
  int rpm = 60;
  int step =5;
  stepperMotor.setSpeed(rpm);
  stepperMotor.setStep(step);
  // Wokwi will check that it is moving clockwize
  while(Serial.available()){
    // waits for wokwi input
  }
  assertEqual((int) Serial.parseInt(), 1);

}



// class NonBLETestsNano: public TestOnce {

//   void setup() override{
//     TestOnce::setup();


//   }
// };