/*
 * HCSR04_Handler.c
 *
 *  Created on: Dec 27, 2020
 *      Author: chris
 */

#include "main.h"

TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim4;

t_sonic_sensor Rear, Front, Right, Left;

/*
 *
 */
void HCSR06_Capture_Handler(TIM_HandleTypeDef *htim)
{
	switch(htim->Channel){
	case HAL_TIM_ACTIVE_CHANNEL_1:
		HCSR06_channel_Handler(&Rear, htim, TIM_CHANNEL_1);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_2:
		HCSR06_channel_Handler(&Front, htim, TIM_CHANNEL_2);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_3:
		HCSR06_channel_Handler(&Right, htim, TIM_CHANNEL_3);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_4:
		HCSR06_channel_Handler(&Left, htim, TIM_CHANNEL_4);
		break;
	default:
		break;
	}
}


/*
 *
 */
void HCSR06_channel_Handler(t_sonic_sensor *sensor, TIM_HandleTypeDef *htim, uint32_t Channel){

	if (sensor->Is_First_Captured==0) // if the first value is not captured
	{
		sensor->IC_Val1 = HAL_TIM_ReadCapturedValue(htim, Channel); // read the first value
		sensor->Is_First_Captured = 1;  // set the first captured as true
		// Now change the polarity to falling edge
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, Channel, TIM_INPUTCHANNELPOLARITY_FALLING);
	}
	else if (sensor->Is_First_Captured==1)   // if the first is already captured
	{
		sensor->IC_Val2 = HAL_TIM_ReadCapturedValue(htim, Channel);  // read second value
		__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
		if (sensor->IC_Val2 > sensor->IC_Val1)
		{
			sensor->Difference = sensor->IC_Val2-sensor->IC_Val1;
		}
		else if (sensor->IC_Val1 > sensor->IC_Val2)
		{
			sensor->Difference = (0xffff - sensor->IC_Val1) + sensor->IC_Val2;
		}
		sensor->Distance = sensor->Difference * .034/2;
		sensor->Is_First_Captured = 0; // set it back to false
		// set polarity to rising edge
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, Channel, TIM_INPUTCHANNELPOLARITY_RISING);
		switch(Channel){
		case TIM_CHANNEL_1:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
			break;
		case TIM_CHANNEL_2:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
			break;
		case TIM_CHANNEL_3:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
			break;
		case TIM_CHANNEL_4:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
			break;
		}
	}
}

/*
 *
 */
int get_Distance_Data(uint8_t sensor){
//
	switch(sensor){
	case REAR_SENSOR:
		return get_sensor_dist(&Rear);
		break;
	case FRONT_SENSOR:
		return get_sensor_dist(&Front);
		break;
	case FRONT_L_SENSOR:
		return get_sensor_dist(&Left);
		break;
	case FRONT_R_SENSOR:
		return get_sensor_dist(&Right);
		break;
	}
return 0;
}

int get_sensor_dist(t_sonic_sensor *sensor){
	return sensor->Distance;
}

void Initiate_Sensor_Timers(){
//
	// init 10us timer
	__HAL_TIM_SET_COUNTER(&htim10, 0);
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);

	// init capture compare timers
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);
}


void disable_10us_Timer(){
	HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);
}




//	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
//	{
//		if (Is_First_Captured==0) // if the first value is not captured
//		{
//			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
//			Is_First_Captured = 1;  // set the first captured as true
//			// Now change the polarity to falling edge
//			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
//		}
//
//		else if (Is_First_Captured==1)   // if the first is already captured
//		{
//			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
//			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
//
//			if (IC_Val2 > IC_Val1)
//			{
//				Difference = IC_Val2-IC_Val1;
//			}
//
//			else if (IC_Val1 > IC_Val2)
//			{
//				Difference = (0xffff - IC_Val1) + IC_Val2;
//			}
//
//			Distance = Difference * .034/2;
//			Is_First_Captured = 0; // set it back to false
//
//			// set polarity to rising edge
//			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
//			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
//		}
//	}



