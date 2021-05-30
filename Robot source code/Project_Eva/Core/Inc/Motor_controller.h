/*
 * Motor_controller.h
 *
 *  Created on: Oct 17, 2020
 *      Author: chris
 */

#ifndef INC_MOTOR_CONTROLLER_H_
#define INC_MOTOR_CONTROLLER_H_

#define FORWARD  'F'
#define BACKWARD 'B'
#define RIGHT	 'R'
#define LEFT	 'L'
#define ENABLED  true
#define HALT	 true

#define upper_Threshold 30
#define lower_Threshold 20
#define joy_frame 3

#define Cutoff_speed 18
#define Dir_Trans_Speed 15
#define Robot_Length 44
#define pitch_res 0.75

#define b  90.0
#define c1  4.0
#define a1  3000.0
#define c2  20.0
#define a2  100.0
#define c3  40.0
#define a3  30.0
#define c4  1000.0
#define a4  32.0

/*PID Parameters*/
#define Kp 0.16
//#define Ki
//#define Kd

typedef struct{
//
	uint8_t strength; 	//0-100;
	uint8_t angle;		//0-360;
} t_Control_Vector;

typedef struct{
//
	float targetSpeed;
	float prevSpeed;
	float Speed;
	float error;
	char Direction;
	char prevDirection;
} t_Motor_class;

typedef struct{
//
	#define kp 0.16
	float Error;
	float prev_Error;
} t_PID_class;

int Get_Halt_Status();
void calculate_Gaussian_Transform();
void Kinematics_Controler();
int Get_Gradiant_Trottle();
void Enable_Motor(char motor);
void Disable_Motor(char motor);
void Set_Right_Motor_Direction(char direction);
void Set_Left_Motor_Direction(char direction);
void Set_Motor_Speed(char motor, uint8_t Speed);
void Initiate_PWM_Signal(char motor);
void Shutdown_PWM_Signal(char motor);

#endif /* INC_MOTOR_CONTROLLER_H_ */
