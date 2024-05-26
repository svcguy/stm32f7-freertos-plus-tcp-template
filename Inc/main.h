/**
 ******************************************************************************
 * @file           : main.h
 * @author         : Auto-generated by STM32 VS Code Extension
 * @brief          : Main program header
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "list.h"
#include "FreeRTOS_IP.h"
#include "stm32f7xx_hal.h"
#include "clock.h"
#include "gpio.h"
#include "rng.h"
#include "rtos.h"
#include "network.h"
#include "uart.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void assert_failed(uint8_t *file, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */