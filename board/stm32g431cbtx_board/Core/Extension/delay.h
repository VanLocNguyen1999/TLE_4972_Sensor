/*
 * delay.h
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#ifndef BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_DELAY_H_
#define BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_DELAY_H_

#include "stm32_config.h"

#define DELAY_TIMER				TIM4
// SysTick_Handler function will be called every 1 us
static inline void delay_hardware_init_ex(){

	LL_TIM_EnableCounter(DELAY_TIMER);
}


static inline void delay_us(uint32_t us) {
    LL_TIM_SetCounter(TIM4, 0); // Reset counter to 0
    while (LL_TIM_GetCounter(TIM4) < us); // Wait until counter reaches 'us'
}

static inline void delay_ms(uint32_t ms)
{
	// Wait until ms reach zero
	while (ms--)
	{
		// Delay 1ms
		delay_us(1000);
	}
}

#endif /* BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_DELAY_H_ */
