/*
 * app_buzzer.h
 *
 *  Created on: May 5, 2019
 *      Author: Hugo Lemoine
 */

#ifndef APP_APP_BUZZER_H_
#define APP_APP_BUZZER_H_

//Buzzer
#define BUZZER_IDLE 0
#define BUZZER_SINGLETICK 1
#define BUZZER_DOUBLETICK 2
#define BUZZER_TRIPLETICK 3

#define CYCLE_TIME 1000
#define BEEP_LENGTH 50

void buzzerTask_init();
void setBuzzerMode(int _imode);


#endif /* APP_APP_BUZZER_H_ */
