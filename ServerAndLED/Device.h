#include "esp_task_wdt.h"
/*
  Device will hold the characteristics for each device
  This will be usefu to keep track of each pin values for safety monitor and reporting back to the app.
*/

#define PAUSE  50
#define ON 150
#define OFF 0
static esp_task_wdt_user_handle_t changeBrightness_twdt_user_hdl;
// analogWrite is used instead of digital write to tue more control of the power given to the device



/*
  Device
  used for keeping track of which GPIO pin used and what kind of device is using it.
*/

struct Device{
  int busy =0;
  int power =0; 
  int pin;
};
struct Motor : Device{
  int positionX;
  int positionY;
  int rotation;
};
struct LED : Device{
 

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
    while (power <= level) { // increase brightness gradually until desired level is met
      analogWrite(pin, power);
      wait((unsigned long)50); // wait is used instead of delay because it allows other operations to happen when this is running
      power++;
      
    }
  } else if (power > level){
    while (power >= level) { // decrease brightness gradually untill desired level is met
      analogWrite(pin, power);
      wait((unsigned long) 50);
      power--;
    }
  }
}

};

