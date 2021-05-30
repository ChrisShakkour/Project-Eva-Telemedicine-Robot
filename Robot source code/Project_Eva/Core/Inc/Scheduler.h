/*
 * Scheduler.h
 *
 *  Created on: Oct 14, 2020
 *      Author: chris
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <math.h>

#define SCHEDULER_UNHALT false
#define SPI_COUNT 9
#define I2C_COUNT 2
#define HCSR04_COUNT 4
#define LED_COUNT 9

void scheduler();
int Scheduler_Halt_Status();
int Scheduler_Control();
int get_Scheduler_Speed();
int get_Scheduler_Angle();

#endif /* SRC_SCHEDULER_H_ */
