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
  Configuration of the custom task watchdog timer
*/
esp_task_wdt_config_t twdt_config = {
  .timeout_ms= WDT_TIMEOUT,
  .idle_core_mask = (1 << CONFIG_FREERTOS_NUMBER_OF_CORES) -1, // bitmask idle core to be monitored
  .trigger_panic = false // whether it reboots or not.
};
 

/*
  Monitor 
  Check the values of each connected device to make sure it's operating at the POWERLIMIT
  Corrects the power if it is over the POWERLIMIT to RECOVER
*/
void monitor(std::vector<Device>){


};