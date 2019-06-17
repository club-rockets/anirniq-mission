#include "app_ejection.h"

#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"
#include "app_buzzer.h"

#include "bsp_can.h"
#include "can/can_driver.h"
#include "bsp_can_regdef.h"

void StartEjectionTask();

osThreadId ejectionTaskHandle;
GPIO_PinState sense_main = GPIO_PIN_RESET;
GPIO_PinState sense_drogue = GPIO_PIN_RESET;

osThreadDef(ejectionTask, StartEjectionTask, EJECTION_PRIORITY, 1, 64);

void ejectionTask_init()
{

	/* definition and creation of buzzerTask */
	  ejectionTaskHandle = osThreadCreate(osThread(ejectionTask), NULL);

}

void StartEjectionTask()
{
	can_regData_u data = {.UINT32_T = NO_CHARGE};

    while (1) {

		sense_main = !HAL_GPIO_ReadPin(SENSE_MAIN_GPIO_Port, SENSE_MAIN_Pin);
		sense_drogue = !HAL_GPIO_ReadPin(SENSE_DROGUE_GPIO_Port, SENSE_DROGUE_Pin);

		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, sense_main);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, sense_drogue);

	
		data.UINT32_T = (sense_drogue) | (sense_main << 1);
		setBuzzerMode(data.UINT32_T);

		can_canSetRegisterData(CAN_MISSION_CHARGE_STATUS_INDEX, &data);

		osDelay(1000);
    }
}

void EjectDrogue(){

	int itrycount = 0;

	sense_drogue = !HAL_GPIO_ReadPin(SENSE_DROGUE_GPIO_Port, SENSE_DROGUE_Pin);

	while (sense_drogue && (itrycount < TRY_NUMBER)){
		HAL_GPIO_WritePin(FIRE_DROGUE_GPIO_Port, FIRE_DROGUE_Pin, GPIO_PIN_SET);
		osDelay(EJECTION_PULSE_MS);
		HAL_GPIO_WritePin(FIRE_DROGUE_GPIO_Port, FIRE_DROGUE_Pin, GPIO_PIN_RESET);
		osDelay(EJECTION_PULSE_MS);
		itrycount++;
	}
}

void EjectMain(){

	int itrycount = 0;

	sense_main = !HAL_GPIO_ReadPin(SENSE_MAIN_GPIO_Port, SENSE_MAIN_Pin);

	while (sense_main && (itrycount < TRY_NUMBER)){
		HAL_GPIO_WritePin(FIRE_MAIN_GPIO_Port, FIRE_MAIN_Pin, GPIO_PIN_SET);
		osDelay(EJECTION_PULSE_MS);
		HAL_GPIO_WritePin(FIRE_MAIN_GPIO_Port, FIRE_MAIN_Pin, GPIO_PIN_RESET);
		osDelay(EJECTION_PULSE_MS);
		itrycount++;
	}

}
