/*
 * sici.h
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#ifndef COMPONENTS_SICI_SICI_H_
#define COMPONENTS_SICI_SICI_H_

#include "stm32_config.h"
#include "board.h"
#include "delay.h"
#include "onewrite.h"
// Interface enable time
#define T_EN 				150
#define T_EN_MAX 			400
#define T_LOW 				20
#define T_PRE_IFEN 			100

// messenger write/read command
#define ENTER_IF_WRITE_COMMAND 					0xABCD

#define POWER_DOWN_ISM_DATA						0x8000
#define POWER_DOWN_ISM_WRITE_COMMAND			0x8250

#define DISABLE_FAIL_INDICATION_DATA					0x0000
#define DISABLE_FAIL_INDICATION_WRITE_COMMAND			0x8010

#define WRITE_COMMAND			0x8402   // ghi bắt đầu tại địa chỉ 40hex

#define LIN_40_READ_COMMAND				0x0400
#define LIN_41_READ_COMMAND				0x0410
#define LIN_42_READ_COMMAND				0x0420
#define LIN_40_WRITE_COMMAND			0x8400
#define LIN_41_WRITE_COMMAND			0x8410
#define LIN_42_WRITE_COMMAND			0x8420

#define READ_DATA						0xFFFF

#define SET_ALL_ZEROS_WRITE_COMMAND		0x83E0
#define SET_ALL_ZEROS_DATA				0x0248


#define EEPROM_REFRESH_CMD 				0x024C
bool sici_enter_sensor_if(void);
uint16_t sici_write_16bit(uint16_t data);
void sici_end(void);

static inline void sici_write_1bit(uint8_t data){

	write_bit(data);
}
static inline uint8_t sici_read_1bit(void){

	uint8_t result = read_bit();
	return !(result);
}
static inline void sici_off(void){

	PORT_SET_MODE_OUTPUT(SICI_GPIO_Port, SICI_Pin);
	PORT_WRITE_LOW(SICI_GPIO_Port, SICI_Pin);

	PORT_SET_MODE_OUTPUT(OCD2_GPIO_Port, OCD2_Pin);
	PORT_WRITE_LOW(OCD2_GPIO_Port, OCD2_Pin);

	PORT_SET_MODE_OUTPUT(POWER_GPIO_Port, POWER_Pin);
	PORT_WRITE_LOW(POWER_GPIO_Port, POWER_Pin);

	delay_us(100);
}
#endif /* COMPONENTS_SICI_SICI_H_ */
