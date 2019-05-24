/*
 * app_ejection.h
 *
 *  Created on: May 5, 2019
 *      Author: Hugo Lemoine
 */

#ifndef APP_APP_EJECTION_H_
#define APP_APP_EJECTION_H_

#define EJECTION_PULSE_MS 50
#define TRY_NUMBER 10

void ejectionTask_init();
void EjectDrogue();
void EjectMain();


#endif /* APP_APP_EJECTION_H_ */
