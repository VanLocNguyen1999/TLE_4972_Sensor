/*
 * update_1khz_it_hardware.h
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#ifndef BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_UPDATE_1KHZ_IT_HARDWARE_H_
#define BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_UPDATE_1KHZ_IT_HARDWARE_H_

#include "stm32_config.h"
#include "tim.h"
#include "board.h"

#define UPDATE_1KHZ_IT_TIMER				TIM8
#define UPDATE_1KHX_IT_IRQ					TIM8_UP_IRQn
#define UPDATE_1KHX_IT_CLEAR_FLAG			LL_TIM_ClearFlag_UPDATE(UPDATE_1KHZ_IT_TIMER)

void update_1khz_it_hardware_init(void);
void update_1khz_it_hardware_enable(void);
void update_1khz_it_hardware_disable(void);
#endif /* BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_UPDATE_1KHZ_IT_HARDWARE_H_ */
