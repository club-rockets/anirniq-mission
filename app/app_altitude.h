#ifndef APP_ALTITUDE_H_
#define APP_ALTITUDE_H_

#include "kalman/kalman.h"
#include "barometer/barometer.h"

/******************************************************************************/
/*                              Type  Prototype                               */
/******************************************************************************/
typedef struct {

	barometer_t barometer;
	kalman_t kalman;

	float ground_altitude; // mean sea level altitude
	float agl_altitude;    // above ground level altitude
	float velocity;
	float acceleration;

	uint32_t last_update;
	uint32_t calibrated;
} rocketdata_t;

/******************************************************************************/
/*                                Define                                      */
/******************************************************************************/
#define APP_ALTITUDE_CYCLE_MS         20
#define CALIBRATION_DELAY_MS         500
#define APOGEE_EJECTION_DELAY_MS     100 //delais d'ejection apres la detection d'apogee en millisec
#define ULTRASONIC_DELAY_MS         2000 //delais minimum apres le launch avant de pouvoir detection l'apogee en millisec
#define LAUNCH_ACCEL_TRIGGER           5 // valeur proportionnelle a l'accel qui declanche le launch state
#define FLIGHT_ALTITUDE_TRIGGER      50 // altitude apres la detection du launch a laquelle on change d'etat en metre
#define MAIN_EJECTION_ALTITUDE       200 //altitude de deploiement du MAIN chute en metre

void app_altitude_init();

#endif
