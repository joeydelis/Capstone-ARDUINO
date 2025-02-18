/*
* Timer will count down or up time depending on twhich function is used
*/
#define SECOND 1000


 

/*
  wait 
  takes the disired time in milliseconds to wait and lopp until the time was been met or greater.
*/
void wait(unsigned long waitTime){
  unsigned long initialTime =millis();
  while(millis()-initialTime <= waitTime) // replaces delay so other functions can run 
      {
      }
}

/*
  Struct Timer 
  used for having different timers if needed.
*/
struct Timer {

  int timing = 0; // will be used to check if the timer stopwatch function is used.
  int* time; // points to the 

/*
  stopwatch
  counts up to a time (duration) in seconds
*/
  void stopwatch (int duration) {
    for (int i =0;i<duration;i++){
      wait((unsigned long) SECOND);
      time = &i;
      Serial.println(i);
    }
  }
   
};