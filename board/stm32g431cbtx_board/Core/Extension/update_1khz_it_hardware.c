/*
 * update_1khz_it_hardware.c
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */
#include "update_1khz_it_hardware.h"

static void update_1khz_it_hardware_start(void);

void update_1khz_it_hardware_init(){

	update_1khz_it_hardware_start();
	NVIC_SetPriority(UPDATE_1KHX_IT_IRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	update_1khz_it_hardware_disable();
}

static void update_1khz_it_hardware_start(void){

	LL_TIM_EnableIT_UPDATE(UPDATE_1KHZ_IT_TIMER);
	LL_TIM_EnableCounter(UPDATE_1KHZ_IT_TIMER);
}

void update_1khz_it_hardware_enable(){

	NVIC_EnableIRQ(UPDATE_1KHX_IT_IRQ);
}
void update_1khz_it_hardware_disable(){

	NVIC_DisableIRQ(UPDATE_1KHX_IT_IRQ);
}
