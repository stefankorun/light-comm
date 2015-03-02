/*
 * init.c
 *
 *  Created on: Feb 22, 2015
 *      Author: Stefan
 */

#include "output.h"

// Private
uint16_t output_signalDuration = 500;

TIM_MATCHCFG_Type TIM_MatchConfigStruct;
void timer1_init(void) {
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	// Initialize timer 0, prescale count time of 1ms
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue = 1000;
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch = TRUE;
	//Enable reset on MR0: TIMER will not reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch = TRUE;
	//do no thing for external output
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	// Set Match value, count value is time (timer * 1000uS =timer mS )
	TIM_MatchConfigStruct.MatchValue = output_signalDuration;
	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM1, &TIM_MatchConfigStruct);
	// To start timer 0
	// TIM_Cmd(LPC_TIM1, ENABLE);
	NVIC_EnableIRQ(TIMER1_IRQn);
}
// IRQ Handlers
void TIMER1_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM1, 0);
	TIM_ResetCounter(LPC_TIM1);
	output_sendBitEnd();
}

// Public functions
// init
void output_init(void) {
	timer1_init();
	rgb_init();
}

// sending
uint8_t output_bufferData;
uint8_t output_bufferPosition = -1;
uint8_t output_bufferStatus = 0; // 0 - data; 1 - pause
void output_sendSignal(uint8_t data) {
	output_bufferData = data;
	output_sendBitEnd();
}
void output_sendBitStart(uint8_t val) {
	if (val == 0) {
		TIM_MatchConfigStruct.MatchValue = output_signalDuration;
	} else {
		TIM_MatchConfigStruct.MatchValue = output_signalDuration * 2;
	}
	TIM_ConfigMatch(LPC_TIM1, &TIM_MatchConfigStruct);
	TIM_Cmd(LPC_TIM1, ENABLE);
	printf("sending %d\n", val);
	rgb_setLeds(5);
}
void output_sendBitEnd() {
	rgb_setLeds(0);
	if (output_bufferStatus == 0) {
		output_bufferStatus = 1;
		TIM_MatchConfigStruct.MatchValue = output_signalDuration;
		TIM_ConfigMatch(LPC_TIM1, &TIM_MatchConfigStruct);
		TIM_Cmd(LPC_TIM1, ENABLE);
	} else {
		output_bufferStatus = 0;
		output_bufferPosition++;
		output_sendBitStart((output_bufferData >> output_bufferPosition) & 1);
	}
}

// Test functions
void output_sendSignalB(signal_length length) {
	rgb_setLeds(5);
	if (length == SIGNAL_SHORT) {
		Timer0_Wait(output_signalDuration);
	} else if (length == SIGNAL_LONG) {
		Timer0_Wait(output_signalDuration * 2);
	}
	rgb_setLeds(0);
	Timer0_Wait(output_signalDuration);
}
void output_readBit(void) {
	uint8_t i = 0;
	uint8_t testInt = 7;
	printf("Testing bit value of %d\n", testInt);
	while (i < 8) {
		printf("%d: %d\n", i, (testInt >> i) & 1);
		++i;
	}
}
