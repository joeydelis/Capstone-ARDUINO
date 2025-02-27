#include "sdkconfig.h"
#include "esp_task_wdt.h"
#include <queue>
#define WDT_TIMEOUT 60000 // 60 seconds for timeout
#define POWERLIMIT 150
#define RECOVER 0
/*
* The role of the Governor is to make sure that the device stays within 
* safe operation.
* 
*/

/*
  Signals re used to tell the timer or device what state it should be in.
*/
enum Signals {
  start, 
  stop,
  restart
};



/*
  Monitor 
  Check the values of each connected device to make sure it's operating at the POWERLIMIT
  Corrects the power if it is over the POWERLIMIT to RECOVER
*/
void monitor(std::vector<Device>){


};