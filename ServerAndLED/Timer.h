#include "esp_task_wdt.h"
#include "esp_err.h"


/*
* Timer will count down or up time depending on twhich function is used
*/
#define SECOND 1000
QueueHandle_t queue;
static esp_task_wdt_user_handle_t wait_twdt_user_hdl;

/*
  wait 
  takes the desired time in milliseconds to wait and lopp until the time was been met or greater.
*/
void wait(unsigned long waitTime){
  unsigned long initialTime =millis();
  // esp_task_wdt_add(NULL);
  while(millis()-initialTime < waitTime) // replaces delay so other functions can run 
      {
       
       
      }
}

/*
  Struct Timer 
  used for having different timers if needed.
*/
struct Timer {

  int timing = 0; // will be used to check if the timer stopwatch function is used.
  int time=0; // points to the 

/*
  stopwatch
  counts up to a time (duration) in seconds
*/
  void stopwatch (int duration) {
    for (int i =0;i<duration;i++){
      xQueueSend(queue, &time, portMAX_DELAY);
      wait((unsigned long) SECOND);
      time = i;
      // Serial.println(i);
    }
    timing =0;
  }
   
};