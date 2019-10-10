

#include "app_acc.h"
#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os.h"
#include "spi.h"
#include "gpio.h"

extern SPI_HandleTypeDef hspi1;

void task_acc(void const * argument){

uint8_t txBuff[1] = {0};
uint8_t rxBuff[1] = {0};

txBuff[0] = ( (1<<0) | (0xf<<2) );

	while(1){
		osDelay(1000);

		HAL_GPIO_WritePin(ACC_CS_GPIO_Port,ACC_CS_Pin,GPIO_PIN_RESET);

		osDelay(1);

		HAL_SPI_Transmit(&hspi1,txBuff,1,0);

		HAL_SPI_Receive(&hspi1,rxBuff,1,0);

		HAL_GPIO_WritePin(ACC_CS_GPIO_Port,ACC_CS_Pin,GPIO_PIN_SET);





	}

}

