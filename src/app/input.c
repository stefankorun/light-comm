/*
 * input.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Stefan
 */

#include "input.h"

void input_processSignal(uint16_t);
void input_appendFrame(uint16_t);

uint8_t baseSensorLength = 150;
uint8_t baseSensorTreshold = 1000;

void input_init(void) {
	oled_init();
	oled_clearScreen(OLED_COLOR_BLACK);

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
	// printf("sensorValue %d", sensorValue);
	if (lightStatus == LIGHT_ON) {
		sensorLength++;
	}
	if (sensorValue > baseSensorTreshold && lightStatus == LIGHT_OFF) {
		lightStatus = LIGHT_ON;
		// printf("LIGHT_ON\n");
	} else if (sensorValue < baseSensorTreshold && lightStatus == LIGHT_ON) {
		lightStatus = LIGHT_OFF;
		input_processSignal(sensorLength);
		sensorLength = 0;
	}
}

int8_t framePosition = -1;
uint16_t frameData = 0;
void input_processSignal(uint16_t sensorLength) {
	printf("sensorLength: %d\t", sensorLength);
	if (sensorLength > baseSensorLength * 4 || framePosition >= 8) {
		if (framePosition == -1) {
			printf("Packet started\n");
			framePosition++;
		} else {
			printf("Packet end: %c\n", (char) frameData);
			input_appendFrame(frameData);
			framePosition = -1;
			frameData = 0;
		}
	} else if (framePosition > -1) {
		if (sensorLength > baseSensorLength * 2) {
			printf("Signal registered: %d\n", 1);
			frameData |= 1 << framePosition++;
		} else {
			printf("Signal registered: %d\n", 0);
			frameData &= ~(1 << framePosition++);
		}
	}
}

int8_t packetPosition = 0;
char packetData[100];
void input_appendFrame(uint16_t frame) {
	if (edet_checkParity(frame)) {
		printf("Frame [%d] parity check failed!\n", frame);
	}
	frame &= ~(1 << 8); // clear parity bit

	packetData[packetPosition++] = (char) frame;
	if (frame == 0) {
		printf("Word: %s\n", packetData);
		oled_putString(1, 1, packetData, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		packetPosition = 0;
	}
}

//
// test functions
void input_checkLoopInt(void) {
	while (!light_getIrqStatus())
		;
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
