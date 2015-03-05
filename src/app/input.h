/*
 * input.h
 *
 *  Created on: Feb 24, 2015
 *      Author: Stefan
 */

#ifndef APP_INPUT_H_
#define APP_INPUT_H_

#include "lpc17xx_i2c.h"
#include "lpc17xx_timer.h"
#include "light.h"

typedef enum {
	LIGHT_ON, LIGHT_OFF
} LightStatus;

void input_init(void);

#endif /* APP_INPUT_H_ */
