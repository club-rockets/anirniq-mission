

#include "app_acc.h"
#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os.h"
#include "spi.h"
#include "gpio.h"
#include "app_sd.h"

#define ACC_CR_REG 0x20
#define ACC_OUT_REG 0x28

extern SPI_HandleTypeDef hspi1;

uint8_t txBuff[4] = {0};
uint8_t rxBuff[16] = {0};

void spiTx(uint8_t* data,uint32_t size, uint32_t release){
	HAL_GPIO_WritePin(ACC_CS_GPIO_Port,ACC_CS_Pin,GPIO_PIN_RESET);

	HAL_SPI_Transmit_IT(&hspi1,data,size);

	osSignalWait(ACC_TRANSFER_CPLT_SIGNAL,osWaitForever);

	if(release){
		HAL_GPIO_WritePin(ACC_CS_GPIO_Port,ACC_CS_Pin,GPIO_PIN_SET);
	}
}

void spiRx(uint8_t* data,uint32_t size){
	HAL_GPIO_WritePin(ACC_CS_GPIO_Port,ACC_CS_Pin,GPIO_PIN_RESET);

	HAL_SPI_Receive_IT(&hspi1,data,size);

	osSignalWait(ACC_TRANSFER_CPLT_SIGNAL,osWaitForever);

	HAL_GPIO_WritePin(ACC_CS_GPIO_Port,ACC_CS_Pin,GPIO_PIN_SET);
}


void task_acc(void const * argument){

uint8_t txBuff[6] = {0};
union accData data = {0};

txBuff[0] = ( (1<<7) | (1<<6) | (0x0f<<0) );

	acc_init();

	while(1){
		//50hz
		osDelay(20);

		//read command
		txBuff[0] = ( (1<<7) | (1<<6) | ACC_OUT_REG);
		spiTx(txBuff,1,0);
		//read
		spiRx(data.bytes,sizeof(union accData));

		//todo convert values to m/s^2

		sd_writeInt("acc_x",data.data.x);
		sd_writeInt("acc_y",data.data.y);
		sd_writeInt("acc_z",data.data.z);

	}

}


void acc_init(){
	//write command
	txBuff[0] = ( (1<<6) | ACC_CR_REG);
	//cr1 register value
	//normal power mode
	//data rate 50hz
	//x y z enabled
	txBuff[1] = ( (0b001<<5) | (0b00<<3) | (1<<2) | (1<<1) | (1<<0));
	//cr2 register
	txBuff[2] = 0;
	//cr3 register
	txBuff[3] = 0;
	//cr4 register
	txBuff[4] = ( (1<<7));
	//cr5 register
	txBuff[5] = 0;

	spiTx(txBuff,6,1);
}
