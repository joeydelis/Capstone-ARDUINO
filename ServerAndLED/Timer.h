#include "esp_task_wdt.h"
#include "esp_err.h"


/*
* Timer will count down or up time depending on twhich function is used
*/
#define SECOND 1000
#define TIMEOUTCLOCK 60*30 
// Time
QueueHandle_t timeQueue;
static esp_task_wdt_user_handle_t wait_twdt_user_hdl;

enum TimerState {
  Run, 
  Stop,
  Restart
};

/*
  wait 
  takes the desired time in milliseconds to wait and lopp until the time was been met or greater.
*/
void wait(unsigned long waitTime){
  unsigned long initialTime =millis();
  unsigned long currentTime =0;
  while(currentTime < waitTime) // replaces delay so other functions can run 
  {
    if( initialTime> millis()){ // handles the overflow case of millis
    // millis overflow after 49 days, so it's not expected the device to be on that long 
    // but just in case.
    initialTime = millis();
    currentTime = currentTime + (millis() - initialTime);
    } else{
    currentTime = millis()-initialTime;
    
    }
  }
}
/*
  Timer 
  used for having different timers if needed.
*/
struct Timer {
  int timing = 0; // will be used to check if the timer stopwatch function is used.
  unsigned long time=0; // points to the 
/*
  stopwatch
  counts to a time (duration) in seconds
*/
  void stopwatch (int duration) {
    for (unsigned long i =0;i<=duration;i++){
      xQueueSend(timeQueue, &time, portMAX_DELAY); // sends the time to the task queue so that another task can report the time.
      wait((unsigned long) SECOND);
      time = i;
    }
    timing =0;
  }
};