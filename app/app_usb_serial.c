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

	uint8_t val;
	uint8_t buffer[] = "Hello world!\n";

	while(1){

		CDC_Transmit_FS(buffer, sizeof(buffer));
		osDelay(100);
			/*
			if (USB_CDC_RX[0] == 's') {

				HAL_GPIO_TogglePin(GPIOD, LED4_Pin);

				val = Launch_MotorTest();
				//val = atoi(&USB_CDC_RX[4]);

				//itoa(115, USB_CDC_TX, 10);

				if (val == 0)
					strcpy(USB_CDC_TX, "Motor test launched\n\r");
				else
					strcpy(USB_CDC_TX, "Motor test canceled\n\r");

				CDC_Transmit_FS(USB_CDC_TX, strlen(USB_CDC_TX));

				USB_CDC_RX[0] = 0;
			}
			*/


	}
}
