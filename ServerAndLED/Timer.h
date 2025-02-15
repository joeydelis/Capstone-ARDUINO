#include <chrono>
#include <time.h>
#define SECOND 1000; 

/*
* Timer will count down or up time depending on twhich function is used
*/

// Design not final

struct Timer {
    void (*fptr)(); // pointer to a function so that if the time is up the timer will stop the function.
    // time var for duration
};