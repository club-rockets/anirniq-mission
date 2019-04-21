#include "app_altitude.h"

#include <stdio.h>

#include "cmsis_os.h"
#include "main.h"
#include "app_sd.h"
#include "barometer/barometer.h"
#include "kalman/kalman.h"

static char buffer[64];

void app_altitude();
osThreadDef(altitude, app_altitude, osPriorityNormal, 1, 512);

void app_altitude_init()
{
    osThreadCreate(osThread(altitude), NULL);
}

void app_altitude()
{
    barometer_t barometer;
    barometer_init(&barometer, BARO_CS_GPIO_Port, BARO_CS_Pin, &hspi2);

    kalman_t kalman;
    kalman_init(&kalman);

    uint32_t last_update = 0;
    uint32_t last_save = 0;

    while (1) {
        /* Read barometer */
        barometer_update(&barometer);

        /* Kalman filtering */
        kalman_update(&kalman, pressure_to_altitude(barometer.pressure), 0, (HAL_GetTick() - last_update) / 1000.0f);
        last_update = HAL_GetTick();

        if (HAL_GetTick() > last_save + 20) {
            sprintf(buffer, "baro,%u,%.2f, %.2f\n",  HAL_GetTick(), barometer.pressure, barometer.temperature);
            app_sd_write(buffer);
            sprintf(buffer, "kalman,%u,%.2f,%.2f,%.2f\n", HAL_GetTick(), kalman.altitude, kalman.velocity, kalman.acceleration);
            app_sd_write(buffer);
            last_save = HAL_GetTick();
        }
        HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    }
}