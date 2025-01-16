/*
 * sici.c
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */


#include "sici.h"


bool sici_enter_sensor_if(void){

	sici_off();
	BOARD_POWER_ON;
	delay_us(T_PRE_IFEN);
   // send low pluse to activate Interface
	BOARD_SICI_OFF;
	delay_us(T_EN);
	BOARD_SICI_ON;
	delay_us(T_EN_MAX - T_EN);
	uint16_t rec = sici_write_16bit(ENTER_IF_WRITE_COMMAND);
	return (rec == 0);
}

uint16_t sici_write_16bit(uint16_t data)
{
	uint16_t dataOut = 0;
  for(int i = 0; i < 16; i++)
  {
	 sici_write_1bit((data>>(i))&0x1);
	 dataOut |= (sici_read_1bit()&0x1) << (i);
  }
  return dataOut;
}
