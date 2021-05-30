/*
 * Scheduler.c
 *
 *  Created on: Oct 14, 2020
 *      Author: chris
 */

#include "main.h"


RawData_Def Accel_Raw_Data, Gyro_Raw_Data;
//Pixy2_Data_CCC Signature_1;

int Intervine_Scheduler =0;
int Sched_Speed =0;
int Sched_Angle =90;

int counter =0;
int counter2 =0;
int sonic_cnt =0;
int led_cnt =0;
int flag2 =0;
int Distance[4] = {0};
int temp_Dist1 =0;
int temp_Dist2 =0;

//Runs at 20ms 50Hz
void scheduler(void){
//
	/*Scheduler Alive LED*/
	if(led_cnt++ > LED_COUNT){
		led_cnt =0;
		HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
	}
	/*##################################*/

	/* Ultra-Sonic sensor section*/
	if(sonic_cnt++ > HCSR04_COUNT){
		sonic_cnt =0;
		Initiate_Sensor_Timers();
		Distance[0] = get_Distance_Data(REAR_SENSOR);
		Distance[1] = get_Distance_Data(FRONT_SENSOR);
		Distance[2] = get_Distance_Data(FRONT_L_SENSOR);
		Distance[3] = get_Distance_Data(FRONT_R_SENSOR);

		if(Distance[0]< 30){
			Set_RGB_LED(0, 0, 255);
		}
		else if(Distance[1]< 30){
			Set_RGB_LED(0, 255, 0);
		}
		else if(Distance[2]< 30){
			Set_RGB_LED(255, 0, 0);
		}
		else if(Distance[3]< 30){
			Set_RGB_LED(255, 255, 255);
		}
		else{
			Set_RGB_LED(0, 0, 0);
		}
		/*
		if(temp_Dist1 < 15 || temp_Dist2 < 15 ) {
			//Intervine_Scheduler = 1;
			Sched_Speed = 0;
			Sched_Angle = 90;
			HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
		}
		//else Intervine_Scheduler = 0;
	*/
	}
	/*##################################*/

/*
 * if UART available
 * get UART data
 * decide on action
 */




	/*##################################*/
/*
 * Pixy_Cam data analysis section
 * 	if Pixy enabled
 * 	get pixy data
 * 	decide on action
 * 	(change target values)
 */
	/*
	if(counter2++ > SPI_COUNT){
		if(flag2){
			Set_RGB_LED(0, 255, 255);
			flag2 = 0;
		}
		else {
			Set_RGB_LED(255, 0, 0);
			flag2 = 1;
		}
		counter2 =0;
	}
	*/
	//Get_Color_Connected_Components(1,1, &Signature_1);



	/*##################################*/
/*
 * Motion sensor data analysis section
 * if sensor enabled
 * get sensor data
 * decide on action
 */
	MPU6050_Get_Accel_RawData(&Accel_Raw_Data);
	//MPU6050_Get_Gyro_RawData(&Gyro_Raw_Data);
	//Accel_Raw_Data -= Accel_Raw_Cali;
	if( abs(Accel_Raw_Data.y) > 1500 ){
		if(counter++ > I2C_COUNT){
			HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
		counter =0;
		}
		//Intervine_Scheduler = 1;
		//Sched_Speed = 100;
		//Sched_Angle = 90;
	}
	//else Intervine_Scheduler =0;

	/*##################################*/

/*
 * motor control section
 * if target values interrupted
 * get new target values for both right & lrft motor
 * linearly change PWM signal for output at 20ms rate!
 * if( current_speed - target_speed > 0) target speed --
 *
 * target speed +1
 */


	/*##################################*/

	// Stop PWM logic for 10us Pulse
	disable_10us_Timer();
}

int Scheduler_Halt_Status(){
	return SCHEDULER_UNHALT;
}

int Scheduler_Control(){
	return Intervine_Scheduler;
}

int get_Scheduler_Speed(){
	return Sched_Speed;
}

int get_Scheduler_Angle(){
	return Sched_Angle;
}


