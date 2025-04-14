
#include "esp_task_wdt.h"
#include <Stepper.h>
#include <ESP_FlexyStepper.h>
/*
  Device will hold the characteristics for each device
  This will be usefu to keep track of each pin values for safety monitor and reporting back to the app.
*/

#define PAUSE 50
#define ON 150
#define OFF 0
using std::string;
// analogWrite is used instead of digital write to tue more control of the power given to the device

const float accelerationLimit = 4;
const float decelerationLimit = 4;
const float speedLimit = 10;
const int stepLimit = 5;

/*
  Device
  used for keeping track of which GPIO pin used and what kind of device is using it.
*/

struct Device {
  // State state = ready;
  int power = 0;
};
/*
  Driver uses the Stepper library to control the stepper motor using a certain speed and steps.
*/
struct Driver : Device {
private:
  Stepper* motorDriver;
  // stepper motor driver pins used with the esp32
  // currently assuming 3 pins are needed

  int enablePin = 0;
  int dirPin = 0;
  int stepPin = 0;
  // unused pin for stepper function;
  int fourthPin = 48;
  // steps needed to complete a revolution

  // revolutions is not changable as it is assumed
  // that the motor will use a fixed revolution
  int revolutions = 360;

  // The motor is started when the speed > 0
  int speed = 0;
  // the pace in which the motor will move at each step
  int step = 5;
public:

  /*
    createStepper
    Creates the stepper motor controller using the pins needed by the stepper motor.
    
    
  */
  void createStepper(int enablePin, int dirPin, int stepPin, int lastPin) {
    this->enablePin = enablePin;
    this->dirPin = dirPin;
    this->stepPin = stepPin;
    this->fourthPin = lastPin;
    Stepper newMotorDriver(this->revolutions, this->enablePin, this->dirPin, this->stepPin, this->fourthPin);
    this->motorDriver = (Stepper*)calloc(1, sizeof(newMotorDriver));
    *this->motorDriver = newMotorDriver;
  }

  /*
    createStepper
    Creates the steppper controller for steppers without the need of the step and fourth pin
  */
  void createStepper(int enablePin, int dirPin, int stepPin) {
    this->enablePin = enablePin;
    this->dirPin = dirPin;
    Stepper newMotorDriver(this->revolutions, this->enablePin, this->dirPin);
    this->motorDriver = (Stepper*)calloc(1, sizeof(newMotorDriver));
    *this->motorDriver = newMotorDriver;
  }

  /*
    setSpeed
    Set the step speed of the motor to the new step speed of the motor
    
    The stepSpeed must not exceed speed limit and must be a non-negative integer
  */
  void setSpeed(int stepSpeed) {
    if (stepSpeed < speedLimit && stepSpeed >= 0) {
      this->speed = stepSpeed;
      this->motorDriver->setSpeed(this->speed);
    }
  }
  /*
    getSpeed
    returns the speed of the motor.
  */
  int getSpeed() {
    return this->speed;
  }
  /*
    setStep
    Set the step size to the new step size

    The new step size must be less than the step limit and must be a non-negative integer
  */
  void setStep(int newStep) {
    if (newStep < stepLimit && newStep >= 0) {
      this->step = newStep;
      this->motorDriver->step(this->step);
    }
  }
  /*
    getpStep
    return the step of the motor
  */
  int getStep() {
    return this->step;
  }
  void emergencyStop() {
    this->speed = 0;
    this->motorDriver->step(this->speed);
  }
};
/*
  FlexyDriver is an alternative Driver controling solution that speaks to the motor in the matter of 
  speed, acceleration, and deceleration rather than steps.
*/
struct FlexyDriver : Device {
private:
  ESP_FlexyStepper motorDriver;

  float speed;
  float acceleration;
  float deceleration;
public:
  /*
    createStepper
    Creates the stepper motor controller with the specified pins
  */
  void createStepper(int motorStepPin, int motorDirectionPin, int emergencyStopPin, int limitSwitchPin) {

    this->motorDriver.connectToPins(motorStepPin, motorDirectionPin, false);
  }
  //steps per second
  /*
    setSpeed
    Set the speed of the motor in steps per second
  */
  void setSpeed(float speed) {
    if (speed < speedLimit) {
      this->speed = speed;
    }
  }
  /*
    getSpeed
    returns the speed of the motor.
  */
  float getSpeed() {
    return this->speed;
  }
  /*
    setAcceleration
    Set the acceleration of the motor in steps per second
  */
  void setAccelereation(float acceleration) {
    if (acceleration < accelerationLimit && acceleration > 0) {
      this->acceleration = acceleration;
    }
  }
  /*
    getAcceleration
    returns the acceleration of the motor
  */
  float getAcceleration() {
    return this->acceleration;
  }
  /*
    setDeceleration
    Sets the deceleration of the stepper motor in steps per second
  */
  void setDeceleration(float deceleration) {
    if (deceleration < decelerationLimit && deceleration > 0) {
      this->deceleration = deceleration;
    }
  }
  /*
    getDeceleration
    returns the deceleration of the stepper motor in steps per second
  */
  float getDeceleration() {
    return this->deceleration;
  }
  /*
    startMotor
    Begins the motor starting on a core on the esp32
  */
  void startMotor(int core) {
    this->motorDriver.startAsService(core);
  }
};
struct LED : Device {
  int pin;

  // LED Control Functions

  /*
  blink
  blink LED from on to off
*/
  void blink() {
    //blinks twice
    int blinkAmount = 2;
    for (int i = 0; i < blinkAmount; i++) {
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
    if (state == 1) {
      power = ON;
      analogWrite(pin, ON);
      // digitalWrite(pin, LOW);
    } else {
      power = OFF;
      analogWrite(pin, OFF);
    }
  }
  /*
  changeBrightness
  changes the current led brightness to the desired level.
*/
  void changeBrightness(int level) {
    if (power < level) {
      while (power != level) {  // increase brightness gradually until desired level is met
        analogWrite(pin, power);
        wait((unsigned long)5);  // wait is used instead of delay because it allows other operations to happen when this is running
        power++;
      }
    } else if (power > level) {
      while (power != level) {  // decrease brightness gradually untill desired level is met
        analogWrite(pin, power);
        wait((unsigned long)5);
        power--;
      }
    }
  }
};
