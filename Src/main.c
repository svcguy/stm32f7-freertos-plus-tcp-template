/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32 VS Code Extension
 * @brief          : Main program body
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

#include "main.h"

/**
 * @brief   The application entry point.  It will setup the hardware,
 *          initalize the RTOS and TCP/IP stack and then start the scheduler
 * @param   None
 * @returns It's bare metal, so it doesn't
*/
int 
main(void)
{
  // HW Init
  HAL_Init();
  Clock_Init();
  GPIO_Init();
  RNG_Init();
  UART_Init();
  printf("Hardware Init Complete!\r\n");

  // RTOS Init
  RTOS_Init();
  printf("RTOS Init Complete!\r\n");

  // TCP/IP Init
  Network_Init();
  printf("TCP/IP Init Complete\r\n");

  // Start FreeRTOS-Kernel
  vTaskStartScheduler();

  // If we've gotten here, something has gone sideways
  Error_Handler();
}

/**
 * @brief   The error handler function
 * @param   None
 * @returns None
*/
void 
Error_Handler(void)
{
  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    // Debugger is attached so break here
    __BKPT(0);
  }
  else
  {
    while(1)
    {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
      HAL_Delay(100);
    }
  }
}

/**
 * @brief   The failed assertion function
 * @param   file The filename where assert_failed was called
 * @param   line The line number where assert_failed was called
 * @returns None
*/
void 
assert_failed(uint8_t *file, uint32_t line)
{
  UNUSED(file);
  UNUSED(line);

  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    // Debugger is attached so break here
    __BKPT(0);
  }
  else
  {
    while(1)
    {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
      HAL_Delay(100);
    }
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  ch the character to print to UART
  * @retval None
  */
int
__io_putchar(int ch)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval The character recieved from the UART
  */
int
__io_getchar(void)
{
  return 0;
}

/**
 * @brief   FreeRTOS's logging function
 * @param   pcFormatString a printf-like format string
 * @returns None
*/
void
vLoggingPrintf(const char *pcFormatString, ...)
{

}
