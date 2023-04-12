/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_iwdg.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADIS_nRST_Pin LL_GPIO_PIN_3
#define ADIS_nRST_GPIO_Port GPIOE
#define SPI4_nCS_Pin LL_GPIO_PIN_4
#define SPI4_nCS_GPIO_Port GPIOE
#define DEBUG_PIN0_Pin LL_GPIO_PIN_0
#define DEBUG_PIN0_GPIO_Port GPIOC
#define DEBUG_PIN1_Pin LL_GPIO_PIN_1
#define DEBUG_PIN1_GPIO_Port GPIOC
#define DEBUG_PIN2_Pin LL_GPIO_PIN_2
#define DEBUG_PIN2_GPIO_Port GPIOC
#define DEBUG_PIN3_Pin LL_GPIO_PIN_3
#define DEBUG_PIN3_GPIO_Port GPIOC
#define LD1_Pin LL_GPIO_PIN_0
#define LD1_GPIO_Port GPIOA
#define LD2_Pin LL_GPIO_PIN_1
#define LD2_GPIO_Port GPIOA
#define LD3_Pin LL_GPIO_PIN_2
#define LD3_GPIO_Port GPIOA
#define CONF_SW8_Pin LL_GPIO_PIN_3
#define CONF_SW8_GPIO_Port GPIOA
#define CONF_SW7_Pin LL_GPIO_PIN_4
#define CONF_SW7_GPIO_Port GPIOA
#define CONF_SW6_Pin LL_GPIO_PIN_5
#define CONF_SW6_GPIO_Port GPIOA
#define CONF_SW5_Pin LL_GPIO_PIN_6
#define CONF_SW5_GPIO_Port GPIOA
#define CONF_SW4_Pin LL_GPIO_PIN_7
#define CONF_SW4_GPIO_Port GPIOA
#define CONF_SW3_Pin LL_GPIO_PIN_4
#define CONF_SW3_GPIO_Port GPIOC
#define CONF_SW2_Pin LL_GPIO_PIN_5
#define CONF_SW2_GPIO_Port GPIOC
#define CONF_SW1_Pin LL_GPIO_PIN_0
#define CONF_SW1_GPIO_Port GPIOB
#define USER_SW_Pin LL_GPIO_PIN_1
#define USER_SW_GPIO_Port GPIOB
#define ADIS_DR_Pin LL_GPIO_PIN_9
#define ADIS_DR_GPIO_Port GPIOB
#define ADIS_DR_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
