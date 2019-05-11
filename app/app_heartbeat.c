#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"

void app_heartbeat();
osThreadDef(heartbeat, app_heartbeat, HEARTHBEAT_PRIORITY, 1, 64);


void app_heartbeat_init()
{
    osThreadCreate(osThread(heartbeat), NULL);
}

void app_heartbeat()
{
    while (1) {
        HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
        osDelay(100);
    }
}
