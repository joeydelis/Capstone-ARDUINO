#include <chrono>
#include <time.h>

#define SECOND 1000; 

struct Timer {
    void (*fptr)();
};