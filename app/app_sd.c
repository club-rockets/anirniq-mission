#include "app_sd.h"

#include "cmsis_os.h"
#include "main.h"
#include "fatfs.h"
#include "ff.h"

#include <stdio.h>

void app_sd();
osThreadDef(sd, app_sd, osPriorityHigh, 1, 2048);

static osMessageQId detect_events;
static FATFS sdfs;
static FIL logfile;
static char sd_path[4];

void app_sd_init()
{
    osMessageQDef(sdqueue, 10, uint16_t);
    detect_events = osMessageCreate(osMessageQ(sdqueue), NULL);

    osThreadCreate(osThread(sd), NULL);
}

void app_sd()
{
    osEvent event;

    while (1) {
        event = osMessageGet(detect_events, osWaitForever);

        if (event.status == osEventMessage) {
            switch(event.value.v) {
                case CARD_CONNECTED:
                    if (BSP_SD_Init() == MSD_OK) {
                        if (f_mount(&sdfs, SDPath, 1) != FR_OK) {
                            break;
                        }

                        if (f_open(&logfile, "mission.log", FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) {
                            break;
                        }

                        f_puts("sd write success\r\n", &logfile);
                        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                        f_close(&logfile);
                        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
                    }
                    break;
                case CARD_DISCONNECTED:
                    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                    break;

                default:
                    break;
            }
        }
    }
}

void app_sd_detect_handler()
{
    if (!HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port, SD_DETECT_Pin)) {
        osMessagePut(detect_events, CARD_CONNECTED, osWaitForever);
    }
    else {
        osMessagePut(detect_events, CARD_DISCONNECTED, osWaitForever);
    }
}