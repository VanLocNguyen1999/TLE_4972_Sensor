/*
 * main.c
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#include "board.h"
#include "main.h"
#include "delay.h"

#define PROJECT_UPDATE_1KHZ_ISR   TIM8_UP_IRQHandler
static void project_init(void);
void write_8bit(uint8_t data);

uint8_t test_check = 0;
uint8_t test_pluse = 0;

int main(void){

	project_init();
	while(1){

	};
}

static void project_init(void) {

	__disable_irq();
	board_hardware_init();
	delay_hardware_init_ex();
	__enable_irq();
}

void PROJECT_UPDATE_1KHZ_ISR(void){

	delay_ms(10);
	test_check = 10;
	write_8bit(0x32);
	test_check = 0;
	UPDATE_1KHX_IT_CLEAR_FLAG;
}

void write_8bit(uint8_t data) {

	for (int i = 0; i < 8; i++) {
		if (data & 1) {
			PORT_SET_MODE_OUTPUT(TP3_GPIO_Port, TP3_Pin);	// drive output low
			BOARD_TP3_RESET;
			test_pluse = 0;
			delay_ms(30);
			BOARD_TP3_SET;	// drive output high
			test_pluse = 10;
			delay_ms(70);
		} else {
			PORT_SET_MODE_OUTPUT(TP3_GPIO_Port, TP3_Pin);	// drive output low
			BOARD_TP3_RESET;
			test_pluse = 0;
			delay_ms(70);
			BOARD_TP3_SET;	// drive output high
			test_pluse = 10;
			delay_ms(30);
		}
	}
}
