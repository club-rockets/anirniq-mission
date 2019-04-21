#ifndef APP_SD_H_
#define APP_SD_H_

#define CARD_CONNECTED    0x00
#define CARD_DISCONNECTED 0x01
#define DATA_BUFFER_FULL  0x02

#define DATA_BUFFER_LENGTH 512

void app_sd_init();
void app_sd_detect_handler();

void app_sd_log(char* message);
void app_sd_write(char* string);

#endif