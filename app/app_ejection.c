#include "app_ejection.h"

#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"
#include "app_buzzer.h"

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

    while (1) {

		sense_main = !HAL_GPIO_ReadPin(SENSE_MAIN_GPIO_Port, SENSE_MAIN_Pin);
		sense_drogue = !HAL_GPIO_ReadPin(SENSE_DROGUE_GPIO_Port, SENSE_DROGUE_Pin);

		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, sense_main);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, sense_drogue);

		if (sense_main && sense_drogue){
			setBuzzerMode(BUZZER_TRIPLETICK);
		} else if (sense_main) {
			setBuzzerMode(BUZZER_DOUBLETICK);
		} else if (sense_drogue) {
			setBuzzerMode(BUZZER_SINGLETICK);
		} else {
			setBuzzerMode(BUZZER_IDLE);
		}

		osDelay(1000);
    }
}

void EjectDrogue(){

	sense_drogue = !HAL_GPIO_ReadPin(SENSE_DROGUE_GPIO_Port, SENSE_DROGUE_Pin);

	while (sense_drogue){
		HAL_GPIO_WritePin(FIRE_DROGUE_GPIO_Port, FIRE_DROGUE_Pin, GPIO_PIN_SET);
		osDelay(EJECTION_PULSE_MS);
		HAL_GPIO_WritePin(FIRE_DROGUE_GPIO_Port, FIRE_DROGUE_Pin, GPIO_PIN_RESET);
		osDelay(EJECTION_PULSE_MS);
	}

}

void EjectMain(){

	sense_main = !HAL_GPIO_ReadPin(SENSE_MAIN_GPIO_Port, SENSE_MAIN_Pin);

	while (sense_drogue){
		HAL_GPIO_WritePin(FIRE_MAIN_GPIO_Port, FIRE_MAIN_Pin, GPIO_PIN_SET);
		osDelay(EJECTION_PULSE_MS);
		HAL_GPIO_WritePin(FIRE_MAIN_GPIO_Port, FIRE_MAIN_Pin, GPIO_PIN_RESET);
		osDelay(EJECTION_PULSE_MS);
	}

}
