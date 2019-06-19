#include "app_altitude.h"

#include <stdio.h>

#include "cmsis_os.h"
#include "main.h"
#include "app_sd.h"
#include "app_ejection.h"
#include "barometer/barometer.h"
#include "kalman/kalman.h"
#include "usbd_cdc_if.h"
#include "enums.h"

#include "bsp_can.h"
#include "id.h"
#include "enums.h"

//static char buffer[64];

void app_altitude();
int Apogee_Detection(rocketdata_t *_rocket_data);
void Rocket_Data_Init(rocketdata_t *_rocketdata);
void Rocket_Data_Calibrate(rocketdata_t *_rocketdata);
void Rocket_Data_Update(rocketdata_t *_rocketdata);

osThreadDef(altitude, app_altitude, ALTITUDE_PRIORITY, 1, 512);

void app_altitude_init()
{
    osThreadCreate(osThread(altitude), NULL);
}

void Rocket_Data_Init(rocketdata_t *_rocketdata)
{
	barometer_init(&(_rocketdata->barometer), BARO_CS_GPIO_Port, BARO_CS_Pin, &hspi2);
	kalman_init(&(_rocketdata->kalman));

	_rocketdata->ground_altitude = 0;
	_rocketdata->agl_altitude = 0;
	_rocketdata->acceleration = 0;
	_rocketdata->velocity = 0;

	_rocketdata->last_update = 0;
	_rocketdata->calibrated = 0;
}

// Set the MSL altitude
void Rocket_Data_Calibrate(rocketdata_t *_rocketdata){

	float err = 0.0;

	while (!_rocketdata->calibrated) {

//		sprintf(buffer, "CALIBRATION\n");
//		CDC_Transmit_FS(buffer, sizeof(buffer));

		osDelay(CALIBRATION_DELAY_MS);
		Rocket_Data_Update(_rocketdata);
		_rocketdata->ground_altitude = _rocketdata->kalman.altitude;
		osDelay(CALIBRATION_DELAY_MS);
		Rocket_Data_Update(_rocketdata);

		//Small check to be sure this is the real ground altitude.
		err = abs(_rocketdata->ground_altitude - _rocketdata->kalman.altitude);
		if (err < 3.0) {
			_rocketdata->calibrated = 1;
		}
	}

}

// Get new altitude, velocity and acceleration from the barometer and kalman filter
void Rocket_Data_Update(rocketdata_t *_rocketdata){
	/* Read barometer */
	barometer_update(&(_rocketdata->barometer));

	/* Kalman filtering */
	kalman_update(&(_rocketdata->kalman), pressure_to_altitude(_rocketdata->barometer.pressure), 0, (HAL_GetTick() - _rocketdata->last_update) / 1000.0f);
	_rocketdata->last_update = HAL_GetTick();

	_rocketdata->agl_altitude = _rocketdata->kalman.altitude - _rocketdata->ground_altitude;
	_rocketdata->acceleration = _rocketdata->kalman.acceleration;
	_rocketdata->velocity = _rocketdata->kalman.velocity;
}

// Calculate apogee detection
int Apogee_Detection(rocketdata_t *_rocket_data) {

	int Apogee_Estimation = 0;
	//estimation de l'apogee
		  //n,est valide qu'en coasting ascent
		  // s(apogee) = s0 - 0.5 * v^2 / a
	Apogee_Estimation = _rocket_data->agl_altitude - 0.5 * (_rocket_data->velocity) * (_rocket_data->velocity) / _rocket_data->acceleration;

  //essaie pour contr� le sonic boom sans le delais
  //estime l'altitude de l'apogee et si la difference est trop grande
  //on annule le projet tu suite
  if (_rocket_data->agl_altitude - Apogee_Estimation < 500) {
    //test supplementaire, l'acceleration se doit d'etre negative...
    if (_rocket_data->acceleration < 0) {
      //lorsque la vitesse est negative, cest l'apogee
      if (_rocket_data->velocity < 0) {
        //Apogee_Altitude = _kalman.altitude;
        return 1;
      }
    }
  }

  return 0;
}

void app_altitude()
{
    uint32_t launch_tick = 0;
    rocketdata_t rocketdata;
    rocket_state myRocketState = INITIALISATION;
    rocket_state previous_myRocketState = INITIALISATION;

    Rocket_Data_Init(&rocketdata);

    can_regData_u regData = {0};
    can_regData_u rxRegData = {0};

    while (1) {

    	Rocket_Data_Update(&rocketdata);

    	previous_myRocketState = myRocketState;

    	can_getRegisterData(COMMUNICATION,CAN_COMMUNICATION_STATUS_INDEX,&rxRegData);

        switch (myRocketState) {

            case INITIALISATION:

            	Rocket_Data_Calibrate(&rocketdata);
            	myRocketState = STANDBY_ON_PAD;


                break;

            case STANDBY_ON_PAD:
                //on detecte le lancement avec l'acceleration
                // safety: si l'altitude est assez grande on skip...
//                if (rocketdata.acceleration > LAUNCH_ACCEL_TRIGGER || (rocketdata.agl_altitude > FLIGHT_ALTITUDE_TRIGGER)) {
                if (rocketdata.agl_altitude > FLIGHT_ALTITUDE_TRIGGER && rxRegData.UINT32_T == LAUNCH_TRANSMISSION) {
                    launch_tick = HAL_GetTick();
                	myRocketState = POWERED_ASCENT;
                }

                break;

            case POWERED_ASCENT:

            	// Engine in burnout when acceleration below 0
                if ((HAL_GetTick() - launch_tick) > ULTRASONIC_DELAY_MS && rocketdata.acceleration < 0) {
                	myRocketState = COASTING_ASCENT;
                }

                break;

            case COASTING_ASCENT:

                if (Apogee_Detection(&rocketdata) == 1) {
                	EjectDrogue();
                	myRocketState = DROGUE_DESCENT;
                }

                break;

            case DROGUE_DESCENT:
                //if main ejection altitude is detected
                //starts 2sec after drogue deployment
                //if (rocket_data.agl_altitude < MAIN_EJECTION_ALTITUDE && temp_rocket->Mission_Time > temp_rocket->Altimeter->Apogee_Time + 2000 + APOGEE_EJECTION_DELAY_MS) {
				if (rocketdata.agl_altitude < MAIN_EJECTION_ALTITUDE) {
					EjectMain();
                	myRocketState = MAIN_DESCENT;
                }

                break;

            case MAIN_DESCENT:
                //lorsque la fusee atteint le meme threshold que pour le flight mode
                if (rocketdata.agl_altitude < FLIGHT_ALTITUDE_TRIGGER) {
                	myRocketState = LANDING;
                }
                break;

            case LANDING:
                //lorsque la vitesse n'est plus negative,
                if (rocketdata.velocity > 0) {
                	myRocketState = RECOVERY;
                }
                break;

            case RECOVERY:

                break;

            default:
            	myRocketState = INITIALISATION;
                break;
            }

        if (previous_myRocketState != myRocketState){
        	regData.UINT32_T = myRocketState;
        	can_canSetRegisterData(CAN_MISSION_ROCKET_STATUS_INDEX,&regData);
        }

//		sprintf(buffer, "State: %u \t Alt: %.2f \t Spd: %.2f \t Acc: %.2f\n", myRocketState, rocketdata.agl_altitude, rocketdata.velocity, rocketdata.acceleration);
//		CDC_Transmit_FS(buffer, sizeof(buffer));

		//app_sd_write(buffer);

        osDelay(APP_ALTITUDE_CYCLE_MS);
    }
}
