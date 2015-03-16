/*
 * error_detect.c
 *
 *  Created on: Mar 6, 2015
 *      Author: Stefan
 */

#include "error_detect.h"

void edet_crcInit(void) {
	crc remainder;
	int dividend;

	/*
	 * Compute the remainder of each possible dividend.
	 */
	for (dividend = 0; dividend < 256; ++dividend) {
		/*
		 * Start with the dividend followed by zeros.
		 */
		remainder = dividend << (WIDTH - 8);
		uint8_t bit;
		/*
		 * Perform modulo-2 division, a bit at a time.
		 */
		for (bit = 8; bit > 0; --bit) {
			/*
			 * Try to divide the current data bit.
			 */
			if (remainder & TOPBIT) {
				remainder = (remainder << 1) ^ POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}

		/*
		 * Store the result into the table.
		 */
		crcTable[dividend] = remainder;
	}

} /* crcInit() */

uint8_t edet_crcFast(uint8_t message[], int nBytes) {
	uint8_t data;
	uint8_t remainder = 0;
	int byte;

	/*
	 * Divide the message by the polynomial, a byte at a time.
	 */
	for (byte = 0; byte < nBytes; ++byte) {
		data = message[byte] ^ (remainder >> (WIDTH - 8));
		remainder = crcTable[data] ^ (remainder << 8);
	}

	/*
	 * The final remainder is the CRC.
	 */
	return (remainder);
}

uint8_t edet_checkParity(uint16_t frame) {
	uint8_t parity = 0;
	uint8_t i = 0;
	for (i = 0; i <= 8; i++) {
		parity = (parity + ((frame >> i) & 1)) % 2;
	}
	return parity;
}
