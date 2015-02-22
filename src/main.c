/*****************************************************************************
 *   A demo example using several of the peripherals on the base board
 *
 *   Copyright(C) 2010, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************/

#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_timer.h"

// moj
#include "lpc17xx_rit.h"

#include "joystick.h"
#include "pca9532.h"
#include "led7seg.h"
#include "rotary.h"
#include "light.h"
#include "oled.h"
#include "acc.h"
#include "rgb.h"

#include "stdio.h"

#include "app/output.h"

static void init_ssp(void) {
	SSP_CFG_Type SSP_ConfigStruct;
	PINSEL_CFG_Type PinCfg;

	/*
	 * Initialize SPI pin connect
	 * P0.7 - SCK;
	 * P0.8 - MISO
	 * P0.9 - MOSI
	 * P2.2 - SSEL - used as GPIO
	 */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 7;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Funcnum = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);

	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP1, ENABLE);

}
static void init_i2c(void) {
	PINSEL_CFG_Type PinCfg;

	/* Initialize I2C2 pin connect */
	PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11;
	PINSEL_ConfigPin(&PinCfg);

	// Initialize I2C peripheral
	I2C_Init(LPC_I2C2, 100000);

	/* Enable I2C1 operation */
	I2C_Cmd(LPC_I2C2, ENABLE);
}
static void init_adc(void) {
	PINSEL_CFG_Type PinCfg;

	/*
	 * Init ADC pin connect
	 * AD0.0 on P0.23
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 23;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	/* Configuration for ADC :
	 * 	Frequency at 0.2Mhz
	 *  ADC channel 0, no Interrupt
	 */
	ADC_Init(LPC_ADC, 200000);
	ADC_IntConfig(LPC_ADC, ADC_CHANNEL_0, DISABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);

}

void sensor_init(void) {
	light_enable();
	light_setRange(LIGHT_RANGE_64000);
}

int waitTime = 250;
void emiter_testFrequency(void) {
	// on
	GPIO_SetValue(2, 64);
	Timer0_Wait(waitTime / 2);
	printf("on: %d\n", (int) light_read());
	Timer0_Wait(waitTime / 2);

	// off
	GPIO_ClearValue(2, 64);
	Timer0_Wait(waitTime / 2);
	printf("off: %d\n", (int) light_read());
	Timer0_Wait(waitTime / 2);
}
void rit_init(void) {
	RIT_Init(LPC_RIT);
	RIT_TimerConfig(LPC_RIT, 2000);
	RIT_Cmd(LPC_RIT, ENABLE);
}

int main(void) {
	init_i2c();
	init_ssp();
	init_adc();

	rgb_init();
	light_init();

	sensor_init();
//	rit_init();
	output_init();

	GPIO_SetDir(2, 0xFFFFFFFF, 1);
	while (1) {
//		emiter_testFrequency();
	}
}

uint8_t light_toggle = 0;
void RIT_IRQHandler(void) {
	printf("RIT_IRQHandler\n");
	if(light_toggle) {
		rgb_setLeds(RGB_RED);
	} else {
		rgb_setLeds(0);
	}
	light_toggle = !light_toggle;
}


