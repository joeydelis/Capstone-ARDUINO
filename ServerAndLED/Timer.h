#include <thread>
#define SECOND 1000

/*
* Timer will count down or up time depending on twhich function is used
*/




struct Timer {

int* time;


void stopwatch (int duration) {
  for (int i =0;i<duration;i++){
    delay(SECOND);
    time = &i;
    Serial.println(i);
  }
}




    
};
