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

	float msl_altitude;// mean sea level altitude
	float agl_altitude;// above ground level altitude
	float velocity;
	float acceleration;

	uint32_t last_update;
	uint32_t calibrated;
} rocketdata_t;

typedef enum rocket_state {
	INITIALISATION,
	STANDBY_ON_PAD,
	LAUNCH,
	POWERED_ASCENT,
	ENGINE_BURNOUT,
	COASTING_ASCENT,
	APOGEE_REACHED,
	DROGUE_DEPLOYMENT,
	DROGUE_DESCENT,
	MAIN_DEPLOYMENT,
	MAIN_DESCENT,
	LANDING,
	RECOVERY,
	PICKEDUP} rocket_state;

/******************************************************************************/
/*                                Define                                      */
/******************************************************************************/
#define APOGEE_EJECTION_DELAY_MS 	2000 //delais d'ejection apres la detection d'apogee en millisec
#define ULTRASONIC_DELAY_MS 		1000 //delais minimum apres le launch avant de pouvoir detection l'apogee en millisec
#define LAUNCH_ACCEL_TRIGGER	 	   5 // valeur proportionnelle a l'accel qui declanche le launch state
#define FLIGHT_ALTITUDE_TRIGGER      100 // altitude apres la detection du launch a laquelle on change d'etat en metre
#define MAIN_EJECTION_ALTITUDE	 	 333 //altitude de deploiement du MAIN chute en metre

void app_altitude_init();

#endif
