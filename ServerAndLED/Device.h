// Device header will contain the
#define FREQUENCY  1000
struct Device{
  // holds up time
  int busy =0;
     int pin;
};
struct Motor : Device{
    int Power =0;
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

void light() {
    digitalWrite(pin, HIGH);
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
