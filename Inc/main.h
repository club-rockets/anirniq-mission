/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "enums.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define CAN_BOARD MISSION
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define CAN_BOARD MISSION
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SENSE_DROGUE_Pin GPIO_PIN_2
#define SENSE_DROGUE_GPIO_Port GPIOE
#define FIRE_DROGUE_Pin GPIO_PIN_3
#define FIRE_DROGUE_GPIO_Port GPIOE
#define FIRE_MAIN_Pin GPIO_PIN_4
#define FIRE_MAIN_GPIO_Port GPIOE
#define SENSE_MAIN_Pin GPIO_PIN_5
#define SENSE_MAIN_GPIO_Port GPIOE
#define BUZZER_Pin GPIO_PIN_3
#define BUZZER_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_11
#define LED4_GPIO_Port GPIOB
#define BARO_CS_Pin GPIO_PIN_12
#define BARO_CS_GPIO_Port GPIOB
#define SD_DETECT_Pin GPIO_PIN_3
#define SD_DETECT_GPIO_Port GPIOD
#define SD_DETECT_EXTI_IRQn EXTI3_IRQn
#define CAN1_STANDBY_Pin GPIO_PIN_0
#define CAN1_STANDBY_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

#define EJECTION_PRIORITY osPriorityRealtime
#define ALTITUDE_PRIORITY osPriorityHigh
#define BUZZER_PRIORITY osPriorityLow
#define SDCARD_PRIORITY osPriorityNormal
#define HEARTHBEAT_PRIORITY osPriorityHigh
#define USBSERIAL_PRIORITY osPriorityNormal


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
