/*
  Device will hold the characteristics for each device
  This will be usefu to keep track of each pin values for safety monitor and reporting back to the app.
*/

#define PAUSE  1000

// analogWrite is used instead of digital write to tue more control of the power given to the device



/*
  struct Device
  used for keeping track of which GPIO pin used and what kind of device is using it.
*/

struct Device{
  int busy =0;
  int pin;
};
struct Motor : Device{
  int Power =0; // power and brightness is the same thing but kept differently here for now
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
}
/*
  light 
  turns the LED on (1) and off (not 1).
*/
void light(int state) {
  
  if(state==1){
    brightness = 150;
    // digitalWrite(pin, LOW);
    analogWrite(pin, brightness);
  }else{
    brightness = 0;
    analogWrite(pin, brightness);

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
      wait((unsigned long)150); // wait is used instead of delay because it allows other operations to happen when this is running
    }
  } else if (brightness > level){

    for (; brightness >= level; brightness--) { // decrease brightness gradually untill desired level is met
      analogWrite(pin, brightness);
      wait((unsigned long) 150);
    }
  }
}

};


