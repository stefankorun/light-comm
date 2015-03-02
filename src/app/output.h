/*
 * init.h
 *
 *  Created on: Feb 22, 2015
 *      Author: Stefan
 */

#ifndef APP_OUTPUT_H_
#define APP_OUTPUT_H_

#include "lpc17xx_timer.h"
#include "rgb.h"

typedef enum {
	SIGNAL_SHORT, SIGNAL_LONG
} signal_length;

typedef struct {

} SignalStatus;

void output_init(void);
void output_sendSignal(uint8_t);

#endif /* APP_OUTPUT_H_ */
