#include "app_sd.h"

#include <stdio.h>
#include <string.h>

#include "cmsis_os.h"
#include "main.h"
#include "fatfs.h"
#include "ff.h"

static FATFS sdfs;
static FIL logfile;
static FIL datafile;

static char data_buffer[DATA_BUFFER_LENGTH];
static size_t data_cursor = 0;

static osMessageQId sd_events;

void app_sd();
osThreadDef(sd, app_sd, SDCARD_PRIORITY, 1, 4096);

void app_sd_init()
{
    osMessageQDef(sdqueue, 10, uint16_t);
    sd_events = osMessageCreate(osMessageQ(sdqueue), NULL);

    osThreadCreate(osThread(sd), NULL);
}

void app_sd()
{
    osEvent event;

    // if (!HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port, SD_DETECT_Pin)) {
    //     osMessagePut(sd_events, CARD_CONNECTED, 0);
    // }

    while (1) {
        event = osMessageGet(sd_events, osWaitForever);

        if (event.status == osEventMessage) {
            switch(event.value.v) {
                case CARD_CONNECTED:
                    if (BSP_SD_Init() == MSD_OK) {
                        if (f_mount(&sdfs, SDPath, 1) != FR_OK) {
                            break;
                        }

                        if (f_open(&logfile, "journal.log", FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) {
                            break;
                        }
                        f_lseek(&logfile, f_size(&logfile));

                        if (f_open(&datafile, "data.csv", FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) {
                            break;
                        }
                        f_lseek(&datafile, f_size(&datafile));

                        f_printf(&logfile, "%u SD Card mounted\n", HAL_GetTick());
                        f_sync(&logfile); 
                        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
                    }
                    break;
                case CARD_DISCONNECTED:
                    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
                    data_cursor = 0;
                    break;

                case DATA_BUFFER_FULL:
                    data_cursor = 0;
                    f_puts(data_buffer, &datafile);
                    f_sync(&datafile);
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
        osMessagePut(sd_events, CARD_CONNECTED, osWaitForever);
    }
    else {
        osMessagePut(sd_events, CARD_DISCONNECTED, osWaitForever);
    }
}

void app_sd_log(char* message)
{
    if (HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port, SD_DETECT_Pin)) {
        return;
    }
}

void app_sd_write(char* string)
{
    if (HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port, SD_DETECT_Pin)) {
        return;
    }

    if (strlen(string) > (DATA_BUFFER_LENGTH - data_cursor)) {
        osMessagePut(sd_events, DATA_BUFFER_FULL, osWaitForever);
    }

    strcpy(data_buffer + data_cursor, string);
    data_cursor += strlen(string);
}
