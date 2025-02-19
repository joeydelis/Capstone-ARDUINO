#include "sdkconfig.h"
#include "esp_task_wdt.h"
#include "Timer.h"
#include "Device.h"
#include <queue>
#define WDT_TIMEOUT 60000 // 60 seconds for timeout

/*
* The role of the Governor is to make sure that the device stays within 
* safe operation.
* 
*/

std::queue<String> cmdQueue;

/*
  Configuration of the custom task watchdog timer
*/
esp_task_wdt_config_t twdt_config = {
  .timeout_ms= WDT_TIMEOUT,
  .idle_core_mask = (1 << CONFIG_FREERTOS_NUMBER_OF_CORES) -1, // bitmask idle core to be monitored
  .trigger_panic = false // whether it reboots or not.
};
 


void monitor(Device device){


};