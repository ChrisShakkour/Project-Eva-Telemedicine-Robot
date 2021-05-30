/*
 * UART_handler.h
 *
 *  Created on: Oct 14, 2020
 *      Author: chris
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
#define BEGIN true
#define ENDED true
#define START false
#define RC_ENABLED true
#define UART_UNHALT false

void Interrupt_On_Character();
int get_Throttle();
int get_angle();
int Remote_Control();
int UART_Halt_Status();

#endif /* INC_UART_HANDLER_H_ */
