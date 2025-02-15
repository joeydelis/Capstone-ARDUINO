/*
  Device will hold the characteristics for each device
  This will be usefu to keep track of each pin values for safety monitor and reporting back to the app.
*/
#define FREQUENCY  1000
struct Device{
  // holds up time
  int busy =0;
     int pin;
};
struct Motor : Device{
    int Power =0; // power and brightness is the same thing but kept differently here for now
};
struct LED : Device{
    int brightness =0;


// LED Control Functions
void blink() {
    digitalWrite(pin, HIGH);
    delay(FREQUENCY);
    digitalWrite(pin, LOW);
    delay(FREQUENCY);
}

void light(int state) {
  if(state==0){
     digitalWrite(pin, LOW);
  }else{
    digitalWrite(pin, HIGH);

  }
}


void changeBrightness(int level) {

  if (brightness <level){

    for (; brightness <= level; brightness++) {
        analogWrite(pin, brightness);
        delay(150);
    }
  } else if (brightness > level){

    for (; brightness >= level; brightness--) {
        analogWrite(pin, brightness);
        delay(150);
    }
  }
}

};
