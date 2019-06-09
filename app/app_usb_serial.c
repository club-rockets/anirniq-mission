/*
 * app_usb_serial.c
 *
 *  Created on: May 17, 2019
 *      Author: Hugo Lemoine
 */

#include "app_usb_serial.h"
#include "main.h"
#include "usbd_cdc_if.h"

#include "cmsis_os.h"

void StartUsbserialTask();

osThreadDef(UsbserialTask, StartUsbserialTask, USBSERIAL_PRIORITY, 1, 512);

void app_usb_serial_init()
{
    osThreadCreate(osThread(UsbserialTask), NULL);
}

void StartUsbserialTask(){

	uint8_t buffer[] = "Hello world!\n";

	while(1){

		CDC_Transmit_FS(buffer, sizeof(buffer));
		osDelay(100);

	}
}
