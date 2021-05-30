/*
 * UART_handler.c
 *
 *  Created on: Oct 14, 2020
 *      Author: chris
 */

#include "main.h"

char buffer[10] = {0};
uint8_t bufffer_index = 0;
uint8_t Transmittion = ENDED;
uint8_t angle_count =0;
int Angle = 0;
int Strength = 0;

/*
 *
 */
void Interrupt_On_Character(){
//
	if(Transmittion == BEGIN){
		bufffer_index = 0;
		memset(&buffer, 0, sizeof(buffer));
		angle_count = 0;
		Transmittion = START;
	}
	if(bufffer_index > 9) {
		Transmittion = ENDED;
		return;
	}
	HAL_UART_Receive(&huart1, (uint8_t*)&buffer[bufffer_index], 1, 10);
	if(buffer[bufffer_index] == 'a') {
		angle_count = bufffer_index;
	}
	if(buffer[bufffer_index] == 's'){
		Transmittion = ENDED;
	}
	else {
		bufffer_index++;
		return;
	}
	if(bufffer_index > 3){
		Angle = atoi(&buffer[0]);
		Strength = atoi(&buffer[angle_count+1]);
	}
}

/*
 * This function makes it possible to get the
 * Strength data in different parts of the code.
 */
int get_Throttle(){
	if(Strength >= 0 && Strength <= 100) return Strength;
	else if(Strength > 100) return 100;
	return 0 ;
}

/*
 * This function makes it possible to get the
 * Angle data in different parts of the code.
 */
int get_angle(){
	if(Angle >= 0 && Angle <= 360) return Angle ;
	return 0 ;
}

/*Logic needs to be added in order to
 *manipulate control between SW & UART*/
int Remote_Control(){
	return RC_ENABLED;
}
int UART_Halt_Status(){
	return UART_UNHALT;
}

