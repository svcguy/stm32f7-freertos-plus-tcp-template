/**
  ******************************************************************************
  * @file           : uart.c
  * @brief          :
  * @author         : svcguy
  * @details        :
  * @copyright      : Copyright svcguy 05/2024
  *                   Permission is hereby granted, free of charge, to any
  *                   person obtaining a copy of this software and associated
  *                   documentation files (the “Software”), to deal in the
  *                   Software without restriction, including without
  *                   limitation the rights to use, copy, modify, merge,
  *                   publish, distribute, sublicense, and/or sell copies of
  *                   the Software, and to permit persons to whom the Software
  *                   is furnished to do so, subject to the following
  *                   conditions:
  *
  *                   The above copyright notice and this permission notice 
  *                   shall be included in all copies or substantial portions 
  *                   of the Software. 
  *
  *                   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  *                   KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  *                   WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  *                   PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
  *                   OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  *                   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
  *                   OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
  *                   THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  ******************************************************************************
  */

/* Includes-----------------------------------------------------------------*/
#include "uart.h"
/* Private typedef----------------------------------------------------------*/
/* Private define-----------------------------------------------------------*/
/* Private macro------------------------------------------------------------*/
/* Private variables--------------------------------------------------------*/
UART_HandleTypeDef huart3;
/* Private function prototypes----------------------------------------------*/
/* Private functions--------------------------------------------------------*/

/**
 * @brief   This function initializes the UART
 * @param   None
 * @returns HAL_OK on success, error code on failure
*/
HAL_StatusTypeDef
UART_Init(void)
{
  HAL_StatusTypeDef ret = HAL_OK;

  // Setup the UART clock
  RCC_PeriphCLKInitTypeDef uartClock;
  uartClock.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  uartClock.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;

  if(HAL_RCCEx_PeriphCLKConfig(&uartClock) != HAL_OK)
  {
    return HAL_ERROR;
  }

  // Setup the UART
  huart3.Instance           = USART3;
  huart3.Init.Mode          = UART_MODE_TX_RX;
  huart3.Init.BaudRate      = 115200;
  huart3.Init.WordLength    = UART_WORDLENGTH_8B;
  huart3.Init.Parity        = UART_PARITY_NONE;
  huart3.Init.StopBits      = UART_STOPBITS_1;
  huart3.Init.HwFlowCtl     = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling  = UART_OVERSAMPLING_16;

  __HAL_RCC_USART3_CLK_ENABLE();
  ret = HAL_UART_Init(&huart3);

  if(ret != HAL_OK)
  {
    // Handle error
    return HAL_ERROR;
  }
  
  return ret;
}