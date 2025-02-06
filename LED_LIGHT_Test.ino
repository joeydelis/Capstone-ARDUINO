#define PIN_LED0 27
#define PIN_LED1 26
#define PIN_LED2 25

int frequency = 1000;



void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED0, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  
}

void loop() {
 
  // int r = random(3);
  // switch(r){
  //   case 0:
  //     blink(PIN_LED0);
  //   break;
  //   case 1:
  //     blink(PIN_LED1);
  //   break;
  //   case 2:
  //     blink(PIN_LED2);
  //   break;
  // }
  blink(PIN_LED0);
  blink(PIN_LED1);
  blink(PIN_LED2);
  // light(PIN_LED0);
  // light(PIN_LED1);
  // light(PIN_LED2);
  
  
}

void blink(int LED){
  // Serial.println("Activating LED in PIN "+ LED);
  digitalWrite(LED, HIGH);
  delay(frequency);
  // Serial.println("Disabling LED in PIN "+ LED);
  digitalWrite(LED, LOW);
  delay(frequency);
  

}

void light(int LED){
digitalWrite(LED, HIGH);

}
