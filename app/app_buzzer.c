#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"

void app_buzzer();
osThreadDef(buzzer, app_buzzer, osPriorityLow, 1, 64);


void app_buzzer_init()
{
    osThreadCreate(osThread(buzzer), NULL);
}

void app_buzzer()
{
    while (1) {
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
        osDelay(1000);
    }
}
