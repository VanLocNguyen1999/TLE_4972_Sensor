/*
 * tle_4972_current_sensor.c
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#include "tle_4972_current_sensor.h"

void tle_4972_init_ex(Tle_4972* sensor_config){

	sensor_config->meas = 0;
	sensor_config->mode = 0;
	/*==================*/
	sensor_config->configRegs[0] = 0;
	sensor_config->configRegs[1] = 0;
	sensor_config->configRegs[2] = 0;
	/*==================*/
	sensor_config->check[0] = 0;
	sensor_config->check[1] = 0;
	sensor_config->check[2] = 0;
}
bool tle_4972_begin(Tle_4972* sensor_config){

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
	// read out config
	sici_write_16bit(LIN_40_READ_COMMAND); // khởi tạo đọc dữ liệu ở địa chỉ 40hex
	sensor_config->configRegs[0] = sici_write_16bit(LIN_41_READ_COMMAND); // khởi tạo đọc dữ liệu ở địa chỉ 41hex và trả về dữ liệu ở địa chỉ 40hex
	sensor_config->configRegs[1] = sici_write_16bit(LIN_42_READ_COMMAND); // khởi tạo đọc dữ liệu ở địa chỉ 42hex và trả về dữ liệu ở địa chỉ 41hex
	sensor_config->configRegs[2] = sici_write_16bit(READ_DATA);			// đọc giá trị ở địa ở địa chỉ  trươc đó (42hex)
//
////	 Write + Read to check if interface is Working
	sici_write_16bit(LIN_40_WRITE_COMMAND);
	sici_write_16bit(0x1234);

	sici_write_16bit(LIN_40_READ_COMMAND);
	if(sici_write_16bit(READ_DATA) != 0x1234){

		tle_4972_end();
		return FLASE;
	}
		//power on ISM
	sici_write_16bit(POWER_DOWN_ISM_WRITE_COMMAND);
	sici_write_16bit(POWER_DOWN_ISM_DATA);
	tle_4972_end();

	sensor_config->meas = (sensor_config->configRegs[0]&0x001F);
	sensor_config->mode = (sensor_config->configRegs[0]&0x0060)>>5;
	return TRUE;
}


