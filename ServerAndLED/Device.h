#include "esp_task_wdt.h"
/*
  Device will hold the characteristics for each device
  This will be usefu to keep track of each pin values for safety monitor and reporting back to the app.
*/

#define PAUSE  1000
static esp_task_wdt_user_handle_t changeBrightness_twdt_user_hdl;
// analogWrite is used instead of digital write to tue more control of the power given to the device



/*
  struct Device
  used for keeping track of which GPIO pin used and what kind of device is using it.
*/

struct Device{
  int busy =0;
  int power =0; // power and brightness is the same thing but kept differently here for now
  int pin;
};
struct Motor : Device{
};
struct LED : Device{
  int brightness =0;

// LED Control Functions

/*
  blink
  blink LED from on to off
*/
void blink() {
  
 
    analogWrite(pin, 100);
    wait((unsigned long)PAUSE);
    analogWrite(pin, 0);
    wait((unsigned long)PAUSE);
    
    analogWrite(pin, 100);
    wait((unsigned long)PAUSE);
    analogWrite(pin, 0);
    wait((unsigned long)PAUSE);
    
}
/*
  light 
  turns the LED on (1) and off (not 1).
*/
void light(int state) {
  
  if(state==1){
    brightness = 150;
    analogWrite(pin, 150);
    // digitalWrite(pin, LOW);
  }else{
    brightness = 0;
    analogWrite(pin, 0);

  }
}
/*
  changeBrightness
  changes the current led brightness to the desired level.
*/
void changeBrightness(int level) {

  
  if (brightness <level){ 

    for (; brightness <= level; brightness++) { // increase brightness gradually until desired level is met
      analogWrite(pin, brightness);
      // delay by one so that watchdog does not trigger
      delay(1); // tried adding the function the task and resetting the wdt but could not get that method to work

      wait((unsigned long)149); // wait is used instead of delay because it allows other operations to happen when this is running
      
     
      
    }
  } else if (brightness > level){

    for (; brightness >= level; brightness--) { // decrease brightness gradually untill desired level is met
     
      analogWrite(pin, brightness);
      delay(1);
      wait((unsigned long) 149);
       
    }
  }
   
}

};


