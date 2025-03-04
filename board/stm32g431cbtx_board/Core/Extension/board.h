/*
 * board.h
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#ifndef BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_BOARD_H_
#define BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_BOARD_H_

#include "stm32_config.h"
#include "gpio.h"
#include "tim.h"
#include "update_1khz_it_hardware.h"


#define BOARD_TP1_SET			LL_GPIO_SetOutputPin(TP1_GPIO_Port,TP1_Pin)
#define BOARD_TP1_RESET			LL_GPIO_ResetOutputPin(TP1_GPIO_Port,TP1_Pin)

#define BOARD_TP2_SET			LL_GPIO_SetOutputPin(TP2_GPIO_Port,TP2_Pin)
#define BOARD_TP2_RESET			LL_GPIO_ResetOutputPin(TP2_GPIO_Port,TP2_Pin)

#define BOARD_TP3_SET			LL_GPIO_SetOutputPin(TP3_GPIO_Port,TP3_Pin)
#define BOARD_TP3_RESET			LL_GPIO_ResetOutputPin(TP3_GPIO_Port,TP3_Pin)

void board_hardware_init(void);

typedef enum{

	GPIO_MODE_INPUT  		= 0x0,
	GPIO_MODE_OUTPUT 		= 0x1,
//	GPIO_MODE_ALTERNATE 	= LL_GPIO_MODE_ALTERNATE,
//	GPIO_MODE_ANALOG 		= LL_GPIO_MODE_ANALOG,
}GPIO_MODE_t;

static inline void GPIO_Set_Mode(GPIO_TypeDef *GPIOx, uint32_t Pin,GPIO_MODE_t mode) {

	uint32_t current_mode = LL_GPIO_GetPinMode(GPIOx, Pin);
	if (current_mode != mode) {
		if (mode == GPIO_MODE_OUTPUT) {
			LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_OUTPUT); // Chuyển chân GPIO sang chế độ đầu ra
			LL_GPIO_SetPinSpeed(GPIOx, Pin, LL_GPIO_SPEED_FREQ_VERY_HIGH); // Đặt tốc độ chân (tốc độ cao)
			LL_GPIO_SetPinOutputType(GPIOx, Pin, LL_GPIO_OUTPUT_PUSHPULL); // Loại điện ra là Push-Pull
			LL_GPIO_SetPinPull(GPIOx, Pin, LL_GPIO_PULL_NO); // Không có pull-up hoặc pull-down
		} else {
			LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_INPUT); // Chuyển chân GPIO sang chế độ đầu vào
			LL_GPIO_SetPinPull(GPIOx, Pin, LL_GPIO_PULL_NO); // Không kéo lên hay kéo xuống (Pull-up hoặc Pull-down)

		}
	}
}

#endif /* BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_BOARD_H_ */
