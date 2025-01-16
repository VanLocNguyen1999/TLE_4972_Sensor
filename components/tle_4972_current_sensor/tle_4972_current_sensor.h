/*
 * tle_4972_current_sensor.h
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#ifndef COMPONENTS_TLE_4972_CURRENT_SENSOR_TLE_4972_CURRENT_SENSOR_H_
#define COMPONENTS_TLE_4972_CURRENT_SENSOR_TLE_4972_CURRENT_SENSOR_H_


#include "stm32_config.h"
#include "gpio.h"
#include "delay.h"
#include "board.h"
#include "sici.h"

typedef struct Tle_4972_t Tle_4972;

typedef enum  {
     S1	= 0x5,
     S2	= 0x6,
     S3 = 0x8,
     S4	= 0xC,
     S5	= 0x10,
     S6 = 0x18,
} MEAS;

typedef enum {
     SD_BID		= 0x00,    // Single-ended bidirectional
     FD			= 0x01,         // Full-duplex
     SD_UNI		= 0x02,    // Single-ended unidirectional
     S_ENDED	= 0x03,   // Stopped or ended
} OPMODE;

typedef enum {
     TIM_0_NS 		= 0x0,
     TIM_500_NS		= 0x1,
     TIM_1000_NS	= 0x2,
     TIM_1500_NS	= 0x3,
     TIM_2000_NS	= 0x4,
     TIM_2500_NS	= 0x5,
     TIM_3000_NS	= 0x6,
     TIM_3500_NS	= 0x7,
     TIM_4000_NS	= 0x8,
     TIM_4500_NS	= 0x9,
     TIM_5000_NS	= 0xA,
     TIM_5500_NS	= 0xB,
     TIM_6000_NS	= 0xC,
     TIM_6500_NS	= 0xD,
     TIM_7000_NS	= 0xE,
     TIM_7500_NS	= 0xF,
} COD_FILTER;

typedef enum {
     COD_ENABLE		= 0x0,    // Single-ended bidirectional
     COD_DISABLE	= 0x1,        // Full-duplex
} COD_STATE;
struct Tle_4972_t{

	 uint16_t meas;
	 uint16_t mode;
	 uint16_t configRegs[3];

	 uint16_t check[3];
};

void tle_4972_init_ex(Tle_4972* sensor_config);
bool tle_4972_begin(Tle_4972* sensor_config);
bool tle_4972_reset(void);

static inline bool tle_4972_set_mesa(Tle_4972* sensor_config, MEAS meas);
static inline bool tle_4972_setconfig(Tle_4972* sensor_config);
static inline void tle_4972_end(void);

static inline bool tle_4972_set_mesa(Tle_4972* sensor_config, MEAS meas){


	uint16_t config_backup = sensor_config->configRegs[0];

	sensor_config->configRegs[0]  &= 0xFFE0;
	sensor_config->configRegs[0] |= (meas&0x1F);
	if(tle_4972_setconfig(sensor_config)){
		sensor_config->meas = meas;
		return TRUE;
	}
	sensor_config->configRegs[0] = config_backup;
	return FLASE;
}

static inline bool tle_4972_setconfig(Tle_4972* sensor_config){

	if (!sici_enter_sensor_if()){
		tle_4972_end();
		return FLASE;
	}
	//power down ISM
	sici_write_16bit(POWER_DOWN_ISM_WRITE_COMMAND);
	sici_write_16bit(POWER_DOWN_ISM_DATA);
	//Disable failure indication
	sici_write_16bit(DISABLE_FAIL_INDICATION_WRITE_COMMAND);
	sici_write_16bit(DISABLE_FAIL_INDICATION_DATA);
	// send config to line 40hex - 42hex

	sici_write_16bit(SET_ALL_ONES_WRITE_COMMAND);
	sici_write_16bit(SET_ALL_ONES_DATA);

	BOARD_OCD2_ON;
	delay_ms(T_EEPVPROG);
	BOARD_OCD2_OFF;
	sici_write_16bit(EEPROM_REFRESH_CMD);
	delay_us(T_EEPWAIT);

	sici_write_16bit(SET_ALL_ZEROS_WRITE_COMMAND);
	sici_write_16bit(SET_ALL_ZEROS_DATA);

	BOARD_OCD2_ON;
	delay_ms(T_EEPVPROG);
	BOARD_OCD2_OFF;
	sici_write_16bit(EEPROM_REFRESH_CMD);
	delay_us(T_EEPWAIT);
	/*------------------*/
	sici_write_16bit(LIN_40_WRITE_COMMAND);
	sici_write_16bit(sensor_config->configRegs[0]); // ghi vào địa chỉ trước đó 40hex

	BOARD_OCD2_ON;
	delay_ms(T_EEPVPROG);
	BOARD_OCD2_OFF;
	sici_write_16bit(EEPROM_REFRESH_CMD);
	delay_us(T_EEPWAIT);

	sici_write_16bit(LIN_41_WRITE_COMMAND);
	sici_write_16bit(sensor_config->configRegs[1]);

	BOARD_OCD2_ON;
	delay_ms(T_EEPVPROG);
	BOARD_OCD2_OFF;
	sici_write_16bit(EEPROM_REFRESH_CMD);
	delay_us(T_EEPWAIT);

	sici_write_16bit(LIN_42_WRITE_COMMAND);
	sici_write_16bit(sensor_config->configRegs[2]);

	BOARD_OCD2_ON;
	delay_ms(T_EEPVPROG);
	BOARD_OCD2_OFF;
	sici_write_16bit(EEPROM_REFRESH_CMD);
	delay_us(T_EEPWAIT);
	// Read data again and check
	sici_write_16bit(LIN_40_READ_COMMAND);
	sensor_config->check[0] = sici_write_16bit(LIN_41_READ_COMMAND);
	sensor_config->check[1] = sici_write_16bit(LIN_42_READ_COMMAND);
	sensor_config->check[2] = sici_write_16bit(READ_DATA);

	if(sensor_config->check[0] != sensor_config->configRegs[0]){
		return FLASE;
	}
	if(sensor_config->check[1] != sensor_config->configRegs[1]){
		return FLASE;
	}
	if(sensor_config->check[2] != sensor_config->configRegs[2]){
		return FLASE;
	}

	//power on ISM
	sici_write_16bit(POWER_DOWN_ISM_WRITE_COMMAND);
	sici_write_16bit(POWER_DOWN_ISM_DATA);
	tle_4972_end();
	return TRUE;
}
static inline void tle_4972_end(void){

	sici_off();
}
#endif /* COMPONENTS_TLE_4972_CURRENT_SENSOR_TLE_4972_CURRENT_SENSOR_H_ */
