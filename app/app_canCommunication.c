/*
 * app_canCommunication.c
 *
 *  Created on: 7 juin 2019
 *      Author: Marc-Andre Denis
 */

#include "app_canCommunication.h"

#include "app_ejection.h"
#include "cmsis_os.h"
#include "bsp_can.h"
#include "bsp_can_regdef.h"
#include "enums.h"
#include "id.h"
#include "main.h"


#define SIGNAL_EJECT_DROGUE (1<<0)
#define SIGNAL_EJECT_MAIN (1<<1)


extern osTimerId transmitRegHandle;
extern osThreadId app_canCommunicHandle;

void regChangeCallback(uint32_t board,uint32_t id);

void tsk_canCommunication(void*arg){


	osTimerStart(transmitRegHandle,100);

	can_setRegisterCallback(COMMUNICATION,CAN_COMMUNICATION_CONTROL_EJECT_DROGUE_INDEX,regChangeCallback);
	can_setRegisterCallback(COMMUNICATION,CAN_COMMUNICATION_CONTROL_EJECT_MAIN_INDEX,regChangeCallback);

	osEvent evt;
	can_regData_u regData;
	while(1){
		evt = osSignalWait(SIGNAL_EJECT_DROGUE|SIGNAL_EJECT_MAIN,osWaitForever);
		if(evt.status == osEventSignal && evt.value.signals & SIGNAL_EJECT_DROGUE){
			can_getRegisterData(COMMUNICATION,CAN_COMMUNICATION_CONTROL_EJECT_DROGUE_INDEX,&regData);
			if(regData.UINT32_T == EJECTION){
				EjectDrogue();
				can_setRegisterCallback(COMMUNICATION,CAN_COMMUNICATION_CONTROL_EJECT_DROGUE_INDEX,0);
			}
		}
		if(evt.status == osEventSignal && evt.value.signals & SIGNAL_EJECT_MAIN){
			can_getRegisterData(COMMUNICATION,CAN_COMMUNICATION_CONTROL_EJECT_MAIN_INDEX,&regData);
			if(regData.UINT32_T == EJECTION){
				EjectMain();
				can_setRegisterCallback(COMMUNICATION,CAN_COMMUNICATION_CONTROL_EJECT_MAIN_INDEX,0);
			}
		}
	}
}


void regChangeCallback(uint32_t board,uint32_t id){
	if(board == COMMUNICATION && id == CAN_COMMUNICATION_CONTROL_EJECT_DROGUE_INDEX){
		osSignalSet(app_canCommunicHandle,SIGNAL_EJECT_DROGUE);
	}
	if(board == COMMUNICATION && id == CAN_COMMUNICATION_CONTROL_EJECT_MAIN_INDEX){
		osSignalSet(app_canCommunicHandle,SIGNAL_EJECT_MAIN);
	}
}

void canTransmitRegCallback(void const * argument){
	static uint32_t i = 0;
	can_canSetRegisterData(i,0);
	i = (i+1)%can_registersSize[CAN_BOARD];
}
