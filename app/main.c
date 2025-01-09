/*
 * main.c
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#include "board.h"
#include "tle_4972_current_sensor.h"
#include "delay.h"
#define PROJECT_UPDATE_1KHZ_ISR   TIM8_UP_IRQHandler
static void project_init(void);

//uint8_t test_check[16] = {0};

Tle_4972 sensor_config;
int main(void){

	project_init();
	while(1){

	};
}

static void project_init(void) {

	__disable_irq();
	board_hardware_init();
	delay_hardware_init_ex();
	tle_4972_init_ex(&sensor_config);
	tle_4972_begin(&sensor_config);

	delay_us(500);
	tle_4972_set_mesa(&sensor_config,S6);

	__enable_irq();
}

void PROJECT_UPDATE_1KHZ_ISR(void){

	UPDATE_1KHX_IT_CLEAR_FLAG;
}
