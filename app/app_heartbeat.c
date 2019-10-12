#include "cmsis_os.h"
#include "gpio.h"

#include "app_ejection.h"
#include "main.h"
#include "app_heartbeat.h"

void app_heartbeat();
osThreadDef(heartbeat, app_heartbeat, HEARTHBEAT_PRIORITY, 1, 64);


void app_heartbeat_init()
{
    osThreadCreate(osThread(heartbeat), NULL);
}

void app_heartbeat()
{
//	osDelay(300000);
//	EjectDrogue();
    while (1) {
        HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
        osDelay(100);
    }
}
