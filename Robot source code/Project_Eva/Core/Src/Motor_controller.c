/*
 * Motor_controller.c
 *
 *  Created on: Oct 17, 2020
 *      Author: chris
 */

#include "main.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

uint8_t Threshold = upper_Threshold;
int prev_Throttle =0;

float l = Robot_Length/2;
float Target_speed =0;
float Right_Target_Speed =0;
float Left_Target_Speed =0;
float Right_Motor_Speed =0;
float Left_Motor_Speed =0;
char prev_Right_Wheel_dir = FORWARD;
char prev_Left_Wheel_dir = FORWARD;
char Right_Wheel_dir = FORWARD;
char Left_Wheel_dir = FORWARD;
float H_of_X[181];
int Polarity_Vector[4] = {1, -1, -1, 1};

/* calculates the discrete transformation vector
 * that transforms the user angle into a radius
 * of turning of the robot, this function runs
 * once after the Micro-controller is initialized.
 */
void calculate_Gaussian_Transform(){
//
	float F_of_X[4];
	for(int i=0; i<=90; i++){
		F_of_X[0] = a1*expf(-pow(i-b,2)/(2*pow(c1,2)));
		F_of_X[1] = a2*expf(-pow(i-b,2)/(2*pow(c2,2)));
		F_of_X[2] = a3*expf(-pow(i-b,2)/(2*pow(c3,2)));
		F_of_X[3] = a4*expf(-pow(i-b,2)/(2*pow(c4,2)));
		H_of_X [i] = F_of_X[0] + F_of_X[1] + F_of_X[2] + F_of_X[3];
		H_of_X [180-i] = H_of_X [i];
	}
}

int Get_Halt_Status(){
	int Halt=0;
	Halt += UART_Halt_Status();
	Halt += Scheduler_Halt_Status();
	return Halt;
}

//Runs at 0.5ms 1Khz
void Kinematics_Controler(){
//
	float Speed =0;
	int Joy_Angle =0;

	if(Get_Halt_Status() != HALT){
		if(Scheduler_Control()){
			Speed = get_Scheduler_Speed();
			Joy_Angle = get_Scheduler_Angle();
		}
		else if(Remote_Control() == ENABLED){
			Speed = Get_Gradiant_Trottle();
			Joy_Angle = get_angle();
		}
		else {
			//Speed = Get_SW_Speed();
			//Joy_Angle = Get_SW_Angle();
		}
	}
	else {
		Speed=0;
		Joy_Angle =0;
	}
	int polar = Joy_Angle/90;
	int ang = Joy_Angle%180;
	float Rad = 0;
	float diff =0;

	if(abs(Speed) <= Cutoff_speed) Speed =0;

	if(ang<20 || ang>160){
		Right_Target_Speed = -Speed*pitch_res*Polarity_Vector[polar];
		Left_Target_Speed  =  Speed*pitch_res*Polarity_Vector[polar];
	}
	else {
		if(Joy_Angle >= 180) Speed = -Speed;
		// ################### System Plant #####################
		/* calculate the desired wheel speeds
		 * right = Omega(Raduis-L/2) = V/R(R-L/2) = V(1-L/2R)
		 * Left  = Omega(Raduis+L/2) = V/R(R+L/2) = V(1+L/2R)*/
		Rad = H_of_X[ang]*Polarity_Vector[polar];
		diff = l/Rad;
		Right_Target_Speed = Speed*(1.0 - diff);
		Left_Target_Speed  = Speed*(1.0 + diff);
	}
	//Get_PID_Controller_Correction()
	float R_error = Right_Target_Speed - Right_Motor_Speed;
	float L_error = Left_Target_Speed - Left_Motor_Speed;
	Right_Motor_Speed += R_error*Kp;
	Left_Motor_Speed  += L_error*Kp;

	if(abs(Right_Motor_Speed) < Dir_Trans_Speed){
		Right_Wheel_dir = (Right_Motor_Speed>0)?FORWARD:BACKWARD;
		if(Right_Wheel_dir != prev_Right_Wheel_dir){
			Set_Right_Motor_Direction(Right_Wheel_dir);
			prev_Right_Wheel_dir = Right_Wheel_dir;
		}
	}
	if(abs(Left_Motor_Speed) < Dir_Trans_Speed){
		Left_Wheel_dir = (Left_Motor_Speed>=0)?FORWARD:BACKWARD;
		if(Left_Wheel_dir != prev_Left_Wheel_dir){
			Set_Left_Motor_Direction(Left_Wheel_dir);
			prev_Left_Wheel_dir = Left_Wheel_dir;
		}
	}
	//
	Set_Motor_Speed(RIGHT, abs(round(Right_Motor_Speed)));
	Set_Motor_Speed(LEFT, abs(round(Left_Motor_Speed)));
}

int Get_Gradiant_Trottle(){
//
	int Throttle = get_Throttle();
	//
	if(abs(Throttle - prev_Throttle) > joy_frame){
		if(Throttle > Threshold){
			Threshold = lower_Threshold;
			Target_speed = Throttle;
		}
		else {
			Threshold = upper_Threshold;
			Target_speed = 0;
		}
		prev_Throttle = Throttle;
	}
	//
	if(abs(Target_speed) <= Cutoff_speed) Target_speed =0;
	//
	return Target_speed;
}

void Enable_Motor(char motor){
//
	if(motor == RIGHT){
		HAL_GPIO_WritePin(GPIOA, R_Mot_EN_Pin, 0);
	}
	if(motor == LEFT){
		HAL_GPIO_WritePin(GPIOA, L_Mot_EN_Pin, 0);
	}
} //One is enabled

void Disable_Motor(char motor){
//
	if(motor == RIGHT){
		HAL_GPIO_WritePin(GPIOA, R_Mot_EN_Pin, 1);
	}
	if(motor == LEFT){
		HAL_GPIO_WritePin(GPIOA, L_Mot_EN_Pin, 1);
	}

} //Zero is Disabled

void Set_Right_Motor_Direction(char direction){
//
	if(direction == FORWARD){
		HAL_GPIO_WritePin(GPIOC, R_Mot_Dir_Pin, 1);	//forwards is 0
	}
	if(direction == BACKWARD){
		HAL_GPIO_WritePin(GPIOC, R_Mot_Dir_Pin, 0); //backwards is 1
	}

}

void Set_Left_Motor_Direction(char direction){
//
	if(direction == FORWARD){
		HAL_GPIO_WritePin(GPIOC, L_Mot_Dir_Pin, 0);	//forwards is 1
	}
	if(direction == BACKWARD){
		HAL_GPIO_WritePin(GPIOC, L_Mot_Dir_Pin, 1);	//backwards is 0
	}
}

void Set_Motor_Speed(char motor, uint8_t Speed){
//
	if(motor == RIGHT){
		htim2.Instance -> CCR1= Speed  ;
	}
	if(motor == LEFT){
		htim2.Instance -> CCR2= Speed  ;
	}
}

void Initiate_PWM_Signal(char motor){
//
	if(motor == RIGHT){
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	}
	if(motor == LEFT){
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	}
}

void Shutdown_PWM_Signal(char motor){
//
	if(motor == RIGHT){
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	}
	if(motor == LEFT){
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	}
}



