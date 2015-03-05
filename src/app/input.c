/*
 * input.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Stefan
 */

#include "input.h"

void input_processSignal(sensorLength);

void input_init(void) {
	light_enable();
	light_setRange(LIGHT_RANGE_64000);
	light_setWidth(LIGHT_WIDTH_08BITS);
	Timer0_Wait(100);

//	light_setHiThreshold(700);
//	light_setLoThreshold(-1);
}

LightStatus lightStatus = LIGHT_OFF;
uint16_t sensorValue = -1;
uint16_t sensorLength = 0;
void input_checkLoop(void) {
	sensorValue = light_read();
//	printf("sensorValue %d", sensorValue);
	if (lightStatus == LIGHT_ON) {
		sensorLength++;
	}
	if (sensorValue > 1000 && lightStatus == LIGHT_OFF) {
		lightStatus = LIGHT_ON;
//		printf("LIGHT_ON\n");
	} else if (sensorValue < 1000 && lightStatus == LIGHT_ON) {
		lightStatus = LIGHT_OFF;
		input_processSignal(sensorLength);
		sensorLength = 0;
	}
}

int8_t packetPosition = -1;
uint16_t packedData = 0;
void input_processSignal(sensorLength) {
	if (sensorLength > 600 || packetPosition >= 8) {
		if (packetPosition == -1) {
			printf("Packet started\n");
			packetPosition++;
		} else {
			printf("Packet end: %c\n", (char) packedData);
			packetPosition = -1;
			packedData = 0;
		}
	} else if (sensorLength > 300) {
		printf("Signal registered: %d\n", 1);
		packedData |= 1 << packetPosition++;
	} else {
		printf("Signal registered: %d\n", 0);
		packedData &= ~(1 << packetPosition++);
	}
//	printf("Signal registered: %d\n", sensorLength);
}


void input_checkLoopInt(void) {
	while(!light_getIrqStatus());
	light_clearIrqStatus();
	printf("LIGHT_ON: %d\n", (int) light_read());
	return;

	if (lightStatus == LIGHT_OFF) {
		lightStatus = LIGHT_ON;
		printf("LIGHT_ON: %d\n", (int) light_read());
		light_setHiThreshold(-1);
		light_setLoThreshold(300);
	} else {
		lightStatus = LIGHT_OFF;
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
