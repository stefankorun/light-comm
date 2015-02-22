/*
 * init.c
 *
 *  Created on: Feb 22, 2015
 *      Author: Stefan
 */

#include "output.h"

// Private
uint16_t signalLength = 300;

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
	TIM_MatchConfigStruct.MatchValue = signalLength;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM1, &TIM_MatchConfigStruct);
	// To start timer 0
//	TIM_Cmd(LPC_TIM1, ENABLE);

	NVIC_EnableIRQ(TIMER1_IRQn);
}

// IRQ Handlers
uint8_t light_toggle = 0;
void TIMER1_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM1, 0);
	if (light_toggle) {
		rgb_setLeds(5);
	} else {
		rgb_setLeds(0);
	}
	light_toggle = !light_toggle;
	TIM_Cmd(LPC_TIM1, ENABLE);
}

// Public functions
void output_init(void) {
	timer1_init();
	rgb_init();
}
void output_send_signal(signal_length length) {
	rgb_setLeds(5);
	if (length == SIGNAL_SHORT) {
		Timer0_Wait(signalLength);
	} else if (length == SIGNAL_LONG) {
		Timer0_Wait(signalLength * 2);
	}
	rgb_setLeds(0);
	Timer0_Wait(signalLength);
}
