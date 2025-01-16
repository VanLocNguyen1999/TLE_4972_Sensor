/*
 * onewrite.h
 *
 *  Created on: Jan 6, 2025
 *      Author: vanlo
 */

#ifndef UTILS_ONEWRITE_ONEWRITE_H_
#define UTILS_ONEWRITE_ONEWRITE_H_

#include "board.h"
#include "delay.h"
#include "onewrite.h"

#define PORT_READ(port, pinmask)      		((port->IDR & pinmask) != 0) // Đọc trạng thái chân
#define PORT_WRITE_LOW(port, pinmask) 		(port->BSRR = ((uint32_t)pinmask << 16)) // Đặt chân LOW
#define PORT_WRITE_HIGH(port, pinmask) 		(port->BSRR = pinmask) // Đặt chân HIGH

#define PORT_SET_MODE_INPUT(port, pinmask) do { \
												LL_GPIO_SetPinMode(port, pinmask, LL_GPIO_MODE_INPUT); \
												LL_GPIO_SetPinPull(port, pinmask, LL_GPIO_PULL_NO); \
											} while(0)

#define PORT_SET_MODE_OUTPUT(port, pinmask) do { \
												LL_GPIO_SetPinMode(port, pinmask, LL_GPIO_MODE_OUTPUT); \
												LL_GPIO_SetPinPull(port, pinmask, LL_GPIO_PULL_NO); \
												LL_GPIO_SetPinSpeed(port, pinmask, LL_GPIO_SPEED_FREQ_VERY_HIGH); \
											} while(0)

#define T_BIT				1000
// write time
#define T1_0 				(0.33f*T_BIT)  // low time sending 0
#define T1_1 				(0.67f*T_BIT) 	 // low time sending 1
#define T2_0 				(T_BIT - T1_0)  // High time sending 0
#define T2_1 				(T_BIT - T2_0)  // High time sending 1
// read time
#define T3					(0.1f*T_BIT)   // low time before read
#define T4					2*(T2_0 - T1_0)  // Response time
#define TR					(0.65*T4)  // Reading time
#define T5					(T_BIT - T4)  // time between 2 bits


static inline void write_bit(uint8_t data) {

	if (data & 1) {
		PORT_SET_MODE_OUTPUT(SICI_GPIO_Port, SICI_Pin);	// drive output low
		PORT_WRITE_LOW(SICI_GPIO_Port, SICI_Pin);
		delay_us(T1_1);
		PORT_WRITE_HIGH(SICI_GPIO_Port, SICI_Pin);	// drive output high
		delay_us(T2_1);
	} else {
		PORT_SET_MODE_OUTPUT(SICI_GPIO_Port, SICI_Pin);	// drive output low
		PORT_WRITE_LOW(SICI_GPIO_Port, SICI_Pin);
		delay_us(T1_0);
		PORT_WRITE_HIGH(SICI_GPIO_Port, SICI_Pin);	// drive output high
		delay_us(T2_0);
	}
}
static inline uint8_t read_bit(void) {
	uint8_t result;
	PORT_SET_MODE_OUTPUT(SICI_GPIO_Port, SICI_Pin);
	PORT_WRITE_LOW(SICI_GPIO_Port, SICI_Pin);
	delay_us(T3);
	PORT_SET_MODE_INPUT(SICI_GPIO_Port, SICI_Pin);// let pin float, pull up will raise
	delay_us(TR);
	BOARD_TP2_SET;
	result = PORT_READ(SICI_GPIO_Port, SICI_Pin);
	delay_us(T4 - T3 - TR + T5);
	BOARD_TP2_RESET;
	return result;
}

#endif /* UTILS_ONEWRITE_ONEWRITE_H_ */
