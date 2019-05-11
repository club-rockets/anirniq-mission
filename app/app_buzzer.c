#include "app_buzzer.h"

#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"

void StartBuzzerTask();

osThreadId buzzerTaskHandle;
int iBuzzerMode = 0;

void setBuzzerMode(int _imode){
	iBuzzerMode = _imode;
}

void buzzerTask_init()
{

	/* definition and creation of buzzerTask */
	  osThreadDef(buzzerTask, StartBuzzerTask, osPriorityLow, 1, 64);
	  buzzerTaskHandle = osThreadCreate(osThread(buzzerTask), NULL);

}

void StartBuzzerTask()
{

    while (1) {

    	switch(iBuzzerMode) {

    	   case BUZZER_IDLE  :
    		   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    		   osDelay(CYCLE_TIME);
    	       break;

    	   case BUZZER_SINGLETICK  :
    		   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    		   osDelay(CYCLE_TIME);
    		   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
			    osDelay(BEEP_LENGTH);
				break;

    	   case BUZZER_DOUBLETICK  :
    		   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    		   osDelay(CYCLE_TIME);
    		   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
				osDelay(BEEP_LENGTH);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
				osDelay(BEEP_LENGTH);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
				osDelay(BEEP_LENGTH);
				break;

    	   case BUZZER_TRIPLETICK  :
    		   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
			   osDelay(CYCLE_TIME);
			   HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
				osDelay(BEEP_LENGTH);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
				osDelay(BEEP_LENGTH);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
				osDelay(BEEP_LENGTH);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
				osDelay(BEEP_LENGTH);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
				osDelay(BEEP_LENGTH);
				break;

    	   /* you can have any number of case statements */
    	   default : /* Optional */
    		   iBuzzerMode = 0;
    	}

    }
}