/*
 * stm32_config.h
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#ifndef BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_STM32_CONFIG_H_
#define BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_STM32_CONFIG_H_

#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_crs.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_gpio.h"


#include "stdlib.h"
#include <stdint.h>
#include <stdbool.h>

#include "user.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

#define ENABLE 	1			//xoa
#define DISABLE	0			//xoa

#define TRUE 	1			//xoa
#define FLASE	0			//xoa

#define SYS_STOP			PWM_DISABLE_OUTPUT
#define SYS_TICK			150000000UL

/* Private defines -----------------------------------------------------------*/
#define TP1_Pin LL_GPIO_PIN_4
#define TP1_GPIO_Port GPIOA
#define TP2_Pin LL_GPIO_PIN_5
#define TP2_GPIO_Port GPIOA
#define POWER_Pin LL_GPIO_PIN_15
#define POWER_GPIO_Port GPIOA
#define SICI_Pin LL_GPIO_PIN_3
#define SICI_GPIO_Port GPIOB
#define OCD2_Pin LL_GPIO_PIN_4
#define OCD2_GPIO_Port GPIOB

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,*/
#endif

#define OPTIMIZE_ATTR  FORCE_INLINE


#define FORCE_INLINE    inline __attribute__((always_inline))


static inline void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


#endif /* BOARD_STM32G431CBTX_BOARD_CORE_EXTENSION_STM32_CONFIG_H_ */
