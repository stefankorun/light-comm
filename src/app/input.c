/*
 * input.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Stefan
 */

#include "input.h"

void input_init(void) {
	light_enable();
	light_setRange(LIGHT_RANGE_64000);

	light_setHiThreshold(1000);
	light_setLoThreshold(-1);
}

LightStatus light_status = LIGHT_OFF;
void input_checkLoop(void) {
	while(!light_getIrqStatus());
	light_clearIrqStatus();
	printf("LIGHT_ON: %d\n", (int) light_read());
	if (light_status == LIGHT_OFF) {
		light_status = LIGHT_ON;
		printf("LIGHT_ON: %d\n", (int) light_read());
		light_setHiThreshold(-1);
//		light_setLoThreshold(700);
	} else {
		light_status = LIGHT_OFF;
		printf("LIGHT_OFF: %d\n", (int) light_read());
		light_setHiThreshold(700);
		light_setLoThreshold(-1);
	}
}

void input_testRange(void) {
	light_setRange(LIGHT_RANGE_1000);
	Timer0_Wait(500);
	printf("LIGHT_RANGE_1000: %d\n", (int) light_read());

	light_setRange(LIGHT_RANGE_4000);
	Timer0_Wait(500);
	printf("LIGHT_RANGE_4000: %d\n", (int) light_read());

	light_setRange(LIGHT_RANGE_16000);
	Timer0_Wait(500);
	printf("LIGHT_RANGE_16000: %d\n", (int) light_read());

	light_setRange(LIGHT_RANGE_64000);
	Timer0_Wait(500);
	printf("LIGHT_RANGE_64000: %d\n", (int) light_read());

	Timer0_Wait(5000);
}
void input_testWidth(void) {
	light_setWidth(LIGHT_WIDTH_04BITS);
	Timer0_Wait(500);
	printf("LIGHT_WIDTH_04BITS: %d\n", (int) light_read());

	light_setWidth(LIGHT_WIDTH_08BITS);
	Timer0_Wait(500);
	printf("LIGHT_WIDTH_08BITS: %d\n", (int) light_read());

	light_setWidth(LIGHT_WIDTH_12BITS);
	Timer0_Wait(500);
	printf("LIGHT_WIDTH_12BITS: %d\n", (int) light_read());

	light_setWidth(LIGHT_WIDTH_16BITS);
	Timer0_Wait(500);
	printf("LIGHT_WIDTH_16BITS: %d\n", (int) light_read());

	Timer0_Wait(5000);
}
