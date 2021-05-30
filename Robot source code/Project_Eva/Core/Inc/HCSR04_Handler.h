/*
 * HCSR04_Handler.h
 *
 *  Created on: Dec 27, 2020
 *      Author: chris
 */

#ifndef INC_HCSR04_HANDLER_H_
#define INC_HCSR04_HANDLER_H_

#define REAR_SENSOR 0
#define FRONT_SENSOR 1
#define FRONT_L_SENSOR 2
#define FRONT_R_SENSOR 3

typedef struct{
//
	uint32_t IC_Val1;
	uint32_t IC_Val2;
	uint32_t Difference;
	uint8_t Is_First_Captured;  // is the first value captured ?
	uint8_t Distance;
}t_sonic_sensor;

void HCSR06_Capture_Handler(TIM_HandleTypeDef *htim);
void HCSR06_channel_Handler(t_sonic_sensor *sensor, TIM_HandleTypeDef *htim, uint32_t Channel);
int get_Distance_Data(uint8_t sensor);
int get_sensor_dist(t_sonic_sensor *sensor);
void Initiate_Sensor_Timers();
void disable_10us_Timer();

#endif /* INC_HCSR04_HANDLER_H_ */
