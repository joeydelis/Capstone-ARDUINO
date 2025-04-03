
#include "esp_task_wdt.h"
#include  <Stepper.h>
#include <ESP_FlexyStepper.h>

/*
  Device will hold the characteristics for each device
  This will be usefu to keep track of each pin values for safety monitor and reporting back to the app.
*/

#define PAUSE  50
#define ON 150
#define OFF 0

// analogWrite is used instead of digital write to tue more control of the power given to the device

const float speedLimit = 10;
const float accelerationLimit =4;

/*
  Device
  used for keeping track of which GPIO pin used and what kind of device is using it.
*/

struct Device{
  // State state = ready;
  int power =0; 
  
};
struct Driver : Device{
  private:
  Stepper* motorDriver;
  // stepper motor driver pins used with the esp32
  // currently assuming 3 pins are needed
  int enablePin=0;
  int dirPin=0;
  int stepPin=0;
  // unused pin for stepper function;
  int fourthPin=48;
  // steps needed to complete a revolution
  int revolutions= 360;
  int speed=0;
  // the pace in which the motor will move at each step
  int step=5;
  public: 

  void createStepper(int enable,int dir, int step, int last){
    this->enablePin = enable;
    this->dirPin = dir;
    this->stepPin = step;
    this->fourthPin = last;
    Stepper newMotorDriver(this->revolutions, this->enablePin,this->dirPin, this->stepPin, this->fourthPin );
    this->motorDriver =(Stepper*) calloc(1,sizeof(newMotorDriver));
    *this->motorDriver = newMotorDriver;
  }

  void setSpeed(int stepSpeed){
  this->speed = stepSpeed;
  this->motorDriver->setSpeed(this->speed);
  }

  int getSpeed(){
    return this->speed;
  }
  int getStep(){
    return this->step;
  }
  void setStep(int newStep){
    this->step = newStep;
    this->motorDriver->step(this->step);
  }
  void emergencyStop(){
     this->speed = 0;
    this->motorDriver->step(this->speed);
  }



};
struct FlexyDriver : Device
{
private:
  ESP_FlexyStepper motorDriver;
  
  float speed;
  float acceleration;
  float deceleration;
public:
  void createStepper(int motorStepPin,int motorDirectionPin, int emergencyStopPin,int limitSwitchPin){
    
    this->motorDriver.connectToPins(motorStepPin, motorDirectionPin, false);
  } 
  //steps per second
  void setSpeed(float speed){
    if(speed < speedLimit){
    this->speed = speed;

    }
  }
  float getSpeed(){
    return this->speed;
  }
  void setAccelereation(float acceleration){
    if(acceleration < accelerationLimit){
      this->acceleration = acceleration;
    }
  }
  float getAcceleration(){
    return this->acceleration;
  }
  void startMotor(){
    this->motorDriver.startAsService();
  }


};
struct LED : Device{
int pin;

// LED Control Functions

/*
  blink
  blink LED from on to off
*/
void blink() {
  //blinks twice
  int blinkAmount = 2;
  for (int i =0; i<blinkAmount; i++){
    analogWrite(pin, ON);
    wait((unsigned long)PAUSE);
    analogWrite(pin, OFF);
    wait((unsigned long)PAUSE);
  }

}
/*
  light 
  turns the LED on (1) and off (not 1).
*/
void light(int state) {
  if(state==1){
    power = ON;
    analogWrite(pin, ON);
    // digitalWrite(pin, LOW);
  }else{
    power = OFF;
    analogWrite(pin, OFF);
  }
}
/*
  changeBrightness
  changes the current led brightness to the desired level.
*/
void changeBrightness(int level) {
  if (power <level){ 
    while (power != level) { // increase brightness gradually until desired level is met
      analogWrite(pin, power);
      wait((unsigned long)5); // wait is used instead of delay because it allows other operations to happen when this is running
      power++;
      
    }
  } else if (power > level){
    while (power != level) { // decrease brightness gradually untill desired level is met
      analogWrite(pin, power);
      wait((unsigned long) 5);
      power--;
    }
  }
}

};


