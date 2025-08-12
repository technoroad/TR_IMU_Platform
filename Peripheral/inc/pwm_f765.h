/*
 * pwm_f765.h
 *
 *  Created on: 2025/08/12
 *      Author: techno-road
 */

#ifndef INC_PWM_F765_H_
#define INC_PWM_F765_H_

#define F7_2KHZ_PWM_PRES 1
#define F7_2KHZ_PWM_ARR 53999
#define F7_4KHZ_PWM_PRES 1
#define F7_4KHZ_PWM_ARR 26999
#define F7_9KHZ_PWM_PRES 1
#define F7_9KHZ_PWM_ARR 11999

void PWM_Initialize();
void PWM_StartOutput();

#endif /* INC_PWM_F765_H_ */
