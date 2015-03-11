/*
 * error_detect.h
 *
 *  Created on: Mar 6, 2015
 *      Author: Stefan
 */

#include "lpc17xx_timer.h"

#ifndef APP_ERROR_DETECT_H_
#define APP_ERROR_DETECT_H_

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8  /* 11011 followed by 0's */

uint8_t crcTable[256];
typedef uint8_t crc;
crc crcTable[256];

uint8_t edet_checkParity(uint16_t);

#endif /* APP_ERROR_DETECT_H_ */
